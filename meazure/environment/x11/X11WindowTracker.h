/*
 * Copyright 2023 C Thing Software
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

#pragma once

#include <meazure/environment/WindowTracker.h>
#include <QThread>
#include <cstdint>


/// Tracks changes to the position and size of all windows on the screen. This is accomplished using the XRecord
/// extension to capture configure notify events. The code is based on examples at
/// https://github.com/nibrahim/showkeys/blob/master/tests/record-example.c and
/// https://github.com/KivApple/qvkbd/blob/master/src/x11support.cpp. Note that Meazure graphics windows (e.g. the
/// crosshair) are not tracked.
///
class X11WindowTracker : public QThread, public WindowTracker {

    Q_OBJECT
    Q_INTERFACES(WindowTracker)

public:
    explicit X11WindowTracker(QObject* parent = nullptr);
    ~X11WindowTracker() override;

    X11WindowTracker(const X11WindowTracker&) = delete;
    X11WindowTracker(X11WindowTracker&&) = delete;
    X11WindowTracker& operator=(const X11WindowTracker&) = delete;

    [[nodiscard]] bool isSupported() const override;

    void start() override;
    void stop() override;

signals:
    /// Emitted when a window has changed its location, size or visibility.
    ///
    /// @param windowId X identifier of the window that has changed
    /// @param x Location of the left side of the window, in pixels
    /// @param y Location of the top of the window, in pixels
    /// @param width Width of the window, in pixels
    /// @param height Height of the window, in pixels
    ///
    void windowChanged(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height) override;

protected:
    void run() override;

private:
    void handleChange(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height);

    int m_stopFd[2];
    volatile bool m_run { false };
};
