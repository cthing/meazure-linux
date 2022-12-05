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

#include "RulerTool.h"
#include <meazure/utils/Geometry.h>
#include <QTransform>

RulerTool::RulerTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                     QObject* parent) :
        Tool(screenInfoProvider, unitsProvider, parent),
        m_origin(screenInfoProvider.getCenter() - QPoint(200, 200)),
        m_moveAnchorPoint(m_origin),
        m_hRuler(new Ruler(screenInfoProvider, unitsProvider, false)),
        m_vRuler(new Ruler(screenInfoProvider, unitsProvider, true)),
        m_hLengthHandle(new Handle(screenInfoProvider, unitsProvider, nullptr, tr("Adjust length"), k_hLengthId)),
        m_vLengthHandle(new Handle(screenInfoProvider, unitsProvider, nullptr, tr("Adjust length"), k_vLengthId)),
        m_moveHandle(new Handle(screenInfoProvider, unitsProvider, nullptr, tr("Move rulers"), k_moveId)),
        m_rotateHandle(new Handle(screenInfoProvider, unitsProvider, nullptr, tr("Rotate rulers"), k_rotateId)),
        m_hLengthDataWin(new ToolDataWindow(screenInfoProvider, unitsProvider, DistReadOnly)),
        m_vLengthDataWin(new ToolDataWindow(screenInfoProvider, unitsProvider, DistReadOnly)),
        m_moveDataWin(new ToolDataWindow(screenInfoProvider, unitsProvider, XYVReadOnly)),
        m_rotateDataWin(new ToolDataWindow(screenInfoProvider, unitsProvider, AngleReadOnly)) {
    connect(m_rotateHandle, &Handle::entered, this, &RulerTool::handleEntered);
    connect(m_rotateHandle, &Handle::departed, this, &RulerTool::handleDeparted);
    connect(m_rotateHandle, &Handle::dragged, this, &RulerTool::rotateHandleDragged);
    connect(m_rotateHandle, &Handle::moved, this, &RulerTool::moved);

    connect(m_moveHandle, &Handle::entered, this, &RulerTool::handleEntered);
    connect(m_moveHandle, &Handle::departed, this, &RulerTool::handleDeparted);
    connect(m_moveHandle, &Handle::dragged, this, &RulerTool::moveHandleDragged);
    connect(m_moveHandle, &Handle::moved, this, &RulerTool::moved);

    connect(m_hLengthHandle, &Handle::entered, this, &RulerTool::handleEntered);
    connect(m_hLengthHandle, &Handle::departed, this, &RulerTool::handleDeparted);
    connect(m_hLengthHandle, &Handle::dragged, this, &RulerTool::lengthHandleDragged);
    connect(m_hLengthHandle, &Handle::moved, this, &RulerTool::moved);

    connect(m_vLengthHandle, &Handle::entered, this, &RulerTool::handleEntered);
    connect(m_vLengthHandle, &Handle::departed, this, &RulerTool::handleDeparted);
    connect(m_vLengthHandle, &Handle::dragged, this, &RulerTool::lengthHandleDragged);
    connect(m_vLengthHandle, &Handle::moved, this, &RulerTool::moved);
}

RulerTool::~RulerTool() {
    m_rotateDataWin->hide();
    setEnabled(false);
    delete m_hRuler;
    delete m_vRuler;
    delete m_hLengthHandle;
    delete m_vLengthHandle;
    delete m_moveHandle;
    delete m_rotateHandle;
    delete m_hLengthDataWin,
    delete m_vLengthDataWin,
    delete m_moveDataWin;
    delete m_rotateDataWin;
}

void RulerTool::setEnabled(bool enable) {
    Tool::setEnabled(enable);

    if (enable) {
        m_hRuler->show();
        m_vRuler->show();
        m_hLengthHandle->show();
        m_vLengthHandle->show();
        m_moveHandle->show();
        m_rotateHandle->show();
    } else {
        m_hRuler->hide();
        m_vRuler->hide();
        m_hLengthHandle->hide();
        m_vLengthHandle->hide();
        m_moveHandle->hide();
        m_rotateHandle->hide();
    }
}

void RulerTool::saveProfile(Profile& profile) const {
    profile.writeBool("Rulers", isEnabled());

    // The tag prefix "RulerSet0" is for compatibility with versions of Meazure on Windows.
    profile.writeInt("RulerSet0-XPos", m_origin.x());
    profile.writeInt("RulerSet0-YPos", m_origin.y());
    profile.writeInt("RulerSet0-HLength", m_hLength);
    profile.writeInt("RulerSet0-VLength", m_vLength);
    profile.writeInt("RulerSet0-Angle", m_angle);
}

