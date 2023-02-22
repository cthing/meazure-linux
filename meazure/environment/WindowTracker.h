/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <QObject>
#include <cstdint>


/// Tracks changes to the position and size of all windows on the screen.
///
struct WindowTracker {

    WindowTracker() = default;
    virtual ~WindowTracker() = default;

    WindowTracker(const WindowTracker&) = delete;
    WindowTracker(WindowTracker&&) = delete;
    WindowTracker& operator=(const WindowTracker&) = delete;

    /// Indicates whether window tracking is supported on the implementing platform.
    ///
    /// @return true if window tracking is supported.
    ///
    [[nodiscard]] virtual bool isSupported() const = 0;

    /// Starts tracking window configuration changes and emitting window changed signals.
    ///
    virtual void start() = 0;

    /// Stops tracking window configuration changes.
    ///
    virtual void stop() = 0;

signals:
    /// Emitted when a window has changed its location, size or visibility.
    ///
    /// @param windowId X identifier of the window that has changed
    /// @param x Location of the left side of the window, in pixels
    /// @param y Location of the top of the window, in pixels
    /// @param width Width of the window, in pixels
    /// @param height Height of the window, in pixels
    ///
    virtual void windowChanged(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height) = 0;
};

Q_DECLARE_INTERFACE(WindowTracker, "WindowTracker")
