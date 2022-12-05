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

#include "AngleTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <QPointF>
#include <QtMath>
#include <cmath>

AngleTool::AngleTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                     QObject* parent) :
        RadioTool(screenInfoProvider, unitsProvider, parent),
        m_vertex(screenInfoProvider.getCenter()),
        m_point1(screenInfoProvider.getCenter() + QPoint(80, -50)),
        m_point2(screenInfoProvider.getCenter() + QPoint(80, 50)),
        m_vertexAnchor(screenInfoProvider.getCenter()),
        m_vertexCH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Vertex"), k_vertexId)),
        m_point1CH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 1"), k_point1Id)),
        m_point2CH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 2"), k_point2Id)),
        m_lineB(new Line(screenInfoProvider, unitsProvider, k_crosshairOffset)),
        m_line1(new Line(screenInfoProvider, unitsProvider, k_crosshairOffset)),
        m_line2(new Line(screenInfoProvider, unitsProvider, k_crosshairOffset)),
        m_dataWinV(new ToolDataWindow(screenInfoProvider, unitsProvider, XYVReadOnly | AngleReadOnly)),
        m_dataWin1(new ToolDataWindow(screenInfoProvider, unitsProvider, XY1ReadOnly | AngleReadOnly)),
        m_dataWin2(new ToolDataWindow(screenInfoProvider, unitsProvider, XY2ReadOnly | AngleReadOnly)) {
    connect(m_point1CH, &CrossHair::entered, this, &AngleTool::entered);
    connect(m_point2CH, &CrossHair::entered, this, &AngleTool::entered);
    connect(m_vertexCH, &CrossHair::entered, this, &AngleTool::entered);

    connect(m_point1CH, &CrossHair::departed, this, &AngleTool::departed);
    connect(m_point2CH, &CrossHair::departed, this, &AngleTool::departed);
    connect(m_vertexCH, &CrossHair::departed, this, &AngleTool::departed);

    connect(m_point1CH, &CrossHair::dragged, this, &AngleTool::dragged);
    connect(m_point2CH, &CrossHair::dragged, this, &AngleTool::dragged);
    connect(m_vertexCH, &CrossHair::dragged, this, &AngleTool::dragged);

    connect(m_point1CH, &CrossHair::moved, this, &AngleTool::moved);
    connect(m_point2CH, &CrossHair::moved, this, &AngleTool::moved);
    connect(m_vertexCH, &CrossHair::moved, this, &AngleTool::moved);
}

AngleTool::~AngleTool() {
    m_dataWin1->hide();
    m_dataWin2->hide();
    m_dataWinV->hide();
    setEnabled(false);
    delete m_dataWin1;
    delete m_dataWin2;
    delete m_dataWinV;
    delete m_point1CH;
    delete m_point2CH;
    delete m_vertexCH;
    delete m_line1;
    delete m_line2;
    delete m_lineB;
}

void AngleTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_point1CH->show();
        m_point2CH->show();
        m_vertexCH->show();
        m_line1->show();
        m_line2->show();
        m_lineB->show();
    } else {
        m_point1CH->hide();
        m_point2CH->hide();
        m_vertexCH->hide();
        m_line1->hide();
        m_line2->hide();
        m_lineB->hide();
    }
}

void AngleTool::saveProfile(Profile& profile) const {
    // Save the position of point 1, point 2 and the vertex.
    //
    const QPointF pos1 = getUnitsProvider().convertPos(m_point1);
    profile.writeStr("AngleX1", StringUtils::dblToStr(pos1.x()));
    profile.writeStr("AngleY1", StringUtils::dblToStr(pos1.y()));

    const QPointF pos2 = getUnitsProvider().convertPos(m_point2);
    profile.writeStr("AngleX2", StringUtils::dblToStr(pos2.x()));
    profile.writeStr("AngleY2", StringUtils::dblToStr(pos2.y()));

    const QPointF posV = getUnitsProvider().convertPos(m_vertex);
    profile.writeStr("AngleXV", StringUtils::dblToStr(posV.x()));
    profile.writeStr("AngleYV", StringUtils::dblToStr(posV.y()));
}

