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

#include <QGuiApplication>
#include <QString>
#include <QDir>


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

    /// Finds the specified subdirectory of the application data directory. During development, the build target
    /// directory is treated as the application data directory. When the application is deployed, the application
    /// data directory is the standard directory for the platform (e.g. /usr/share/<appname>).
    ///
    /// @param[in] subdir Subdirectory of the application data directory
    /// @return Subdirectory of the application data directory.
    ///
    inline QString findAppDataDir(const QString& subdir) {
        const QDir appDir(QGuiApplication::applicationDirPath());

        // During development the data directory is in the same directory as the application.
        const QDir appSubdir(appDir.filePath(subdir));
        if (appSubdir.exists()) {
            return appSubdir.filesystemPath().c_str();
        }

        // When deployed, the data directory is in a standard location.
        const QDir shareDir("/usr/share/meazure");
        const QDir shareSubdir(shareDir.filePath(subdir));
        return shareSubdir.filesystemPath().c_str();
    }
}
