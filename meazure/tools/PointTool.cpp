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
#include <meazure/utils/StringUtils.h>
#include <QPointF>

PointTool::PointTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                     QObject *parent) :
        RadioTool(screenInfoProvider, unitsProvider, parent),
        m_center(screenInfoProvider.getCenter()),
        m_anchorPoint(m_center),
        m_crosshair(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 1"))),
        m_dataWindow(new ToolDataWindow(screenInfoProvider, unitsProvider, XY1ReadOnly)) {
    connect(m_crosshair, &CrossHair::entered, this, &PointTool::entered);
    connect(m_crosshair, &CrossHair::departed, this, &PointTool::departed);
    connect(m_crosshair, &CrossHair::dragged, this, &PointTool::dragged);
    connect(m_crosshair, &CrossHair::moved, this, &PointTool::moved);
}

PointTool::~PointTool() {
    setEnabled(false);
    delete m_dataWindow;
    delete m_crosshair;
}

void PointTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_crosshair->show();
    } else {
        m_crosshair->hide();
        m_dataWindow->hide();
    }
}

void PointTool::saveProfile(Profile& profile) const {
    // Save the position of the crosshair.
    //
    const QPointF pos = getUnitsProvider().convertPos(m_center);
    profile.writeStr("PointX1", StringUtils::dblToStr(pos.x()));
    profile.writeStr("PointY1", StringUtils::dblToStr(pos.y()));
}

void PointTool::loadProfile(Profile& profile) {
    // Use the current position as the default value for those position components that are not
    // specified in the profile.
    //
    const QPointF defaultPos = getUnitsProvider().convertPos(m_center);

    // Load the crosshair position.
    //
    QPointF pos;
    pos.rx() = profile.readDbl("PointX1", defaultPos.x());
    pos.ry() = profile.readDbl("PointY1", defaultPos.y());
    m_center = getUnitsProvider().unconvertPos(pos);
    m_anchorPoint = m_center;

    setPosition();
}

void PointTool::setX1Position(double x) {
    m_center.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_crosshair, x));
    setPosition();
}

void PointTool::setY1Position(double y) {
    m_center.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_crosshair, y));
    setPosition();
}

void PointTool::stepX1Position(int numSteps) {
    m_center.rx() += numSteps;
    setPosition();
}

void PointTool::stepY1Position(int numSteps) {
    m_center.ry() += numSteps;
    setPosition();
}

void PointTool::setPosition() {
    m_center = getScreenInfo().constrainPosition(m_center);
    m_crosshair->setPosition(m_center);
}

void PointTool::refresh() {
    setPosition();

    emit activePositionChanged(m_center);
}

void PointTool::flash() {
    m_crosshair->flash();
}

void PointTool::strobe() {
    m_crosshair->strobe();
    m_dataWindow->strobe();
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

    setPosition();
}

void PointTool::moved(CrossHair&, int, QPoint center) {
    if (isEnabled()) {
        const QPointF coord = getUnitsProvider().convertCoord(center);

        m_dataWindow->xy1PositionChanged(coord, center);
        m_dataWindow->moveNear(m_crosshair->geometry());

        emit activePositionChanged(center);
        emit xy1PositionChanged(coord, center);
    }
}
