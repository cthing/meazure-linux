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

#include "PointTool.h"
#include <meazure/utils/Geometry.h>

PointTool::PointTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                     QObject *parent) :
        RadioTool(unitsProvider, parent),
        m_center(screenInfoProvider.getCenter()),
        m_anchorPoint(m_center),
        m_dataWindow(new ToolDataWindow(screenInfoProvider, unitsProvider, XY1ReadOnly)) {
    m_crosshair = new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 1"));
    connect(m_crosshair, &CrossHair::entered, this, &PointTool::entered);
    connect(m_crosshair, &CrossHair::departed, this, &PointTool::departed);
    connect(m_crosshair, &CrossHair::dragged, this, &PointTool::dragged);
    connect(m_crosshair, &CrossHair::moved, this, &PointTool::moved);
}

PointTool::~PointTool() {
    m_dataWindow->hide();
    setEnabled(false);
    delete m_crosshair;
}

void PointTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_crosshair->show();
    } else {
        m_crosshair->hide();
    }
}

void PointTool::flash() {
    m_crosshair->flash();
}

void PointTool::strobe() {
    m_crosshair->strobe();
}

void PointTool::entered(CrossHair&, int, QPoint, Qt::KeyboardModifiers) {
    m_dataWindow->show();
}

void PointTool::departed(CrossHair&, int) {
    m_dataWindow->hide();
}

void PointTool::dragged(CrossHair&, int, QPoint center, Qt::KeyboardModifiers keyboardModifiers) {
    m_center = center;

    // Shift + drag locks the movement of the crosshair to vertical or horizontal.
    if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
        if (Geometry::isVerticallyOriented(m_anchorPoint, m_center)) {
            m_center.rx() = m_anchorPoint.x();
        } else {
            m_center.ry() = m_anchorPoint.y();
        }
    } else {
        m_anchorPoint = m_center;
    }

    m_crosshair->setPosition(m_center);
}

void PointTool::moved(CrossHair&, int, QPoint center) { // NOLINT(readability-convert-member-functions-to-static)
    printf("================================================== %d %d\n", center.x(), center.y());
}
