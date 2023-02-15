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

#include "Graphic.h"
#include "x11/X11GraphicTag.h"
#include <meazure/utils/PlatformUtils.h>
#include <QEvent>


Graphic::Graphic(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QWidget *parent) :
        QWidget(parent),
        m_screenInfo(screenInfo),
        m_unitsProvider(unitsProvider) {
    if (parent == nullptr) {
        setWindowFlag(Qt::X11BypassWindowManagerHint, true);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_AlwaysShowToolTips, true);
    }
}

bool Graphic::isGraphicWindow(unsigned long windowId) {
    if (PlatformUtils::isX11()) {
        return X11GraphicTag::isGraphicWindow(windowId);
    }

    return false;
}

bool Graphic::event(QEvent* ev) {
    if (PlatformUtils::isX11()) {
        X11GraphicTag::processEvents(this, ev);
    }

    return QWidget::event(ev);
}
