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
#include <QLine>


Rectangle::Rectangle(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider, double offset,
                     QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(parent),
        m_screenInfo(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_offset(offset),
        m_lineWidth(lineWidth),
        m_start(1, 1),
        m_end(10, 10) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowTransparentForInput);

    m_pen.setColor(lineColor);
    m_pen.setWidth(lineWidth);
}

void Rectangle::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void Rectangle::setLineWidth(int width) {
    m_pen.setWidth(width);
    m_lineWidth = width;
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
    windowRect.adjust(-m_lineWidth, -m_lineWidth, m_lineWidth, m_lineWidth);

    setGeometry(windowRect);
}

void Rectangle::paintEvent(QPaintEvent*) {
    static constexpr int k_numSides = 4;

    QSize offset(0, 0);

    if (m_offset > 0.0) {
        const int screenIndex = m_screenInfo.screenForPoint(m_start);
        const QSizeF screenRes = m_screenInfo.getScreenRes(screenIndex);

        offset = m_unitsProvider.convertToPixels(InchesId, screenRes, m_offset, 0.0);
    }

    QPainter painter(this);
    painter.setPen(m_pen);

    // Compensate for margin.
    //
    const int right = width() - m_lineWidth - 1;
    const int bottom = height() - m_lineWidth - 1;

    if ((m_start.x() > m_end.x()) != (m_start.y() > m_end.y())) {
        const QLine lines[k_numSides] = {
                QLine(m_lineWidth, m_lineWidth, right - offset.width(), m_lineWidth),    // Top
                QLine(m_lineWidth, m_lineWidth, m_lineWidth, bottom - offset.height()),  // Left
                QLine(right, m_lineWidth + offset.height(), right, bottom),              // Right
                QLine(m_lineWidth + offset.width(), bottom, right, bottom)               // Bottom
        };
        painter.drawLines(lines, k_numSides);
    } else {
        const QLine lines[k_numSides] = {
                QLine(m_lineWidth + offset.width(), m_lineWidth, right, m_lineWidth),    // Top
                QLine(m_lineWidth, m_lineWidth + offset.height(), m_lineWidth, bottom),  // Left
                QLine(right, m_lineWidth, right, bottom - offset.height()),              // Right
                QLine(m_lineWidth, bottom, right - offset.width(), bottom)               // Bottom
        };
        painter.drawLines(lines, k_numSides);
    }
}