void RulerTool::loadProfile(Profile& profile) {
    // The tag prefix "RulerSet0" is for compatibility with versions of Meazure on Windows.
    m_origin = QPoint(profile.readInt("RulerSet0-XPos", m_origin.x()),
                      profile.readInt("RulerSet0-YPos", m_origin.y()));
    m_hLength = profile.readInt("RulerSet0-HLength", m_hLength);
    m_vLength = profile.readInt("RulerSet0-VLength", m_vLength);
    m_angle = profile.readInt("RulerSet0-Angle", m_angle);

    refresh();
    setEnabled(profile.readBool("Rulers", false));
}

void RulerTool::setPositionConstrained(const QPoint &origin, int angle, int hLength, int vLength) {
    const QPoint originalOrigin(m_origin);
    const int originalAngle = m_angle;
    const int originalHLength = m_hLength;
    const int originalVLength = m_vLength;

    m_origin = origin;
    m_angle = angle;
    m_hLength = hLength;
    m_vLength = vLength;

    if (!setPosition()) {
        m_origin = originalOrigin;
        m_angle = originalAngle;
        m_hLength = originalHLength;
        m_vLength = originalVLength;

        setPosition();
    }
}

bool RulerTool::setPosition() {
    m_hRuler->setPosition(m_origin, m_hLength, m_angle);
    m_vRuler->setPosition(m_origin, m_vLength, m_angle + 90);

    const int hRulerThk = m_hRuler->getRulerThk();
    const int hRulerHalfThk = qRound(hRulerThk / 2.0);
    const int vRulerHalfThk = qRound(m_vRuler->getRulerThk() / 2.0);

    const QTransform originTransform = Geometry::rotateAround(m_angle, m_origin);

    const QPoint originHandlePos(m_origin.x() - hRulerHalfThk, m_origin.y() - vRulerHalfThk);
    m_moveHandle->setPosition(originTransform.map(originHandlePos));

    const QPoint hHandlePos(m_origin.x() + m_hLength + qRound(m_hLengthHandle->rect().width() / 2.0),
                            m_origin.y() - hRulerHalfThk);
    m_hLengthHandle->setPosition(originTransform.map(hHandlePos));

    const QPoint rotateHandlePos(m_origin.x() + m_hLength + m_hLengthHandle->rect().width()
                                 + k_rotateHandleSpacing + qRound(m_rotateHandle->rect().width() / 2.0),
                                 m_origin.y());
    m_rotateHandle->setPosition(originTransform.map(rotateHandlePos));

    const QPoint vHandlePos(m_origin.x() - vRulerHalfThk,
                            m_origin.y() + m_vLength + qRound(m_vLengthHandle->rect().height() / 2.0));
    m_vLengthHandle->setPosition(originTransform.map(vHandlePos));

    const QTransform indicatorTransform = Geometry::rotateAround(-m_angle, m_origin);
    for (int i = 0; i < static_cast<int>(m_indicators.size()); i++) {
        const QPoint indicatorPos = m_indicators[i];
        if (indicatorPos == k_unusedIndicator) {
            m_hRuler->setIndicator(i, indicatorPos.x());
            m_vRuler->setIndicator(i, indicatorPos.y());
        } else {
            const QPoint relativePos = indicatorTransform.map(indicatorPos) - m_origin;
            m_hRuler->setIndicator(i, relativePos.x());
            m_vRuler->setIndicator(i, relativePos.y());
        }
    }

    const QRect boundingRect = constructBoundingRect();
    return getScreenInfo().getAvailableVirtualRect().contains(boundingRect);
}

QRect RulerTool::constructBoundingRect() {
    return m_moveHandle->geometry()
                       .united(m_hLengthHandle->geometry())
                       .united(m_vLengthHandle->geometry())
                       .united(m_rotateHandle->geometry())
                       .united(m_hRuler->geometry())
                       .united(m_vRuler->geometry());
}

void RulerTool::refresh() {
    setPosition();

    m_hRuler->repaint();
    m_vRuler->repaint();
}

void RulerTool::radioToolSelected(RadioTool&) {
    m_indicators.fill(k_unusedIndicator);
    setPosition();
}

void RulerTool::setIndicator(int indicatorIdx, QPoint pos) {
    m_indicators.at(indicatorIdx) = pos;
    setPosition();
}

