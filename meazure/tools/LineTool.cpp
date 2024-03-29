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

#include "LineTool.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <meazure/utils/Cloaker.h>
#include <QPointF>
#include <QSizeF>

LineTool::LineTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent) :
        RadioTool(screenInfo, unitsProvider, parent),
        m_point1(screenInfo->getCenter() - QPoint(30, 30)),
        m_point2(screenInfo->getCenter() + QPoint(30, 30)),
        m_point1CH(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Point 1"), k_point1Id)),
        m_point2CH(new Crosshair(screenInfo, unitsProvider, nullptr, tr("Point 2"), k_point2Id)),
        m_line(new Line(screenInfo, unitsProvider, k_enabledCrosshairOffset)),
        m_dataWin1(new ToolDataWindow(screenInfo, unitsProvider, XY1ReadOnly | DistReadOnly)),
        m_dataWin2(new ToolDataWindow(screenInfo, unitsProvider, XY2ReadOnly | DistReadOnly)) {
    connect(m_point1CH, &Crosshair::entered, this, &LineTool::entered);
    connect(m_point2CH, &Crosshair::entered, this, &LineTool::entered);

    connect(m_point1CH, &Crosshair::departed, this, &LineTool::departed);
    connect(m_point2CH, &Crosshair::departed, this, &LineTool::departed);

    connect(m_point1CH, &Crosshair::dragged, this, &LineTool::dragged);
    connect(m_point2CH, &Crosshair::dragged, this, &LineTool::dragged);

    connect(m_point1CH, &Crosshair::moved, this, &LineTool::moved);
    connect(m_point2CH, &Crosshair::moved, this, &LineTool::moved);
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

void LineTool::setCrosshairsEnabled(bool enable) {
    if (enable) {
        m_line->setOffset(k_enabledCrosshairOffset);
        m_point1CH->show();
        m_point2CH->show();
    } else {
        m_line->setOffset(k_disabledCrosshairOffset);
        m_point1CH->hide();
        m_point2CH->hide();
    }
}

void LineTool::setDataWinEnabled(bool enable) {
    RadioTool::setDataWinEnabled(enable);

    if (!enable) {
        m_dataWin1->hide();
        m_dataWin2->hide();
    }
}

QImage LineTool::grabRegion() const {
    const Cloaker cloak(m_point1CH, m_point2CH, m_line, m_dataWin1, m_dataWin2);

    const QRect regionRect = QRect(m_point1, m_point2).normalized();
    return m_screenInfo->grabScreen(regionRect.x(), regionRect.y(), regionRect.width(), regionRect.height());
}

void LineTool::writeConfig(Config& config) const {
    // Save the position of each end point.
    //
    const QPointF pos1 = m_unitsProvider->convertPos(m_point1);
    config.writeStr("LineX1", StringUtils::dblToStr(pos1.x()));
    config.writeStr("LineY1", StringUtils::dblToStr(pos1.y()));

    const QPointF pos2 = m_unitsProvider->convertPos(m_point2);
    config.writeStr("LineX2", StringUtils::dblToStr(pos2.x()));
    config.writeStr("LineY2", StringUtils::dblToStr(pos2.y()));
}

void LineTool::readConfig(const Config& config) {
    // Use the current positions as the default values for those positions that are not specified in the configuration.
    //
    const QPointF defaultPos1 = m_unitsProvider->convertPos(m_point1);
    const QPointF defaultPos2 = m_unitsProvider->convertPos(m_point2);

    // Load the end point positions.
    //
    QPointF pos1;
    pos1.rx() = config.readDbl("LineX1", defaultPos1.x());
    pos1.ry() = config.readDbl("LineY1", defaultPos1.y());
    m_point1 = m_unitsProvider->unconvertPos(pos1);

    QPointF pos2;
    pos2.rx() = config.readDbl("LineX2", defaultPos2.x());
    pos2.ry() = config.readDbl("LineY2", defaultPos2.y());
    m_point2 = m_unitsProvider->unconvertPos(pos2);

    setPosition();
}

void LineTool::setXY1Position(const QPointF& position) {
    m_activePointId = k_point1Id;
    m_point1 = m_unitsProvider->unconvertCoord(position);
    setPosition();
}

void LineTool::setX1Position(double x) {
    m_activePointId = k_point1Id;
    m_point1.rx() = qRound(m_unitsProvider->unconvertCoord(ConvertX, m_point1CH, x));
    setPosition();
}

void LineTool::setY1Position(double y) {
    m_activePointId = k_point1Id;
    m_point1.ry() = qRound(m_unitsProvider->unconvertCoord(ConvertY, m_point1CH, y));
    setPosition();
}

void LineTool::setXY2Position(const QPointF& position) {
    m_activePointId = k_point2Id;
    m_point2 = m_unitsProvider->unconvertCoord(position);
    setPosition();
}

void LineTool::setX2Position(double x) {
    m_activePointId = k_point2Id;
    m_point2.rx() = qRound(m_unitsProvider->unconvertCoord(ConvertX, m_point2CH, x));
    setPosition();
}

void LineTool::setY2Position(double y) {
    m_activePointId = k_point2Id;
    m_point2.ry() = qRound(m_unitsProvider->unconvertCoord(ConvertY, m_point2CH, y));
    setPosition();
}

void LineTool::stepX1Position(int numSteps) {
    m_activePointId = k_point1Id;
    m_point1.rx() += numSteps;
    setPosition();
}

void LineTool::stepY1Position(int numSteps) {
    m_activePointId = k_point1Id;
    m_point1.ry() += numSteps;
    setPosition();
}

void LineTool::stepX2Position(int numSteps) {
    m_activePointId = k_point2Id;
    m_point2.rx() += numSteps;
    setPosition();
}

void LineTool::stepY2Position(int numSteps) {
    m_activePointId = k_point2Id;
    m_point2.ry() += numSteps;
    setPosition();
}

void LineTool::setPosition() {
    m_point1 = m_screenInfo->constrainPosition(m_point1);
    m_point2 = m_screenInfo->constrainPosition(m_point2);

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

void LineTool::entered(Crosshair&, int id, QPoint, Qt::KeyboardModifiers) {
    if (isDataWinEnabled()) {
        if (id == k_point1Id) {
            m_dataWin1->show();
        } else {
            m_dataWin2->show();
        }
    }
}

void LineTool::departed(Crosshair&, int id) {
    if (id == k_point1Id) {
        m_dataWin1->hide();
    } else {
        m_dataWin2->hide();
    }
}

void LineTool::dragged(Crosshair&, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers) {
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

        movingDelta -= followingPos - m_screenInfo->constrainPosition(followingPos);
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

void LineTool::moved(Crosshair&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    const QPointF coord1 = m_unitsProvider->convertCoord(m_point1);
    const QPointF coord2 = m_unitsProvider->convertCoord(m_point2);
    const QSizeF wh = m_unitsProvider->getWidthHeight(m_point1, m_point2);
    const double distance = Geometry::hypot(wh);
    const double aspect = Geometry::aspectRatio(wh);
    const double area = Geometry::area(wh);
    const double angle = m_unitsProvider->convertAngle(Geometry::angle(coord1, coord2));

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
