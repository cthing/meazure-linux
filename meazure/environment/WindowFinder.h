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

#include <QObject>
#include <QPoint>
#include <QRect>
#include <vector>


class Finder;


/// Finds the geometry of X11 top level client windows on all screens. Windows that are input-only, minimized or are
/// Meazure graphic elements are not included.
///
class WindowFinder {

public:
    WindowFinder();
    ~WindowFinder();

    WindowFinder(const WindowFinder&) = delete;
    WindowFinder(WindowFinder&&) = delete;
    WindowFinder& operator=(const WindowFinder&) = delete;

    /// Refreshes the internal list of top level windows. Windows that are input-only, minimized or are Meazure
    /// graphic elements are not included. Typically, this method should be called whenever there is a relevant change
    /// to the windows on the screen.
    ///
    void refresh();

    /// Attempts to find geometry of a visible window at the specified position.
    ///
    /// @param[in] position Location at which to find a window
    /// @return Rectangle representing the found window or an empty rectangle if no window is found at the specified
    ///     position. Where possible, the rectangle represents the actual application window and not the border added
    ///     by the window manager. In certain cases, the window manager border is reported, which includes any shadow
    ///     around that border.
    ///
    QRect find(const QPoint& position);

private:
    Finder* m_updater;
    bool m_firstUpdate { false };
    std::vector<QRect> m_windows;
};
