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

#pragma once

#include <QThread>
#include <cstdint>


/// Trackes changes to the position and size of all windows on the screen. This is accomplished using the XRecord
/// extension to capture configure notify events. The code is based on examples at
/// https://github.com/nibrahim/showkeys/blob/master/tests/record-example.c and
/// https://github.com/KivApple/qvkbd/blob/master/src/x11support.cpp. Note that Meazure graphics windows (e.g. the
/// crosshair) are not tracked.
///
class WindowTracker : public QThread {

    Q_OBJECT

public:
    explicit WindowTracker(QObject* parent = nullptr);
    ~WindowTracker() override;

    WindowTracker(const WindowTracker&) = delete;
    WindowTracker(WindowTracker&&) = delete;
    WindowTracker& operator=(const WindowTracker&) = delete;

    /// Starts tracking window configuration changes and emitting window changed signals.
    ///
    void start();

    /// Stops tracking window configuration changes.
    ///
    void stop();

signals:
    /// Emitted when a window has changed its location, size or visibility.
    ///
    /// @param windowId X identifier of the window that has changed
    /// @param x Location of the left side of the window, in pixels
    /// @param y Location of the top of the window, in pixels
    /// @param width Width of the window, in pixels
    /// @param height Height of the window, in pixels
    ///
    void windowChanged(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height);

protected:
    void run() override;

private:
    void handleChange(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height);

    volatile bool m_run { false };
};
