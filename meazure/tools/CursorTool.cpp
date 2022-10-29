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


CursorTool::CursorTool(const UnitsProvider& unitsProvider, QObject *parent) : RadioTool(unitsProvider, parent) {
    connect(m_pointerTracker, &PointerTracker::motion, this, &CursorTool::pointerMotion);
}

void CursorTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_pointerTracker->start();
    } else {
        m_pointerTracker->stop();
    }
}

void CursorTool::refresh() {
    emitMeasurement(QCursor::pos());
}

void CursorTool::pointerMotion(int16_t x, int16_t y) {
    emitMeasurement(QPoint(x, y));
}

void CursorTool::emitMeasurement(QPoint position) {
    if (isEnabled()) {
        const QPointF coord = getUnitsProvider().convertCoord(position);
        emit xy1PositionChanged(coord);
    }
}
