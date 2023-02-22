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

#include "Circle.h"
#include <meazure/utils/Geometry.h>
#include <QRect>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>


Circle::Circle(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, double gap,
               QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(screenInfo, unitsProvider, parent),
        m_gap(gap),
        m_pen(QBrush(lineColor), lineWidth),
        m_center(50, 50),
        m_perimeter(20, 20) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::WindowTransparentForInput, true);

    connect(Colors::getChangeNotifier(), &Colors::ChangeNotifier::colorChanged, this, &Circle::colorChanged);
    connect(Dimensions::getChangeNotifier(), &Dimensions::ChangeNotifier::lineWidthChanged, this, &Circle::setLineWidth);
    connect(m_screenInfo, &ScreenInfo::resolutionChanged, this, [this]() {
        setPosition(m_center, m_perimeter);
        repaint();
    });
}

void Circle::colorChanged(Colors::Item item, QRgb color) {
    if (item == Colors::LineFore) {
        setColor(color);
    }
}

void Circle::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void Circle::setLineWidth(int width) {
    m_pen.setWidth(width);
    repaint();
}

void Circle::setGap(double gap) {
    m_gap = gap;
    repaint();
}

void Circle::setPosition(const QPoint& center, const QPoint& perimeter) {
    m_center = m_screenInfo->constrainPosition(center);
    m_perimeter = m_screenInfo->constrainPosition(perimeter);
    m_radius = Geometry::hypot(m_center, m_perimeter);

    const int x = qRound(m_center.x() - m_radius);
    const int y = qRound(m_center.y() - m_radius);
    const int wh = qRound(2.0 * m_radius);
    setGeometry(QRect(x, y, wh, wh));
}

void Circle::paintEvent(QPaintEvent*) {
    const int screenIndex = m_screenInfo->screenForPoint(m_perimeter);
    const QSizeF screenRes = m_screenInfo->getScreenRes(screenIndex);

    const QSize gap = (m_gap > 0.0) ? m_unitsProvider->convertToPixels(InchesId, screenRes, m_gap, 1) : QSize(0, 0);
    const double halfGapAngle = qRadiansToDegrees(gap.width()/m_radius);
    const double startAngle = 360.0 - qRadiansToDegrees(Geometry::angle(m_center, m_perimeter)) + halfGapAngle;
    const double spanAngle = 360.0 - 2.0 * halfGapAngle;
    const double halfPenWidth = m_pen.width() / 2.0;

    QRectF winRect(rect());
    winRect.adjust(halfPenWidth, halfPenWidth, -halfPenWidth, -halfPenWidth);

    QPainterPath path;
    path.arcMoveTo(winRect, startAngle);
    path.arcTo(winRect, startAngle, spanAngle);

    QPainter painter(this);
    painter.strokePath(path, m_pen);
}
