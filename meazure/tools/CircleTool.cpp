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

#include "CircleTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <QPointF>
#include <QSizeF>

CircleTool::CircleTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                       QObject* parent) :
        RadioTool(screenInfoProvider, unitsProvider, parent),
        m_center(screenInfoProvider.getCenter()),
        m_perimeter(screenInfoProvider.getCenter() + QPoint(30, 30)),
        m_centerCH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Vertex"), k_centerId)),
        m_perimeterCH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 1"), k_perimeterId)),
        m_circle(new Circle(screenInfoProvider, unitsProvider, k_crosshairRadialOffset)),
        m_line(new Line(screenInfoProvider, unitsProvider, k_crosshairOffset)),
        m_dataWinCenter(new ToolDataWindow(screenInfoProvider, unitsProvider, XYVReadOnly | DistReadOnly)),
        m_dataWinPerimeter(new ToolDataWindow(screenInfoProvider, unitsProvider, XY1ReadOnly | DistReadOnly)) {
    connect(m_centerCH, &CrossHair::entered, this, &CircleTool::entered);
    connect(m_perimeterCH, &CrossHair::entered, this, &CircleTool::entered);

    connect(m_centerCH, &CrossHair::departed, this, &CircleTool::departed);
    connect(m_perimeterCH, &CrossHair::departed, this, &CircleTool::departed);

    connect(m_centerCH, &CrossHair::dragged, this, &CircleTool::dragged);
    connect(m_perimeterCH, &CrossHair::dragged, this, &CircleTool::dragged);

    connect(m_centerCH, &CrossHair::moved, this, &CircleTool::moved);
    connect(m_perimeterCH, &CrossHair::moved, this, &CircleTool::moved);
}

CircleTool::~CircleTool() {
    m_dataWinCenter->hide();
    m_dataWinPerimeter->hide();
    setEnabled(false);
    delete m_dataWinCenter;
    delete m_dataWinPerimeter;
    delete m_centerCH;
    delete m_perimeterCH;
    delete m_circle;
    delete m_line;
}

void CircleTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_centerCH->show();
        m_perimeterCH->show();
        m_circle->show();
        m_line->show();
    } else {
        m_centerCH->hide();
        m_perimeterCH->hide();
        m_circle->hide();
        m_line->hide();
    }
}

void CircleTool::saveProfile(Profile& profile) const {
    // Save the position of each end point.
    //
    const QPointF posCenter = getUnitsProvider().convertPos(m_center);
    profile.writeStr("CircleXV", StringUtils::dblToStr(posCenter.x()));
    profile.writeStr("CircleYV", StringUtils::dblToStr(posCenter.y()));

    const QPointF posPerimeter = getUnitsProvider().convertPos(m_perimeter);
    profile.writeStr("CircleX1", StringUtils::dblToStr(posPerimeter.x()));
    profile.writeStr("CircleY1", StringUtils::dblToStr(posPerimeter.y()));
}

void CircleTool::loadProfile(Profile& profile) {
    // Use the current positions as the default values for those positions that are not specified in the profile.
    //
    const QPointF defaultCenter = getUnitsProvider().convertPos(m_center);
    const QPointF defaultPerimeter = getUnitsProvider().convertPos(m_perimeter);

    // Load the end point positions.
    //
    QPointF posPerimeter;
    posPerimeter.rx() = profile.readDbl("CircleX1", defaultCenter.x());
    posPerimeter.ry() = profile.readDbl("CircleY1", defaultCenter.y());
    m_perimeter = getUnitsProvider().unconvertPos(posPerimeter);

    QPointF posCenter;
    posCenter.rx() = profile.readDbl("CircleXV", defaultPerimeter.x());
    posCenter.ry() = profile.readDbl("CircleYV", defaultPerimeter.y());
    m_center = getUnitsProvider().unconvertPos(posCenter);

    setPosition();
}

void CircleTool::setX1Position(double x) {
    m_perimeter.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_perimeterCH, x));
    setPosition();
}

void CircleTool::setY1Position(double y) {
    m_perimeter.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_perimeterCH, y));
    setPosition();
}

void CircleTool::setXVPosition(double x) {
    m_center.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_centerCH, x));
    setPosition();
}

void CircleTool::setYVPosition(double y) {
    m_center.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_centerCH, y));
    setPosition();
}

