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

#include "DesktopDetector.h"


QByteArray DesktopDetector::detect() {
    // KDE, ubuntu:GNOME, GNOME, UNITY, LXDE, MATE, XFCE...
    const QByteArray xdgCurrentDesktop = qgetenv("XDG_CURRENT_DESKTOP").toUpper();
    if (!xdgCurrentDesktop.isEmpty()) {
        return xdgCurrentDesktop.contains("GNOME") ? "GNOME" : xdgCurrentDesktop;
    }

    // Classic fallbacks
    if (!qEnvironmentVariableIsEmpty("KDE_FULL_SESSION")) {
        return "KDE";
    }
    if (!qEnvironmentVariableIsEmpty("GNOME_DESKTOP_SESSION_ID")) {
        return "GNOME";
    }

    // Fallback to checking $DESKTOP_SESSION (unreliable)
    QByteArray desktopSession = qgetenv("DESKTOP_SESSION");

    // This can be a path in /usr/share/xsessions
    const qsizetype slash = desktopSession.lastIndexOf('/');
    if (slash != -1) {
        // try decoding just the basename
        desktopSession = desktopSession.mid(slash + 1);
    }

    return (desktopSession == "gnome" || desktopSession == "xfce" || desktopSession == "kde") ?
        desktopSession.toUpper() : "";
}

bool DesktopDetector::isGnome() {
    return detect() == "GNOME";
}
