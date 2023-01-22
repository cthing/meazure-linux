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

#include "RectangleTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <meazure/utils/Cloaker.h>
#include <QPointF>
#include <QSizeF>

RectangleTool::RectangleTool(const ScreenInfo& screenInfo, const UnitsProvider& unitsProvider, QObject* parent) :
        RadioTool(screenInfo, unitsProvider, parent),
        m_point1(screenInfo.getCenter() - QPoint(30, 30)),
        m_point2(screenInfo.getCenter() + QPoint(30, 30)),
        m_anchorPoint1(m_point1),
        m_anchorPoint2(m_point2),
        m_point1CH(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Point 1"), k_point1Id)),
        m_point2CH(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Point 2"), k_point2Id)),
        m_rectangle(new Rectangle(screenInfo, unitsProvider, k_crosshairOffset)),
        m_dataWin1(new ToolDataWindow(screenInfo, unitsProvider, XY1ReadOnly | DistReadOnly)),
        m_dataWin2(new ToolDataWindow(screenInfo, unitsProvider, XY2ReadOnly | DistReadOnly)) {
    connect(m_point1CH, &Crosshair::entered, this, &RectangleTool::entered);
    connect(m_point2CH, &Crosshair::entered, this, &RectangleTool::entered);

    connect(m_point1CH, &Crosshair::departed, this, &RectangleTool::departed);
    connect(m_point2CH, &Crosshair::departed, this, &RectangleTool::departed);

    connect(m_point1CH, &Crosshair::dragged, this, &RectangleTool::dragged);
    connect(m_point2CH, &Crosshair::dragged, this, &RectangleTool::dragged);

    connect(m_point1CH, &Crosshair::moved, this, &RectangleTool::moved);
    connect(m_point2CH, &Crosshair::moved, this, &RectangleTool::moved);
}

RectangleTool::~RectangleTool() {
    setEnabled(false);
    delete m_dataWin1;
    delete m_dataWin2;
    delete m_point1CH;
    delete m_point2CH;
    delete m_rectangle;
}

void RectangleTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_point1CH->show();
        m_point2CH->show();
        m_rectangle->show();
    } else {
        m_point1CH->hide();
        m_point2CH->hide();
        m_rectangle->hide();
        m_dataWin1->hide();
        m_dataWin2->hide();
    }
}

void RectangleTool::setCrosshairsEnabled(bool enable) {
    if (enable) {
        m_rectangle->setOffset(k_crosshairOffset);
        m_point1CH->show();
        m_point2CH->show();
    } else {
        m_rectangle->setOffset(0.0);
        m_point1CH->hide();
        m_point2CH->hide();
    }
}

void RectangleTool::setDataWinEnabled(bool enable) {
    RadioTool::setDataWinEnabled(enable);

    if (!enable) {
        m_dataWin1->hide();
        m_dataWin2->hide();
    }
}

QImage RectangleTool::grabRegion() const {
    const Cloaker cloak(m_point1CH, m_point2CH, m_rectangle, m_dataWin1, m_dataWin2);

    const QRect regionRect = m_rectangle->geometry();
    return m_screenInfo.grabScreen(regionRect.x(), regionRect.y(), regionRect.width(), regionRect.height());
}

void RectangleTool::writeConfig(Config& config) const {
    // Save the position of each end point.
    //
    const QPointF pos1 = m_unitsProvider.convertPos(m_point1);
    config.writeStr("RectX1", StringUtils::dblToStr(pos1.x()));
    config.writeStr("RectY1", StringUtils::dblToStr(pos1.y()));

    const QPointF pos2 = m_unitsProvider.convertPos(m_point2);
    config.writeStr("RectX2", StringUtils::dblToStr(pos2.x()));
    config.writeStr("RectY2", StringUtils::dblToStr(pos2.y()));
}

void RectangleTool::readConfig(const Config& config) {
    // Use the current positions as the default values for those positions that are not specified in the configuration.
    //
    const QPointF defaultPos1 = m_unitsProvider.convertPos(m_point1);
    const QPointF defaultPos2 = m_unitsProvider.convertPos(m_point2);

    // Load the end point positions.
    //
    QPointF pos1;
    pos1.rx() = config.readDbl("RectX1", defaultPos1.x());
    pos1.ry() = config.readDbl("RectY1", defaultPos1.y());
    m_point1 = m_unitsProvider.unconvertPos(pos1);

    QPointF pos2;
    pos2.rx() = config.readDbl("RectX2", defaultPos2.x());
    pos2.ry() = config.readDbl("RectY2", defaultPos2.y());
    m_point2 = m_unitsProvider.unconvertPos(pos2);

    m_anchorPoint1 = m_point1;
    m_anchorPoint2 = m_point2;

    setPosition();
}

void RectangleTool::setXY1Position(const QPointF& position) {
    m_activePointId = k_point1Id;
    m_point1 = m_unitsProvider.unconvertCoord(position);
    setPosition();
}

void RectangleTool::setX1Position(double x) {
    m_activePointId = k_point1Id;
    m_point1.rx() = qRound(m_unitsProvider.unconvertCoord(ConvertX, m_point1CH, x));
    setPosition();
}

void RectangleTool::setY1Position(double y) {
    m_activePointId = k_point1Id;
    m_point1.ry() = qRound(m_unitsProvider.unconvertCoord(ConvertY, m_point1CH, y));
    setPosition();
}

