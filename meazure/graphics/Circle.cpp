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

#include "Circle.h"
#include <meazure/utils/Geometry.h>
#include <QRect>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>


Circle::Circle(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider, double gap,
               QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(parent),
        m_screenInfo(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_gap(gap),
        m_pen(QBrush(lineColor), lineWidth),
        m_center(50, 50),
        m_perimeter(20, 20) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowTransparentForInput);
}

void Circle::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void Circle::setLineWidth(int width) {
    m_pen.setWidth(width);
    repaint();
}

void Circle::setPosition(const QPoint& center, const QPoint& perimeter) {
    m_center = m_screenInfo.constrainPosition(center);
    m_perimeter = m_screenInfo.constrainPosition(perimeter);
    m_radius = Geometry::hypot(m_center, m_perimeter);

    const int x = qRound(m_center.x() - m_radius);
    const int y = qRound(m_center.y() - m_radius);
    const int wh = qRound(2.0 * m_radius);
    setGeometry(QRect(x, y, wh, wh));
}

void Circle::paintEvent(QPaintEvent*) {
    const int screenIndex = m_screenInfo.screenForPoint(m_perimeter);
    const QSizeF screenRes = m_screenInfo.getScreenRes(screenIndex);

    const QSize gap = m_unitsProvider.convertToPixels(InchesId, screenRes, m_gap, 0.0);
    const double halfGapAngle = qRadiansToDegrees(gap.width()/m_radius);
    const double startAngle = 360.0 - qRadiansToDegrees(Geometry::angle(m_center, m_perimeter)) + halfGapAngle;
    const double spanAngle = 360.0 - 2.0 * halfGapAngle;

    const QRectF winRect(rect());
    QPainterPath path;
    path.arcMoveTo(winRect, startAngle);
    path.arcTo(winRect, startAngle, spanAngle);

    QPainter painter(this);
    painter.strokePath(path, m_pen);
}
