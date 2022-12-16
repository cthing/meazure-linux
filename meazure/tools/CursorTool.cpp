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

#include "CursorTool.h"
#include <QPointF>


CursorTool::CursorTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                       QObject *parent) :
        RadioTool(screenInfoProvider, unitsProvider, parent),
        m_dataWindow(new ToolDataWindow(screenInfoProvider, unitsProvider, XY1ReadOnly)) {
    connect(m_pointerTracker, &PointerTracker::motion, this, &CursorTool::pointerMotion);
}

CursorTool::~CursorTool() {
    setEnabled(false);
    delete m_dataWindow;
}

void CursorTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_pointerTracker->start();
        if (isDataWinEnabled()) {
            m_dataWindow->show();
        }
    } else {
        m_pointerTracker->stop();
        m_dataWindow->hide();
    }
}

void CursorTool::setDataWinEnabled(bool enable) {
    RadioTool::setDataWinEnabled(enable);

    if (!enable) {
        m_dataWindow->hide();
    }
}

void CursorTool::refresh() {
    emitMeasurement(QCursor::pos());
}

void CursorTool::strobe() {
    m_dataWindow->strobe();
}

bool CursorTool::hasCrosshairs() const {
    return false;
}

void CursorTool::pointerMotion(int16_t x, int16_t y) {
    emitMeasurement(QPoint(x, y));
}

void CursorTool::placeDataWin(const QPoint &position) {
    const int screenIndex = getScreenInfo().screenForPoint(position);
    const QSize cursorSize = getScreenInfo().getCursorSize(screenIndex);
    const QRect targetRect(position, cursorSize);
    m_dataWindow->moveNear(targetRect);
}

void CursorTool::emitMeasurement(QPoint position) {
    if (isEnabled()) {
        const QPointF coord = getUnitsProvider().convertCoord(position);

        m_dataWindow->xy1PositionChanged(coord, position);
        placeDataWin(position);

        emit activePositionChanged(position);
        emit xy1PositionChanged(coord, position);
    }
}
