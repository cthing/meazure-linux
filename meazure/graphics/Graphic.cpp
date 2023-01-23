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
#include <QEvent>
#include <meazure/utils/XlibUtils.h>
#include <X11/Xatom.h>


static Atom MEA_GFX = None;


Graphic::Graphic(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QWidget *parent) :
        QWidget(parent),
        m_screenInfo(screenInfo),
        m_unitsProvider(unitsProvider) {
    if (parent == nullptr) {
        setWindowFlag(Qt::X11BypassWindowManagerHint, true);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_AlwaysShowToolTips, true);

        if (MEA_GFX == None) {
            MEA_GFX = XInternAtom(Xlib::qtDisplay(), "MEA_GFX", False);
        }
    }
}

bool Graphic::isGraphicWindow(unsigned long windowId) {
    if (windowId == None) {
        return false;
    }

    Atom typeReturn = None;
    int formatReturn = 0;
    unsigned long numItemsReturn = 0;
    unsigned long bytesAfterReturn = 0;
    unsigned char *data = nullptr;
    const int status = XGetWindowProperty(Xlib::qtDisplay(), windowId, MEA_GFX, 0, 1, False, XA_CARDINAL,
                                          &typeReturn, &formatReturn, &numItemsReturn, &bytesAfterReturn, &data);
    if (status != Success || data == nullptr) {
        return false;
    }

    const bool found = *data == 1;
    XFree(data);
    return found;
}

bool Graphic::event(QEvent* ev) {
    if (ev->type() == QEvent::WinIdChange) {
        if (parent() == nullptr) {
            const auto win = static_cast<Window>(effectiveWinId());
            if (win != None) {
                const uint8_t value = 1;
                XChangeProperty(Xlib::qtDisplay(), win, MEA_GFX, XA_CARDINAL, 8, PropModeReplace, &value, 1);
            }
        }
    }

    return QWidget::event(ev);
}
