/*
 * Copyright 2022 C Thing Software
 *
 * This file is part of Meazure.
 *
 * Meazure is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Meazure is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Meazure.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "WindowTracker.h"
#include <meazure/graphics/Graphic.h>
#include <meazure/utils/XlibUtils.h>
#include <meazure/utils/XRecordUtils.h>
#include <X11/Xproto.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>


WindowTracker::WindowTracker(QObject *parent) : QThread(parent) { // NOLINT(cppcoreguidelines-pro-type-member-init)
}

WindowTracker::~WindowTracker() {
    stop();
}

void WindowTracker::start() {
    // Create the pipe that is used to unblock the select to stop the tracking thread.
    if (pipe2(m_stopFd, O_NONBLOCK) == -1) {
        qCritical("Could not create pipe: %s", strerror(errno));    // NOLINT(concurrency-mt-unsafe)
        return;
    }

    // Indicate that the tracking thread should loop.
    m_run = true;

    // Start the tracking thread.
    QThread::start();
}

void WindowTracker::stop() {
    if (!m_run) {
        return;
    }

    // Indicate that the tracking thread should stop looping.
    m_run = false;

    // If the tracking thread is blocked in select, writing to the pipe will unblock it. Though it is virtually
    // impossible for the pipe to be full (typically 4096 byte capacity), ignore EAGAIN anyway.
    if (write(m_stopFd[1], "q", 1) == -1 && errno != EAGAIN) {
        qWarning("Could not write to stop pipe: %s", strerror(errno));    // NOLINT(concurrency-mt-unsafe)
    }

    // Wait for the tracking thread to complete.
    wait();

    // Close the pipe used to unblock the tracking thread.
    close(m_stopFd[0]);
    close(m_stopFd[1]);
}

void WindowTracker::run() {
    // Two display connections are recommended by the XRecord spec
    // (https://www.x.org/releases/X11R7.6/doc/libXtst/recordlib.html#record_clients). The spec also indicates which
    // display connections should be specified in a given XRecord function call. For example, the control display
    // should be used in the call to XRecordCreateContext.
    Xlib::Display controlDisplay;
    Xlib::Display dataDisplay;

    // Set the control display to sync mode to prevent a fatal X error when the data display context is enabled.
    XSynchronize(controlDisplay, True);

    if (!XRecord::hasExtension(controlDisplay)) {
        qFatal("XRecord extension not supported on this X Server");
    }

    XRecord::Range range;
    range.setDeliveredEvent(ConfigureNotify);

    const XRecord::Context context(controlDisplay, dataDisplay, 0, XRecordAllClients, 1, range, 1);

    auto callback = [](XPointer priv, XRecordInterceptData *hook) {
        auto* instance = reinterpret_cast<WindowTracker*>(priv);

        if (hook->category == XRecordFromServer) {
            // The data is formatted as an X Protocol event defined in Xproto.h. For a ConfigureNotify
            // event, the data is formatted according to the configureNotify structure in the xEvent
            // union.
            auto* configureEvent = reinterpret_cast<xEvent*>(hook->data);
            if (configureEvent->u.u.type == ConfigureNotify) {
                const Window win = configureEvent->u.configureNotify.window;
                if (!Graphic::isGraphicWindow(win)) {
                    instance->handleChange(win,
                                           configureEvent->u.configureNotify.x,
                                           configureEvent->u.configureNotify.y,
                                           configureEvent->u.configureNotify.width,
                                           configureEvent->u.configureNotify.height);
                }
            }
        }

        XRecordFreeData(hook);
    };

    context.enableAsync(callback, reinterpret_cast<XPointer>(this));

    const int displayFd = ConnectionNumber(dataDisplay.display());

    const int numFds = std::max(displayFd, m_stopFd[0]) + 1;
    fd_set fds;

    while (m_run) {
        // The select system call is used to avoid spinning on XRecordProcessReplies, which would needlessly
        // load the processor. The stop pipe is used to unblock the select so that the state of the run flag
        // can be checked and the loop can be terminated when stop() is called. This technique for unblocking
        // the select is called the "self-pipe trick". See http://cr.yp.to/docs/selfpipe.html for the description
        // of this technique created by D. J. Bernstein.
        FD_ZERO(&fds);
        FD_SET(displayFd, &fds);
        FD_SET(m_stopFd[0], &fds);

        select(numFds, &fds, nullptr, nullptr, nullptr);
        if (FD_ISSET(displayFd, &fds)) {
            context.processReplies();
        }
    }
}

void WindowTracker::handleChange(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height) {
    emit windowChanged(windowId, x, y, width, height);
}
