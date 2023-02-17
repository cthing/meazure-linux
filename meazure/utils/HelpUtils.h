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

#include "PlatformUtils.h"
#include <QIcon>
#include <QStyle>
#include <QApplication>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>


/// Controls the Qt Assistant viewer to show the application help content.
///
namespace HelpUtils {

    inline QIcon getHelpIcon() {
        const int iconSize = QApplication::style()->pixelMetric(QStyle::PM_SmallIconSize);
        return QIcon::fromTheme("dialog-question").pixmap(iconSize, iconSize);
    }

    inline QString getHelpPathname() {
        const QString helpPath = PlatformUtils::findAppDataDir("help");
        return helpPath + QDir::separator() + "help.html";
    }

    inline bool isHelpPresent() {
        return QFileInfo::exists(getHelpPathname());
    }

    inline void openHelp() {
        if (isHelpPresent()) {
            QDesktopServices::openUrl(QUrl("file://" + getHelpPathname()));
        }
    }
}