void AngleTool::loadProfile(Profile& profile) {
    // Use the current positions as the default values for those positions that are not specified in the profile.
    //
    const QPointF defaultPos1 = getUnitsProvider().convertPos(m_point1);
    const QPointF defaultPos2 = getUnitsProvider().convertPos(m_point2);
    const QPointF defaultPosV = getUnitsProvider().convertPos(m_vertex);

    // Load the position of point 1, point 2 and the vertex.
    //
    QPointF pos1;
    pos1.rx() = profile.readDbl("AngleX1", defaultPos1.x());
    pos1.ry() = profile.readDbl("AngleY1", defaultPos1.y());
    m_point1 = getUnitsProvider().unconvertPos(pos1);

    QPointF pos2;
    pos2.rx() = profile.readDbl("AngleX2", defaultPos2.x());
    pos2.ry() = profile.readDbl("AngleY2", defaultPos2.y());
    m_point2 = getUnitsProvider().unconvertPos(pos2);

    QPointF posV;
    posV.rx() = profile.readDbl("AngleXV", defaultPosV.x());
    posV.ry() = profile.readDbl("AngleYV", defaultPosV.y());
    m_vertex = getUnitsProvider().unconvertPos(posV);

    m_vertexAnchor = m_vertex;

    setPosition();
}

void AngleTool::setX1Position(double x) {
    m_point1.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_point1CH, x));
    setPosition();
}

void AngleTool::setY1Position(double y) {
    m_point1.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_point1CH, y));
    setPosition();
}

void AngleTool::setX2Position(double x) {
    m_point2.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_point2CH, x));
    setPosition();
}

void AngleTool::setY2Position(double y) {
    m_point2.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_point2CH, y));
    setPosition();
}

void AngleTool::setXVPosition(double x) {
    m_vertex.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_vertexCH, x));
    setPosition();
}

void AngleTool::setYVPosition(double y) {
    m_vertex.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_vertexCH, y));
    setPosition();
}

void AngleTool::stepX1Position(int numSteps) {
    m_point1.rx() += numSteps;
    setPosition();
}

void AngleTool::stepY1Position(int numSteps) {
    m_point1.ry() += numSteps;
    setPosition();
}

void AngleTool::stepX2Position(int numSteps) {
    m_point2.rx() += numSteps;
    setPosition();
}

void AngleTool::stepY2Position(int numSteps) {
    m_point2.ry() += numSteps;
    setPosition();
}

void AngleTool::stepXVPosition(int numSteps) {
    m_vertex.rx() += numSteps;
    setPosition();
}

void AngleTool::stepYVPosition(int numSteps) {
    m_vertex.ry() += numSteps;
    setPosition();
}

void AngleTool::setPosition() {
    m_point1 = getScreenInfo().constrainPosition(m_point1);
    m_point2 = getScreenInfo().constrainPosition(m_point2);
    m_vertex = getScreenInfo().constrainPosition(m_vertex);

    m_point1CH->setPosition(m_point1);
    m_point2CH->setPosition(m_point2);
    m_vertexCH->setPosition(m_vertex);
    m_line1->setPosition(m_vertex, m_point1);
    m_line2->setPosition(m_vertex, m_point2);

    setBisectorPosition();
}

void AngleTool::setBisectorPosition() {
    // Angles are calculated based on the converted positions so that screen resolutions are taken into account.
    //
    const QPointF coord1 = getUnitsProvider().convertCoord(m_point1);
    const QPointF coord2 = getUnitsProvider().convertCoord(m_point2);
    const QPointF coordV = getUnitsProvider().convertCoord(m_vertex);

    // The bisector angle is the average of the angle made by each line relative to the x-axis.
    //
    // Bisector angle = (angle 1 + angle 2) / 2
    //
    double alphaB = (Geometry::angle(coordV, coord1) + Geometry::angle(coordV, coord2)) / 2.0;

    if (getUnitsProvider().isInvertY()) {
        alphaB = -alphaB;
    }

    // Ensure that the bisector is on the acute or obtuse side of the angle.
    //
    QPoint pointB(m_vertex.x() + static_cast<int>(k_lengthB * std::cos(alphaB)),
                  m_vertex.y() + static_cast<int>(k_lengthB * std::sin(alphaB)));
    const QPointF pB = getUnitsProvider().convertCoord(pointB);

    const bool angleSign = Geometry::angle(coordV, coord1, coord2) >= 0.0;
    const bool bisectorSign = Geometry::angle(coordV, coord1, pB) >= 0.0;

    // If we need to flip the bisector, add 180 degrees and recalculate its location.
    //
    if ((angleSign && !bisectorSign) || (!angleSign && bisectorSign)) {
        alphaB = alphaB + M_PI;
        pointB.rx() = m_vertex.x() + static_cast<int>(k_lengthB * std::cos(alphaB));
        pointB.ry() = m_vertex.y() + static_cast<int>(k_lengthB * std::sin(alphaB));
    }

    // Position the bisector line based on the calculated point.
    //
    m_lineB->setPosition(m_vertex, pointB);
}

void AngleTool::refresh() {
    setPosition();
}

void AngleTool::flash() {
    m_point1CH->flash();
    m_point2CH->flash();
    m_vertexCH->flash();
}

