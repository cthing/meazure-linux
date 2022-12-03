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

#include "Ruler.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/MathUtils.h>
#include <QPainter>
#include <QFontMetrics>


Ruler::Ruler(const ScreenInfoProvider &screenInfoProvider, const UnitsProvider &unitsProvider, bool flip,
             QWidget* parent) :
        Graphic(parent),
        m_screenInfo(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_font("FreeSans", 10),
        m_flip(flip) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowTransparentForInput);

    m_font.setStyleHint(QFont::SansSerif);

    m_fontHeight = QFontMetrics(m_font).height();
}

void Ruler::setPosition(const QPoint& origin, int length, int angle) {
    m_angle = angle;
    m_length = length;

    const double angleFrac = (m_angle % 90) / 90.0;

    const int screenIdx = m_screenInfo.screenForPoint(origin);
    const QSizeF res = m_screenInfo.getScreenRes(screenIdx);

    m_majorTickHeight = convertToPixels(res, angleFrac, k_majorTickHeight, k_majorTickMinHeight);
    m_minorTickHeight = convertToPixels(res, angleFrac, k_minorTickHeight, k_minorTickMinHeight);
    m_labelBottomMargin = convertToPixels(res, angleFrac,k_labelBottomMargin, k_labelBottomMinMargin);
    m_labelTopMargin = convertToPixels(res, angleFrac, k_labelTopMargin, k_labelTopMinMargin);

    const int rulerThk = m_majorTickHeight + m_labelBottomMargin + m_labelTopMargin + m_fontHeight;

    const QRect globalRulerRect(origin.x(), m_flip ? origin.y() : (origin.y() - rulerThk), m_length, rulerThk);
    const QRect boundingRect = Geometry::rotateAround(m_angle, origin).mapRect(globalRulerRect);
    setGeometry(boundingRect);

    const int xoLocal = origin.x() - x();
    const int yoLocal = origin.y() - y();

    m_rulerRect.setRect(xoLocal, m_flip ? yoLocal : (yoLocal - rulerThk), m_length, rulerThk);
    m_rulerTransform = Geometry::rotateAround(m_angle, xoLocal, yoLocal);
}

int Ruler::convertToPixels(const QSizeF& res, double angleFrac, double value, int minValue) {
    const QSize convertedValue = m_unitsProvider.convertToPixels(InchesId, res, value, minValue);
    return MathUtils::linearInterpolate(convertedValue.height(), convertedValue.width(), angleFrac);
}

void Ruler::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    painter.setPen(QColor(0, 0, 0));
    painter.setBrush(QColor(0, 255, 0));

    painter.setTransform(m_rulerTransform);
    painter.drawRect(m_rulerRect);
}