void RectangleTool::setXY2Position(const QPointF& position) {
    m_activePointId = k_point2Id;
    m_point2 = m_unitsProvider.unconvertCoord(position);
    setPosition();
}

void RectangleTool::setX2Position(double x) {
    m_activePointId = k_point2Id;
    m_point2.rx() = qRound(m_unitsProvider.unconvertCoord(ConvertX, m_point2CH, x));
    setPosition();
}

void RectangleTool::setY2Position(double y) {
    m_activePointId = k_point2Id;
    m_point2.ry() = qRound(m_unitsProvider.unconvertCoord(ConvertY, m_point2CH, y));
    setPosition();
}

void RectangleTool::stepX1Position(int numSteps) {
    m_activePointId = k_point1Id;
    m_point1.rx() += numSteps;
    setPosition();
}

void RectangleTool::stepY1Position(int numSteps) {
    m_activePointId = k_point1Id;
    m_point1.ry() += numSteps;
    setPosition();
}

void RectangleTool::stepX2Position(int numSteps) {
    m_activePointId = k_point2Id;
    m_point2.rx() += numSteps;
    setPosition();
}

void RectangleTool::stepY2Position(int numSteps) {
    m_activePointId = k_point2Id;
    m_point2.ry() += numSteps;
    setPosition();
}

void RectangleTool::setPosition() {
    m_point1 = m_screenInfo.constrainPosition(m_point1);
    m_point2 = m_screenInfo.constrainPosition(m_point2);

    m_point1CH->setPosition(m_point1);
    m_point2CH->setPosition(m_point2);
    m_rectangle->setPosition(m_point1, m_point2);
}

void RectangleTool::refresh() {
    setPosition();

    emitActivePosition();
}

void RectangleTool::flash() {
    m_point1CH->flash();
    m_point2CH->flash();
}

void RectangleTool::strobe() {
    m_point1CH->strobe();
    m_point2CH->strobe();
    m_dataWin1->strobe();
    m_dataWin2->strobe();
}

void RectangleTool::entered(Crosshair&, int id, QPoint, Qt::KeyboardModifiers) {
    if (isDataWinEnabled()) {
        if (id == k_point1Id) {
            m_dataWin1->show();
        } else {
            m_dataWin2->show();
        }
    }
}

void RectangleTool::departed(Crosshair&, int id) {
    if (id == k_point1Id) {
        m_dataWin1->hide();
    } else {
        m_dataWin2->hide();
    }
}

void RectangleTool::dragged(Crosshair&, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers) {
    m_activePointId = id;

    // Ctrl+drag moves all the crosshairs as a single unit
    //
    if ((keyboardModifiers & Qt::ControlModifier) != 0) {
        QPoint movingDelta;
        QPoint followingPos;

        if (id == k_point1Id) {
            movingDelta = crosshairCenter - m_point1;
            followingPos = m_point2 + movingDelta;
        } else {
            movingDelta = crosshairCenter - m_point2;
            followingPos = m_point1 + movingDelta;
        }

        movingDelta -= followingPos - m_screenInfo.constrainPosition(followingPos);
        m_point1 += movingDelta;
        m_point2 += movingDelta;
    } else {
        QPoint& anchorPoint = (id != k_point1Id) ? m_anchorPoint1 : m_anchorPoint2;
        QPoint* curPos = (id == k_point1Id) ? &m_point1 : &m_point2;
        *curPos = crosshairCenter;

        // Shift + drag locks the movement of the crosshair to vertical or horizontal.
        //
        if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
            if (Geometry::isVerticallyOriented(anchorPoint, *curPos)) {
                curPos->rx() = anchorPoint.x();
            } else {
                curPos->ry() = anchorPoint.y();
            }
        } else {
            anchorPoint = *curPos;
        }
    }

    setPosition();
}

void RectangleTool::moved(Crosshair&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    const QPointF coord1 = m_unitsProvider.convertCoord(m_point1);
    const QPointF coord2 = m_unitsProvider.convertCoord(m_point2);
    const QSizeF wh = m_unitsProvider.getWidthHeight(m_point1, m_point2);
    const double distance = Geometry::hypot(wh);
    const double aspect = Geometry::aspectRatio(wh);
    const double area = Geometry::area(wh);
    const double angle = m_unitsProvider.convertAngle(Geometry::angle(coord1, coord2));

    m_dataWin1->distanceChanged(distance);
    m_dataWin2->distanceChanged(distance);
    if (id == k_point1Id) {
        m_dataWin1->xy1PositionChanged(coord1, m_point1);
        m_dataWin1->moveNear(m_point1CH->geometry());

        emit xy1PositionChanged(coord1, m_point1);
    } else {
        m_dataWin2->xy2PositionChanged(coord2, m_point2);
        m_dataWin2->moveNear(m_point2CH->geometry());

        emit xy2PositionChanged(coord2, m_point2);
    }

    emitActivePosition();

    emit widthHeightChanged(wh);
    emit distanceChanged(distance);
    emit areaChanged(area);
    emit aspectChanged(aspect);
    emit angleChanged(angle);
}

void RectangleTool::emitActivePosition() {
    if (m_activePointId == k_point1Id) {
        emit activePositionChanged(m_point1);
    } else {
        emit activePositionChanged(m_point2);
    }
}
