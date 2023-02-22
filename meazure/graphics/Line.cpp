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

#include "Line.h"
#include <meazure/utils/Geometry.h>
#include <QSize>
#include <QPainter>
#include <cmath>


Line::Line(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, double offset,
           QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(screenInfo, unitsProvider, parent),
        m_offset(offset),
        m_lineWidth(lineWidth),
        m_pen(QBrush(lineColor), lineWidth),
        m_start(1, 1),
        m_end(10, 10) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::WindowTransparentForInput, true);

    connect(Colors::getChangeNotifier(), &Colors::ChangeNotifier::colorChanged, this, &Line::colorChanged);
    connect(Dimensions::getChangeNotifier(), &Dimensions::ChangeNotifier::lineWidthChanged, this, &Line::setLineWidth);
    connect(m_screenInfo, &ScreenInfo::resolutionChanged, this, [this]() {
        setPosition(m_start, m_end);
        repaint();
    });
}

void Line::colorChanged(Colors::Item item, QRgb color) {
    if (item == Colors::LineFore) {
        setColor(color);
    }
}

void Line::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void Line::setLineWidth(int width) {
    m_pen.setWidth(width);
    m_lineWidth = width;
    setPosition(m_start, m_end);
}

void Line::setOffset(double offset) {
    m_offset = offset;
    setPosition(m_start, m_end);
}

void Line::setPosition(const QPoint& start, const QPoint& end) {
    m_start = start;
    m_end = end;

    QPoint normalizedStart(std::min(start.x(), end.x()), std::min(start.y(), end.y()));
    QPoint normalizedEnd(std::max(start.x(), end.x()), std::max(start.y(), end.y()));

    normalizedStart = m_screenInfo->constrainPosition(normalizedStart);
    normalizedEnd = m_screenInfo->constrainPosition(normalizedEnd);
    QRect windowRect(normalizedStart, normalizedEnd);

    if (m_offset > 0.0) {
        const int screenIndex = m_screenInfo->screenForPoint(normalizedStart);
        const QSizeF screenRes = m_screenInfo->getScreenRes(screenIndex);

        const double angle = Geometry::angle(normalizedStart, normalizedEnd);
        const QSize offset = m_unitsProvider->convertToPixels(InchesId, screenRes, m_offset, 1);
        const int offsetX = qRound(offset.width() * std::cos(angle));
        const int offsetY = qRound(offset.height() * std::sin(angle));

        windowRect.adjust(offsetX, offsetY, -offsetX, -offsetY);
    }

    // Grow the window by the line thickness so that it is not clipped by the window edge when vertical or horizontal.
    //
    windowRect.adjust(-m_lineWidth, -m_lineWidth, m_lineWidth, m_lineWidth);

    setGeometry(windowRect);
}

void Line::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(m_pen);

    // Compensate for margin.
    //
    const int right = width() - m_lineWidth - 1;
    const int bottom = height() - m_lineWidth - 1;

    if ((m_start.x() > m_end.x()) != (m_start.y() > m_end.y())) {
        painter.drawLine(m_lineWidth, bottom, right, m_lineWidth);
    } else {
        painter.drawLine(m_lineWidth, m_lineWidth, right, bottom);
    }
}
