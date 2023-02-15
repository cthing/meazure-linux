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

#include <QGuiApplication>


/// Utility functions for obtaining information about the runtime platform.
///
namespace PlatformUtils {

    /// Indicates if the current window system is Wayland.
    ///
    /// @return true if the current window system is Wayland.
    ///
    inline bool isWayland() {
        return QGuiApplication::platformName().startsWith("wayland");
    }

    /// Indicates if the current window system is X11.
    ///
    /// @return true if the current window system is X11.
    ///
    inline bool isX11() {
        return QGuiApplication::platformName() == "xcb";
    }
}
