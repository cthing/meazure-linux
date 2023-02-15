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

#include "WindowTool.h"
#include <meazure/environment/x11/X11WindowFinder.h>
#include <meazure/environment/x11/X11WindowTracker.h>
#include <meazure/environment/noop/NoopWindowFinder.h>
#include <meazure/environment/noop/NoopWindowTracker.h>
#include <meazure/utils/Geometry.h>
#include <meazure/utils/Cloaker.h>
#include <meazure/utils/PlatformUtils.h>
#include <meazure/graphics/Dimensions.h>

WindowTool::WindowTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent) :
        RadioTool(screenInfo, unitsProvider, parent),
        m_rectangle(new Rectangle(screenInfo, unitsProvider)),
        m_dataWindow(new ToolDataWindow(screenInfo, unitsProvider, WHReadOnly)) {
    m_rectangle->setLineWidth(2 * Dimensions::getLineWidth());

    if (PlatformUtils::isX11()) {
        m_windowFinder = new X11WindowFinder();
        m_windowTracker = new X11WindowTracker(this);
    } else {
        m_windowFinder = new NoopWindowFinder();
        m_windowTracker = new NoopWindowTracker(this);
    }

    connect(m_pointerTracker, &CursorTracker::motion, this, &WindowTool::cursorMotion);
    connect(dynamic_cast<const QObject*>(m_windowTracker),
            SIGNAL(windowChanged(unsigned long, int16_t, int16_t, uint16_t, uint16_t)),
            this, SLOT(windowChanged(unsigned long, int16_t, int16_t, uint16_t, uint16_t)));
}

WindowTool::~WindowTool() {
    setEnabled(false);
    delete m_windowFinder;
    delete m_rectangle;
    delete m_dataWindow;
}

void WindowTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_windowFinder->refresh();
        m_pointerTracker->start();
        m_windowTracker->start();
    } else {
        m_rectangle->hide();
        m_dataWindow->hide();
        m_pointerTracker->stop();
        m_windowTracker->stop();
    }
}

void WindowTool::setDataWinEnabled(bool enable) {
    RadioTool::setDataWinEnabled(enable);

    if (!enable) {
        m_dataWindow->hide();
    }
}

QImage WindowTool::grabRegion() const {
    const Cloaker cloak(m_rectangle, m_dataWindow);

    const QRect regionRect = m_rectangle->geometry();
    return m_screenInfo->grabScreen(regionRect.x(), regionRect.y(), regionRect.width(), regionRect.height());
}

void WindowTool::refresh() {
    setPosition(QCursor::pos());
}

void WindowTool::strobe() {
    m_dataWindow->strobe();
}

bool WindowTool::hasCrosshairs() const {
    return false;
}

void WindowTool::windowChanged(unsigned long, int16_t, int16_t, uint16_t, uint16_t) {
    m_windowFinder->refresh();
}

void WindowTool::cursorMotion(QPoint pos) {
    if (isEnabled()) {
        setPosition(pos);
    }
}

void WindowTool::setPosition(const QPoint& position) {
    if (!isEnabled()) {
        return;
    }

    QRect windowRect = m_windowFinder->find(position);
    if (windowRect.isEmpty()) {
        const int screenIdx = m_screenInfo->screenForPoint(position);
        windowRect = m_screenInfo->getScreenRect(screenIdx);
    }

    const QPoint point1 = windowRect.topLeft();
    const QPoint point2 = windowRect.bottomRight();

    m_rectangle->setPosition(point1, point2);
    m_rectangle->show();

    const QPointF coord1 = m_unitsProvider->convertCoord(point1);
    const QPointF coord2 = m_unitsProvider->convertCoord(point2);
    const QSizeF wh = m_unitsProvider->getWidthHeight(point1, point2);
    const double distance = Geometry::hypot(wh);
    const double aspect = Geometry::aspectRatio(wh);
    const double area = Geometry::area(wh);
    const double angle = m_unitsProvider->convertAngle(Geometry::angle(coord1, coord2));

    m_dataWindow->widthHeightChanged(wh);
    m_dataWindow->moveNear(point1);
    if (isDataWinEnabled()) {
        m_dataWindow->show();
    }

    emit activePositionChanged(point1);
    emit xy1PositionChanged(coord1, point1);
    emit xy2PositionChanged(coord2, point2);
    emit widthHeightChanged(wh);
    emit distanceChanged(distance);
    emit areaChanged(area);
    emit aspectChanged(aspect);
    emit angleChanged(angle);
}
