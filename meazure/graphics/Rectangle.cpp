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

#include "Rectangle.h"
#include <QPainter>
#include <QSize>


Rectangle::Rectangle(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider, double offset,
                     QWidget* parent, QRgb lineColor) :
        Graphic(parent),
        m_screenInfo(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_offset(offset),
        m_color(lineColor),
        m_start(1, 1),
        m_end(10, 10) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowTransparentForInput);
}

void Rectangle::setColor(QRgb color) {
    m_color.setRgb(color);
    repaint();
}

void Rectangle::setPosition(const QPoint& start, const QPoint& end) {
    m_start = start;
    m_end = end;

    QPoint normalizedStart(std::min(start.x(), end.x()), std::min(start.y(), end.y()));
    QPoint normalizedEnd(std::max(start.x(), end.x()), std::max(start.y(), end.y()));

    normalizedStart = m_screenInfo.constrainPosition(normalizedStart);
    normalizedEnd = m_screenInfo.constrainPosition(normalizedEnd);
    QRect windowRect(normalizedStart, normalizedEnd);

    // Grow the window by the line thickness so that it is not clipped by the window edge when vertical or horizontal.
    //
    windowRect.adjust(-k_lineWidth, -k_lineWidth, k_lineWidth, k_lineWidth);

    setGeometry(windowRect);
}

void Rectangle::paintEvent(QPaintEvent*) {
    QSize offset(0, 0);

    if (m_offset > 0.0) {
        const int screenIndex = m_screenInfo.screenForPoint(m_start);
        const QSizeF screenRes = m_screenInfo.getScreenRes(screenIndex);

        offset = m_unitsProvider.convertToPixels(InchesId, screenRes, m_offset, 0.0);
    }

    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(m_color, k_lineWidth));

    // Compensate for margin.
    //
    const int right = width() - k_lineWidth - 1;
    const int bottom = height() - k_lineWidth - 1;

    if ((m_start.x() > m_end.x()) != (m_start.y() > m_end.y())) {
        painter.drawLine(k_lineWidth, k_lineWidth, right - offset.width(), k_lineWidth);    // Top
        painter.drawLine(k_lineWidth, k_lineWidth, k_lineWidth, bottom - offset.height());  // Left
        painter.drawLine(right, k_lineWidth + offset.height(), right, bottom);              // Right
        painter.drawLine(k_lineWidth + offset.width(), bottom, right, bottom);              // Bottom
    } else {
        painter.drawLine(k_lineWidth + offset.width(), k_lineWidth, right, k_lineWidth);    // Top
        painter.drawLine(k_lineWidth, k_lineWidth + offset.height(), k_lineWidth, bottom);  // Left
        painter.drawLine(right, k_lineWidth, right, bottom - offset.height());              // Right
        painter.drawLine(k_lineWidth, bottom, right - offset.width(), bottom);              // Bottom
    }
}