void CircleTool::stepX1Position(int numSteps) {
    m_perimeter.rx() += numSteps;
    setPosition();
}

void CircleTool::stepY1Position(int numSteps) {
    m_perimeter.ry() += numSteps;
    setPosition();
}

void CircleTool::stepXVPosition(int numSteps) {
    m_center.rx() += numSteps;
    setPosition();
}

void CircleTool::stepYVPosition(int numSteps) {
    m_center.ry() += numSteps;
    setPosition();
}

void CircleTool::setPosition() {
    m_perimeter = getScreenInfo().constrainPosition(m_perimeter);
    m_center = getScreenInfo().constrainPosition(m_center);

    m_perimeterCH->setPosition(m_perimeter);
    m_centerCH->setPosition(m_center);
    m_circle->setPosition(m_center, m_perimeter);
    m_line->setPosition(m_center, m_perimeter);
}

void CircleTool::refresh() {
    setPosition();
}

void CircleTool::flash() {
    m_perimeterCH->flash();
    m_centerCH->flash();
}

void CircleTool::strobe() {
    m_perimeterCH->strobe();
    m_centerCH->strobe();
    m_dataWinPerimeter->strobe();
    m_dataWinCenter->strobe();
}

void CircleTool::entered(CrossHair&, int id, QPoint, Qt::KeyboardModifiers) {
    if (id == k_perimeterId) {
        m_dataWinPerimeter->show();
    } else {
        m_dataWinCenter->show();
    }
}

void CircleTool::departed(CrossHair&, int id) {
    if (id == k_perimeterId) {
        m_dataWinPerimeter->hide();
    } else {
        m_dataWinCenter->hide();
    }
}

void CircleTool::dragged(CrossHair&, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers) {
    // Ctrl+drag moves all the crosshairs as a single unit
    //
    if ((keyboardModifiers & Qt::ControlModifier) != 0) {
        QPoint movingDelta;
        QPoint followingPos;

        if (id == k_centerId) {
            movingDelta = crosshairCenter - m_center;
            followingPos = m_perimeter + movingDelta;
        } else {
            movingDelta = crosshairCenter - m_perimeter;
            followingPos = m_center + movingDelta;
        }

        movingDelta -= followingPos - getScreenInfo().constrainPosition(followingPos);
        m_center += movingDelta;
        m_perimeter += movingDelta;
    } else {
        QPoint* curPos = (id == k_centerId) ? &m_center : &m_perimeter;
        *curPos = crosshairCenter;

        // Shift + drag locks the movement of the crosshair to vertical or horizontal.
        //
        if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
            const QPoint& fixedPoint = (id == k_centerId) ? m_perimeter : m_center;

            if (Geometry::isVerticallyOriented(m_center, m_perimeter)) {
                curPos->rx() = fixedPoint.x();
            } else {
                curPos->ry() = fixedPoint.y();
            }
        }
    }

    setPosition();
}

void CircleTool::moved(CrossHair&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    const QPointF coordPerimeter = getUnitsProvider().convertCoord(m_perimeter);
    const QPointF coordCenter = getUnitsProvider().convertCoord(m_center);
    const double radius = Geometry::hypot(coordPerimeter, coordCenter);
    const double diameter = 2.0 * radius;
    const QSizeF wh = QSizeF(diameter, diameter);
    const double aspect = Geometry::aspectRatio(wh);
    const double area = Geometry::area(radius);
    const double angle = getUnitsProvider().convertAngle(Geometry::angle(coordPerimeter, coordCenter));

    m_dataWinPerimeter->distanceChanged(radius);
    m_dataWinCenter->distanceChanged(radius);
    if (id == k_perimeterId) {
        m_dataWinPerimeter->xy1PositionChanged(coordPerimeter, m_perimeter);
        m_dataWinPerimeter->moveNear(m_perimeterCH->geometry());

        emit xy1PositionChanged(coordPerimeter, m_perimeter);
    } else {
        m_dataWinCenter->xyvPositionChanged(coordCenter, m_center);
        m_dataWinCenter->moveNear(m_centerCH->geometry());

        emit xyvPositionChanged(coordCenter, m_center);
    }

    emit widthHeightChanged(wh);
    emit distanceChanged(radius);
    emit areaChanged(area);
    emit aspectChanged(aspect);
    emit angleChanged(angle);
}
