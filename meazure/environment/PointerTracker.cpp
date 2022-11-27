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

#include "PointerTracker.h"
#include <meazure/utils/XlibUtils.h>
#include <meazure/utils/XRecordUtils.h>
#include <X11/Xproto.h>
#include <sys/select.h>


PointerTracker::PointerTracker(QObject *parent) : QThread(parent) {
}

PointerTracker::~PointerTracker() {
    if (isRunning()) {
        stop();
    }
}

void PointerTracker::start() {
    m_run = true;

    QThread::start();
}

void PointerTracker::stop() {
    m_run = false;

    wait();
}

void PointerTracker::run() {
    constexpr long k_selectTimeout = 2;   // Seconds

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
    range.setDeviceEvent(MotionNotify);

    const XRecord::Context context(controlDisplay, dataDisplay, 0, XRecordAllClients, 1, range, 1);

    auto callback = [](XPointer priv, XRecordInterceptData *hook) {
        auto* instance = reinterpret_cast<PointerTracker*>(priv);

        if (hook->category == XRecordFromServer) {
            // The data is formatted as an X Protocol event defined in Xproto.h. For a MotionNotify
            // event, the data is formatted according to the keyButtonPointer structure in the xEvent
            // union.
            auto* motionEvent = reinterpret_cast<xEvent*>(hook->data);
            if (motionEvent->u.u.type == MotionNotify) {
                instance->handleMotion(motionEvent->u.keyButtonPointer.rootX,
                                       motionEvent->u.keyButtonPointer.rootY);
            }
        }

        XRecordFreeData(hook);
    };

    context.enableAsync(callback, reinterpret_cast<XPointer>(this));

    const int displayFd = ConnectionNumber(dataDisplay.display());

    while (m_run) {
        // The select system call is used to avoid spinning on XRecordProcessReplies, which would
        // needlessly load the processor. The timeout is used to avoid blocking on the select so
        // that the state of the run flag can be checked and the loop can be terminated when stop()
        // is called.
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(displayFd, &fds);
        timeval timeout { k_selectTimeout, 0 };
        select(displayFd + 1, &fds, nullptr, nullptr, &timeout);
        if (FD_ISSET(displayFd, &fds)) {
            context.processReplies();
        }
    }
}

void PointerTracker::handleMotion(int16_t x, int16_t y) {
    emit motion(x, y);
}
