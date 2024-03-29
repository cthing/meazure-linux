/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "PointTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <QPointF>

PointTool::PointTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject *parent) :
        RadioTool(screenInfo, unitsProvider, parent),
        m_center(screenInfo->getCenter()),
        m_anchorPoint(m_center),
        m_crosshair(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Point 1"))),
        m_dataWindow(new ToolDataWindow(screenInfo, unitsProvider, XY1ReadOnly)) {
    connect(m_crosshair, &Crosshair::entered, this, &PointTool::entered);
    connect(m_crosshair, &Crosshair::departed, this, &PointTool::departed);
    connect(m_crosshair, &Crosshair::dragged, this, &PointTool::dragged);
    connect(m_crosshair, &Crosshair::moved, this, &PointTool::moved);
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

void PointTool::setCrosshairsEnabled(bool enable) {
    if (enable) {
        m_crosshair->show();
    } else {
        m_crosshair->hide();
    }
}

void PointTool::setDataWinEnabled(bool enable) {
    RadioTool::setDataWinEnabled(enable);

    if (!enable) {
        m_dataWindow->hide();
    }
}

void PointTool::writeConfig(Config& config) const {
    // Save the position of the crosshair.
    //
    const QPointF pos = m_unitsProvider->convertPos(m_center);
    config.writeStr("PointX1", StringUtils::dblToStr(pos.x()));
    config.writeStr("PointY1", StringUtils::dblToStr(pos.y()));
}

void PointTool::readConfig(const Config& config) {
    // Use the current position as the default value for those position components that are not
    // specified in the configuration.
    //
    const QPointF defaultPos = m_unitsProvider->convertPos(m_center);

    // Load the crosshair position.
    //
    QPointF pos;
    pos.rx() = config.readDbl("PointX1", defaultPos.x());
    pos.ry() = config.readDbl("PointY1", defaultPos.y());
    m_center = m_unitsProvider->unconvertPos(pos);
    m_anchorPoint = m_center;

    setPosition();
}

void PointTool::setXY1Position(const QPointF& position) {
    m_center = m_unitsProvider->unconvertCoord(position);
    setPosition();
}

void PointTool::setX1Position(double x) {
    m_center.rx() = qRound(m_unitsProvider->unconvertCoord(ConvertX, m_crosshair, x));
    setPosition();
}

void PointTool::setY1Position(double y) {
    m_center.ry() = qRound(m_unitsProvider->unconvertCoord(ConvertY, m_crosshair, y));
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
    m_center = m_screenInfo->constrainPosition(m_center);
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

void PointTool::entered(Crosshair&, int, QPoint, Qt::KeyboardModifiers) {
    if (isDataWinEnabled()) {
        m_dataWindow->show();
    }
}

void PointTool::departed(Crosshair&, int) {
    m_dataWindow->hide();
}

void PointTool::dragged(Crosshair&, int, QPoint center, Qt::KeyboardModifiers keyboardModifiers) {
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

void PointTool::moved(Crosshair&, int, QPoint center) {
    if (isEnabled()) {
        const QPointF coord = m_unitsProvider->convertCoord(center);

        m_dataWindow->xy1PositionChanged(coord, center);
        m_dataWindow->moveNear(m_crosshair->geometry());

        emit activePositionChanged(center);
        emit xy1PositionChanged(coord, center);
    }
}