void RulerTool::handleEntered(Handle&, int id, QPoint center, Qt::KeyboardModifiers) {
    switch (id) {
        case k_hLengthId:
            m_hLengthDataWin->show();
            break;
        case k_vLengthId:
            m_vLengthDataWin->show();
            break;
        case k_moveId:
            m_moveAnchorPoint = center;
            m_moveDataWin->show();
            break;
        case k_rotateId:
            m_rotateDataWin->show();
            break;
        default:
            break;
    }
}

void RulerTool::handleDeparted(Handle&, int id) {
    switch (id) {
        case k_hLengthId:
            m_hLengthDataWin->hide();
            break;
        case k_vLengthId:
            m_vLengthDataWin->hide();
            break;
        case k_moveId:
            m_moveDataWin->hide();
            break;
        case k_rotateId:
            m_rotateDataWin->hide();
            break;
        default:
            break;
    }
}

void RulerTool::lengthHandleDragged(Handle&, int id, QPoint center, Qt::KeyboardModifiers) {
    static constexpr auto k_calcNewLength = [](int angle, const QPoint& origin, const QWidget* handle,
                                               const QPoint& newCenter, int originalLength) {
        const QTransform inverseTransform = Geometry::rotateAround(angle, origin);
        const QPoint transformedOriginalCenter = inverseTransform.map(handle->geometry().center());
        const QPoint transformedNewCenter = inverseTransform.map(newCenter);
        const QPoint delta = transformedNewCenter - transformedOriginalCenter;
        const int newLength = originalLength + delta.x();

        return (newLength > k_minLength) ? newLength : k_minLength;
    };

    if (id == k_hLengthId) {
        const int newLength = k_calcNewLength(-m_angle, m_origin, m_hLengthHandle, center, m_hLength);
        setPositionConstrained(m_origin, m_angle, newLength, m_vLength);
    } else {
        const int newLength = k_calcNewLength(-(m_angle + 90), m_origin, m_vLengthHandle, center, m_vLength);
        setPositionConstrained(m_origin, m_angle, m_hLength, newLength);
    }
}

void RulerTool::rotateHandleDragged(Handle&, int, QPoint center, Qt::KeyboardModifiers keyboardModifiers) {
    int angle = m_angle;

    if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
        const int sector = Geometry::calcSector(m_origin, center);
        switch (sector) {
            case -3:
            case -2:
                angle = -90;
                break;
            case -1:
            case 1:
                angle = 0;
                break;
            case 2:
            case 3:
                angle = 90;
                break;
            case 4:
            case -4:
                angle = 180;
                break;
            case 0:
            default:
                break;
        }
    } else {
        const double angleRadians = Geometry::angle(m_origin, center);
        angle = qRound(qRadiansToDegrees(angleRadians));
    }

    setPositionConstrained(m_origin, angle, m_hLength, m_vLength);
}

void RulerTool::moveHandleDragged(Handle&, int, QPoint center, Qt::KeyboardModifiers keyboardModifiers) {
    QPoint origin(m_origin);

    const QPoint movingDelta = center - m_moveHandle->geometry().center();

    if ((keyboardModifiers & Qt::ShiftModifier) != 0) {
        if (Geometry::isVerticallyOriented(m_moveAnchorPoint, center)) {
            origin.ry() += movingDelta.y();
        } else {
            origin.rx() += movingDelta.x();
        }
    } else {
        origin += movingDelta;
        m_moveAnchorPoint = center;
    }

    setPositionConstrained(origin, m_angle, m_hLength, m_vLength);
}

void RulerTool::moved(Handle&, int id, QPoint) {
    if (!isEnabled()) {
        return;
    }

    switch (id) {
        case k_hLengthId:
            m_hLengthDataWin->distanceChanged(m_hLength);
            break;
        case k_vLengthId:
            m_vLengthDataWin->distanceChanged(m_vLength);
            break;
        case k_moveId:
            m_moveDataWin->xyvPositionChanged(getUnitsProvider().convertCoord(m_origin), m_origin);
            break;
        case k_rotateId:
            m_rotateDataWin->angleChanged(getUnitsProvider().convertAngle(qDegreesToRadians(m_angle)));
            break;
        default:
            break;
    }

    m_hLengthDataWin->moveNear(m_hLengthHandle->geometry());
    m_vLengthDataWin->moveNear(m_vLengthHandle->geometry());
    m_moveDataWin->moveNear(m_moveHandle->geometry());
    m_rotateDataWin->moveNear(m_rotateHandle->geometry());
}
