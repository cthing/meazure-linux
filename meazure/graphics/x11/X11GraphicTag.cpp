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

#include "X11GraphicTag.h"
#include <meazure/utils/x11/XlibUtils.h>
#include <X11/Xatom.h>


static Atom MEA_GFX = None;


static void ensureIntern() {
    if (MEA_GFX == None) {
        MEA_GFX = XInternAtom(Xlib::qtDisplay(), "MEA_GFX", False);
    }
}

bool X11GraphicTag::isGraphicWindow(unsigned long windowId) {
    if (windowId == None) {
        return false;
    }

    ensureIntern();

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

void X11GraphicTag::processEvents(const QWidget* target, const QEvent* event) {
    if (event->type() == QEvent::WinIdChange) {
        if (target->parent() == nullptr) {
            const auto win = static_cast<Window>(target->effectiveWinId());
            if (win != None) {
                ensureIntern();

                const uint8_t value = 1;
                XChangeProperty(Xlib::qtDisplay(), win, MEA_GFX, XA_CARDINAL, 8, PropModeReplace, &value, 1);
            }
        }
    }
}