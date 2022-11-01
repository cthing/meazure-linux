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

#include <QByteArray>


/// Attempts to detect the desktop environment (i.e. window manager).
///
namespace DesktopDetector {

    /// Attempts to detect the desktop environment.
    ///
    /// @return String identifying the desktop. Typical examples are "GNOME", "KDE", "XFCE". If the desktop cannot
    ///     be determined, the empty string is returned. Note that "ubuntu:GNOME" will be returned as "GNOME".
    ///
    QByteArray detect();

    bool isGnome();
}
