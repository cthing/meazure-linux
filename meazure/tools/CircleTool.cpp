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

#include "CircleTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <meazure/utils/Cloaker.h>
#include <QPointF>
#include <QSizeF>

CircleTool::CircleTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent) :
        RadioTool(screenInfo, unitsProvider, parent),
        m_center(screenInfo->getCenter()),
        m_perimeter(screenInfo->getCenter() + QPoint(30, 30)),
        m_centerCH(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Vertex"), k_centerId)),
        m_perimeterCH(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Point 1"), k_perimeterId)),
        m_circle(new Circle(screenInfo, unitsProvider, k_enabledCrosshairRadialOffset)),
        m_line(new Line(screenInfo, unitsProvider, k_enabledCrosshairOffset)),
        m_dataWinCenter(new ToolDataWindow(screenInfo, unitsProvider, XYVReadOnly | DistReadOnly)),
        m_dataWinPerimeter(new ToolDataWindow(screenInfo, unitsProvider, XY1ReadOnly | DistReadOnly)) {
    connect(m_centerCH, &Crosshair::entered, this, &CircleTool::entered);
    connect(m_perimeterCH, &Crosshair::entered, this, &CircleTool::entered);

    connect(m_centerCH, &Crosshair::departed, this, &CircleTool::departed);
    connect(m_perimeterCH, &Crosshair::departed, this, &CircleTool::departed);

    connect(m_centerCH, &Crosshair::dragged, this, &CircleTool::dragged);
    connect(m_perimeterCH, &Crosshair::dragged, this, &CircleTool::dragged);

    connect(m_centerCH, &Crosshair::moved, this, &CircleTool::moved);
    connect(m_perimeterCH, &Crosshair::moved, this, &CircleTool::moved);
}

CircleTool::~CircleTool() {
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
        m_dataWinCenter->hide();
        m_dataWinPerimeter->hide();
    }
}

void CircleTool::setCrosshairsEnabled(bool enable) {
    if (enable) {
        m_circle->setGap(k_enabledCrosshairRadialOffset);
        m_line->setOffset(k_enabledCrosshairOffset);
        m_centerCH->show();
        m_perimeterCH->show();
    } else {
        m_circle->setGap(k_disabledCrosshairRadialOffset);
        m_line->setOffset(k_disabledCrosshairOffset);
        m_centerCH->hide();
        m_perimeterCH->hide();
    }
}

void CircleTool::setDataWinEnabled(bool enable) {
    RadioTool::setDataWinEnabled(enable);

    if (!enable) {
        m_dataWinCenter->hide();
        m_dataWinPerimeter->hide();
    }
}

QImage CircleTool::grabRegion() const {
    const Cloaker cloak(m_centerCH, m_perimeterCH, m_circle, m_line, m_dataWinCenter, m_dataWinPerimeter);

    const QRect regionRect = m_circle->geometry().normalized();
    return m_screenInfo->grabScreen(regionRect.x(), regionRect.y(), regionRect.width(), regionRect.height());
}

void CircleTool::writeConfig(Config& config) const {
    // Save the position of each end point.
    //
    const QPointF posCenter = m_unitsProvider->convertPos(m_center);
    config.writeStr("CircleXV", StringUtils::dblToStr(posCenter.x()));
    config.writeStr("CircleYV", StringUtils::dblToStr(posCenter.y()));

    const QPointF posPerimeter = m_unitsProvider->convertPos(m_perimeter);
    config.writeStr("CircleX1", StringUtils::dblToStr(posPerimeter.x()));
    config.writeStr("CircleY1", StringUtils::dblToStr(posPerimeter.y()));
}

void CircleTool::readConfig(const Config& config) {
    // Use the current positions as the default values for those positions that are not specified in the configuration.
    //
    const QPointF defaultCenter = m_unitsProvider->convertPos(m_center);
    const QPointF defaultPerimeter = m_unitsProvider->convertPos(m_perimeter);

    // Load the end point positions.
    //
    QPointF posPerimeter;
    posPerimeter.rx() = config.readDbl("CircleX1", defaultCenter.x());
    posPerimeter.ry() = config.readDbl("CircleY1", defaultCenter.y());
    m_perimeter = m_unitsProvider->unconvertPos(posPerimeter);

    QPointF posCenter;
    posCenter.rx() = config.readDbl("CircleXV", defaultPerimeter.x());
    posCenter.ry() = config.readDbl("CircleYV", defaultPerimeter.y());
    m_center = m_unitsProvider->unconvertPos(posCenter);

    setPosition();
}

void CircleTool::setXY1Position(const QPointF& position) {
    m_activePointId = k_perimeterId;
    m_perimeter = m_unitsProvider->unconvertCoord(position);
    setPosition();
}

void CircleTool::setX1Position(double x) {
    m_activePointId = k_perimeterId;
    m_perimeter.rx() = qRound(m_unitsProvider->unconvertCoord(ConvertX, m_perimeterCH, x));
    setPosition();
}

void CircleTool::setY1Position(double y) {
    m_activePointId = k_perimeterId;
    m_perimeter.ry() = qRound(m_unitsProvider->unconvertCoord(ConvertY, m_perimeterCH, y));
    setPosition();
}

void CircleTool::setXYVPosition(const QPointF& position) {
    m_activePointId = k_centerId;
    m_center = m_unitsProvider->unconvertCoord(position);
    setPosition();
}

void CircleTool::setXVPosition(double x) {
    m_activePointId = k_centerId;
    m_center.rx() = qRound(m_unitsProvider->unconvertCoord(ConvertX, m_centerCH, x));
    setPosition();
}

void CircleTool::setYVPosition(double y) {
    m_activePointId = k_centerId;
    m_center.ry() = qRound(m_unitsProvider->unconvertCoord(ConvertY, m_centerCH, y));
    setPosition();
}

void CircleTool::stepX1Position(int numSteps) {
    m_activePointId = k_perimeterId;
    m_perimeter.rx() += numSteps;
    setPosition();
}

void CircleTool::stepY1Position(int numSteps) {
    m_activePointId = k_perimeterId;
    m_perimeter.ry() += numSteps;
    setPosition();
}

void CircleTool::stepXVPosition(int numSteps) {
    m_activePointId = k_centerId;
    m_center.rx() += numSteps;
    setPosition();
}

void CircleTool::stepYVPosition(int numSteps) {
    m_activePointId = k_centerId;
    m_center.ry() += numSteps;
    setPosition();
}

void CircleTool::setPosition() {
    m_perimeter = m_screenInfo->constrainPosition(m_perimeter);
    m_center = m_screenInfo->constrainPosition(m_center);

    m_perimeterCH->setPosition(m_perimeter);
    m_centerCH->setPosition(m_center);
    m_circle->setPosition(m_center, m_perimeter);
    m_line->setPosition(m_center, m_perimeter);
}

void CircleTool::refresh() {
    setPosition();

    emitActivePosition();
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

void CircleTool::entered(Crosshair&, int id, QPoint, Qt::KeyboardModifiers) {
    if (isDataWinEnabled()) {
        if (id == k_perimeterId) {
            m_dataWinPerimeter->show();
        } else {
            m_dataWinCenter->show();
        }
    }
}

void CircleTool::departed(Crosshair&, int id) {
    if (id == k_perimeterId) {
        m_dataWinPerimeter->hide();
    } else {
        m_dataWinCenter->hide();
    }
}

void CircleTool::dragged(Crosshair&, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers) {
    m_activePointId = id;

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

        movingDelta -= followingPos - m_screenInfo->constrainPosition(followingPos);
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

void CircleTool::moved(Crosshair&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    const QPointF coordPerimeter = m_unitsProvider->convertCoord(m_perimeter);
    const QPointF coordCenter = m_unitsProvider->convertCoord(m_center);
    const double radius = Geometry::hypot(coordPerimeter, coordCenter);
    const double diameter = 2.0 * radius;
    const QSizeF wh = QSizeF(diameter, diameter);
    const double aspect = Geometry::aspectRatio(wh);
    const double area = Geometry::area(radius);
    const double angle = m_unitsProvider->convertAngle(Geometry::angle(coordCenter, coordPerimeter));

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

    emitActivePosition();

    emit widthHeightChanged(wh);
    emit distanceChanged(radius);
    emit areaChanged(area);
    emit aspectChanged(aspect);
    emit angleChanged(angle);
}

void CircleTool::emitActivePosition() {
    if (m_activePointId == k_perimeterId) {
        emit activePositionChanged(m_perimeter);
    } else {
        emit activePositionChanged(m_center);
    }
}
