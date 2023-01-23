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

#include "OriginMarker.h"
#include <QPainter>
#include <QSizeF>
#include <QBrush>


OriginMarker::OriginMarker(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider,
                            QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(screenInfo, unitsProvider, parent),
        m_pen(QBrush(lineColor), lineWidth) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::WindowTransparentForInput, true);

    connect(Colors::getChangeNotifier(), &Colors::ChangeNotifier::colorChanged, this, &OriginMarker::colorChanged);
    connect(Dimensions::getChangeNotifier(), &Dimensions::ChangeNotifier::lineWidthChanged, this,
            &OriginMarker::setLineWidth);
    connect(m_screenInfo, &ScreenInfo::resolutionChanged, this, [this]() {
        setPosition(m_origin, m_inverted);
        repaint();
    });
}

void OriginMarker::colorChanged(Colors::Item item, QRgb color) {
    if (item == Colors::LineFore) {
        setColor(color);
    }
}

void OriginMarker::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void OriginMarker::setLineWidth(int width) {
    m_pen.setWidth(width);
    repaint();
}

void OriginMarker::setPosition(const QPoint& origin, bool inverted) {
    m_origin = origin;
    m_inverted = inverted;

    // The length of the axes are expressed in inches and are converted to pixels based on the resolution of the
    // display containing the origin.
    //
    const int screenIdx = m_screenInfo->screenForPoint(m_origin);
    const QSizeF res = m_screenInfo->getScreenRes(screenIdx);
    m_axisLength = m_unitsProvider->convertToPixels(InchesId, res, k_axesLength, k_axesLengthMin);

    if (inverted) {
        setGeometry(m_origin.x(), m_origin.y() - m_axisLength.height(), m_axisLength.width(), m_axisLength.height());
    } else {
        setGeometry(m_origin.x(), m_origin.y(), m_axisLength.width(), m_axisLength.height());
    }
}

void OriginMarker::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(m_pen);

    const int y = m_inverted ? rect().bottom() : 0;
    QLine axes[] = {
            // x-axis
            QLine(0, y, rect().right(), y),
            // y-axis
            QLine(0, 0, 0, rect().bottom())
    };

    painter.drawLines(axes, 2);
}