void AngleTool::strobe() {
    m_point1CH->strobe();
    m_point2CH->strobe();
    m_vertexCH->strobe();
    m_dataWin1->strobe();
    m_dataWin2->strobe();
    m_dataWinV->strobe();
}

void AngleTool::entered(CrossHair&, int id, QPoint, Qt::KeyboardModifiers) {
    if (id == k_point1Id) {
        m_dataWin1->show();
    } else if (id == k_point2Id) {
        m_dataWin2->show();
    } else {
        m_dataWinV->show();
    }
}

void AngleTool::departed(CrossHair&, int id) {
    if (id == k_point1Id) {
        m_dataWin1->hide();
    } else if (id == k_point2Id) {
        m_dataWin2->hide();
    } else {
        m_dataWinV->hide();
    }
}

void AngleTool::dragged(CrossHair&, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers) {
    // Ctrl + drag moves the all the crosshairs as a single unit.
    //
    if ((keyboardModifiers & Qt::ControlModifier) != 0) {
        QPoint movingDelta;
        QPoint followingPos1;
        QPoint followingPos2;

        if (id == k_point1Id) {
            movingDelta = center - m_point1;
            followingPos1 = m_point2 + movingDelta;
            followingPos2 = m_vertex + movingDelta;
        } else if (id == k_point2Id) {
            movingDelta = center - m_point2;
            followingPos1 = m_point1 + movingDelta;
            followingPos2 = m_vertex + movingDelta;
        } else {
            movingDelta = center - m_vertex;
            followingPos1 = m_point1 + movingDelta;
            followingPos2 = m_point2 + movingDelta;
        }

        const QPoint d1 = followingPos1 - getScreenInfo().constrainPosition(followingPos1);
        const QPoint d2 = followingPos2 - getScreenInfo().constrainPosition(followingPos2);

        QPoint d;
        d.rx() = (std::abs(d1.x()) < std::abs(d2.x())) ? d2.x() : d1.x();
        d.ry() = (std::abs(d1.y()) < std::abs(d2.y())) ? d2.y() : d1.y();

        movingDelta -= d;
        m_point1 += movingDelta;
        m_point2 += movingDelta;
        m_vertex += movingDelta;
    } else {
        if (id == k_point1Id) {
            m_point1 = center;

            // Shift + drag locks the movement of the crosshair to vertical or horizontal.
            //
            if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
                if (Geometry::isVerticallyOriented(m_vertex, m_point1)) {
                    m_point1.rx() = m_vertex.x();
                } else {
                    m_point1.ry() = m_vertex.y();
                }
            }
        } else if (id == k_point2Id) {
            m_point2 = center;

            // Shift + drag locks the movement of the crosshair to vertical or horizontal.
            //
            if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
                if (Geometry::isVerticallyOriented(m_vertex, m_point2)) {
                    m_point2.rx() = m_vertex.x();
                } else {
                    m_point2.ry() = m_vertex.y();
                }
            }
        } else {
            m_vertex = center;

            // Shift + drag locks the movement of the crosshair to vertical or horizontal.
            //
            if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
                if (Geometry::isVerticallyOriented(m_vertexAnchor, m_vertex)) {
                    m_vertex.rx() = m_vertexAnchor.x();
                } else {
                    m_vertex.ry() = m_vertexAnchor.y();
                }
            } else {
                m_vertexAnchor = m_vertex;
            }
        }
    }

    setPosition();
}

void AngleTool::moved(CrossHair&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    const QPointF coord1 = getUnitsProvider().convertCoord(m_point1);
    const QPointF coord2 = getUnitsProvider().convertCoord(m_point2);
    const QPointF coordV = getUnitsProvider().convertCoord(m_vertex);
    const double angle = getUnitsProvider().convertAngle(Geometry::angle(coordV, coord1, coord2));

    m_dataWin1->angleChanged(angle);
    m_dataWin2->angleChanged(angle);
    m_dataWinV->angleChanged(angle);
    if (id == k_point1Id) {
        m_dataWin1->xy1PositionChanged(coord1, m_point1);
        m_dataWin1->moveNear(m_point1CH->geometry());

        emit xy1PositionChanged(coord1, m_point1);
    } else if (id == k_point2Id){
        m_dataWin2->xy2PositionChanged(coord2, m_point2);
        m_dataWin2->moveNear(m_point2CH->geometry());

        emit xy2PositionChanged(coord2, m_point2);
    } else {
        m_dataWinV->xyvPositionChanged(coordV, m_vertex);
        m_dataWinV->moveNear(m_vertexCH->geometry());

        emit xyvPositionChanged(coordV, m_vertex);
    }

    emit angleChanged(angle);
}
