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

#include "LineTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <meazure/utils/Cloaker.h>
#include <QPointF>
#include <QSizeF>

LineTool::LineTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                   QObject* parent) :
        RadioTool(screenInfoProvider, unitsProvider, parent),
        m_point1(screenInfoProvider.getCenter() - QPoint(30, 30)),
        m_point2(screenInfoProvider.getCenter() + QPoint(30, 30)),
        m_point1CH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 1"), k_point1Id)),
        m_point2CH(new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 2"), k_point2Id)),
        m_line(new Line(screenInfoProvider, unitsProvider, k_crosshairOffset)),
        m_dataWin1(new ToolDataWindow(screenInfoProvider, unitsProvider, XY1ReadOnly | DistReadOnly)),
        m_dataWin2(new ToolDataWindow(screenInfoProvider, unitsProvider, XY2ReadOnly | DistReadOnly)) {
    connect(m_point1CH, &CrossHair::entered, this, &LineTool::entered);
    connect(m_point2CH, &CrossHair::entered, this, &LineTool::entered);

    connect(m_point1CH, &CrossHair::departed, this, &LineTool::departed);
    connect(m_point2CH, &CrossHair::departed, this, &LineTool::departed);

    connect(m_point1CH, &CrossHair::dragged, this, &LineTool::dragged);
    connect(m_point2CH, &CrossHair::dragged, this, &LineTool::dragged);

    connect(m_point1CH, &CrossHair::moved, this, &LineTool::moved);
    connect(m_point2CH, &CrossHair::moved, this, &LineTool::moved);
}

LineTool::~LineTool() {
    setEnabled(false);
    delete m_dataWin1;
    delete m_dataWin2;
    delete m_point1CH;
    delete m_point2CH;
    delete m_line;
}

void LineTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_point1CH->show();
        m_point2CH->show();
        m_line->show();
    } else {
        m_point1CH->hide();
        m_point2CH->hide();
        m_line->hide();
        m_dataWin1->hide();
        m_dataWin2->hide();
    }
}

QImage LineTool::grabRegion() const {
    const Cloaker cloak(m_point1CH, m_point2CH, m_line, m_dataWin1, m_dataWin2);

    const QRect regionRect(m_point1, m_point2);
    return getScreenInfo().grabScreen(regionRect.x(), regionRect.y(), regionRect.width(), regionRect.height());
}

void LineTool::saveProfile(Profile& profile) const {
    // Save the position of each end point.
    //
    const QPointF pos1 = getUnitsProvider().convertPos(m_point1);
    profile.writeStr("LineX1", StringUtils::dblToStr(pos1.x()));
    profile.writeStr("LineY1", StringUtils::dblToStr(pos1.y()));

    const QPointF pos2 = getUnitsProvider().convertPos(m_point2);
    profile.writeStr("LineX2", StringUtils::dblToStr(pos2.x()));
    profile.writeStr("LineY2", StringUtils::dblToStr(pos2.y()));
}

void LineTool::loadProfile(Profile& profile) {
    // Use the current positions as the default values for those positions that are not specified in the profile.
    //
    const QPointF defaultPos1 = getUnitsProvider().convertPos(m_point1);
    const QPointF defaultPos2 = getUnitsProvider().convertPos(m_point2);

    // Load the end point positions.
    //
    QPointF pos1;
    pos1.rx() = profile.readDbl("LineX1", defaultPos1.x());
    pos1.ry() = profile.readDbl("LineY1", defaultPos1.y());
    m_point1 = getUnitsProvider().unconvertPos(pos1);

    QPointF pos2;
    pos2.rx() = profile.readDbl("LineX2", defaultPos2.x());
    pos2.ry() = profile.readDbl("LineY2", defaultPos2.y());
    m_point2 = getUnitsProvider().unconvertPos(pos2);

    setPosition();
}

void LineTool::setX1Position(double x) {
    m_point1.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_point1CH, x));
    setPosition();
}

void LineTool::setY1Position(double y) {
    m_point1.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_point1CH, y));
    setPosition();
}

void LineTool::setX2Position(double x) {
    m_point2.rx() = qRound(getUnitsProvider().unconvertCoord(ConvertX, m_point2CH, x));
    setPosition();
}

void LineTool::setY2Position(double y) {
    m_point2.ry() = qRound(getUnitsProvider().unconvertCoord(ConvertY, m_point2CH, y));
    setPosition();
}

void LineTool::stepX1Position(int numSteps) {
    m_point1.rx() += numSteps;
    setPosition();
}

void LineTool::stepY1Position(int numSteps) {
    m_point1.ry() += numSteps;
    setPosition();
}

void LineTool::stepX2Position(int numSteps) {
    m_point2.rx() += numSteps;
    setPosition();
}

void LineTool::stepY2Position(int numSteps) {
    m_point2.ry() += numSteps;
    setPosition();
}

void LineTool::setPosition() {
    m_point1 = getScreenInfo().constrainPosition(m_point1);
    m_point2 = getScreenInfo().constrainPosition(m_point2);

    m_point1CH->setPosition(m_point1);
    m_point2CH->setPosition(m_point2);
    m_line->setPosition(m_point1, m_point2);
}

void LineTool::refresh() {
    setPosition();

    emitActivePosition();
}

void LineTool::flash() {
    m_point1CH->flash();
    m_point2CH->flash();
}

void LineTool::strobe() {
    m_point1CH->strobe();
    m_point2CH->strobe();
    m_dataWin1->strobe();
    m_dataWin2->strobe();
}

void LineTool::entered(CrossHair&, int id, QPoint, Qt::KeyboardModifiers) {
    if (id == k_point1Id) {
        m_dataWin1->show();
    } else {
        m_dataWin2->show();
    }
}

void LineTool::departed(CrossHair&, int id) {
    if (id == k_point1Id) {
        m_dataWin1->hide();
    } else {
        m_dataWin2->hide();
    }
}

void LineTool::dragged(CrossHair&, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers) {
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

        movingDelta -= followingPos - getScreenInfo().constrainPosition(followingPos);
        m_point1 += movingDelta;
        m_point2 += movingDelta;
    } else {
        QPoint* curPos = (id == k_point1Id) ? &m_point1 : &m_point2;
        *curPos = crosshairCenter;

        // Shift + drag locks the movement of the crosshair to vertical or horizontal.
        //
        if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
            const QPoint& fixedPoint = (id == k_point1Id) ? m_point2 : m_point1;

            if (Geometry::isVerticallyOriented(m_point1, m_point2)) {
                curPos->rx() = fixedPoint.x();
            } else {
                curPos->ry() = fixedPoint.y();
            }
        }
    }

    setPosition();
}

void LineTool::moved(CrossHair&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    const QPointF coord1 = getUnitsProvider().convertCoord(m_point1);
    const QPointF coord2 = getUnitsProvider().convertCoord(m_point2);
    const QSizeF wh = getUnitsProvider().getWidthHeight(m_point1, m_point2);
    const double distance = Geometry::hypot(wh);
    const double aspect = Geometry::aspectRatio(wh);
    const double area = Geometry::area(wh);
    const double angle = getUnitsProvider().convertAngle(Geometry::angle(coord1, coord2));

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

void LineTool::emitActivePosition() {
    if (m_activePointId == k_point1Id) {
        emit activePositionChanged(m_point1);
    } else {
        emit activePositionChanged(m_point2);
    }
}
