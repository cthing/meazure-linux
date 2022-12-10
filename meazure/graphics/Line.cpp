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

#include "Line.h"
#include <meazure/utils/Geometry.h>
#include <QSize>
#include <QPainter>
#include <cmath>


Line::Line(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider, double offset,
           QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(parent),
        m_screenInfo(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_offset(offset),
        m_lineWidth(lineWidth),
        m_pen(QBrush(lineColor), lineWidth),
        m_start(1, 1),
        m_end(10, 10) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowTransparentForInput);
}

void Line::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void Line::setLineWidth(int width) {
    m_pen.setWidth(width);
    m_lineWidth = width;
    repaint();
}

void Line::setPosition(const QPoint& start, const QPoint& end) {
    m_start = start;
    m_end = end;

    QPoint normalizedStart(std::min(start.x(), end.x()), std::min(start.y(), end.y()));
    QPoint normalizedEnd(std::max(start.x(), end.x()), std::max(start.y(), end.y()));

    normalizedStart = m_screenInfo.constrainPosition(normalizedStart);
    normalizedEnd = m_screenInfo.constrainPosition(normalizedEnd);
    QRect windowRect(normalizedStart, normalizedEnd);

    if (m_offset > 0.0) {
        const int screenIndex = m_screenInfo.screenForPoint(normalizedStart);
        const QSizeF screenRes = m_screenInfo.getScreenRes(screenIndex);

        const double angle = Geometry::angle(normalizedStart, normalizedEnd);
        const QSize offset = m_unitsProvider.convertToPixels(InchesId, screenRes, m_offset, 0.0);
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
