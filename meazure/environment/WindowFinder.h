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

#include <QPoint>
#include <QRect>


/// Finds the geometry of top level windows on all screens.
///
struct WindowFinder {

    WindowFinder() = default;
    virtual ~WindowFinder() = default;

    WindowFinder(const WindowFinder&) = delete;
    WindowFinder(WindowFinder&&) = delete;
    WindowFinder& operator=(const WindowFinder&) = delete;

    /// Indicates whether window finding is supported on the implementing platform.
    ///
    /// @return true if window finding is supported.
    ///
    [[nodiscard]] virtual bool isSupported() const = 0;

    /// Refreshes the internal list of top level windows. Typically, this method should be called whenever there
    /// is a relevant change to the windows on the screen.
    ///
    virtual void refresh() = 0;

    /// Attempts to find geometry of a visible window at the specified position.
    ///
    /// @param[in] position Location at which to find a window
    /// @return Rectangle representing the found window or an empty rectangle if no window is found at the specified
    ///     position. Where possible, the rectangle represents the actual application window and not the border added
    ///     by the window manager. In certain cases, the window manager border is reported, which includes any shadow
    ///     around that border.
    ///
    virtual QRect find(const QPoint& position) = 0;
};
