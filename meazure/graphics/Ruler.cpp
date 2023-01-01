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
#include <QGraphicsOpacityEffect>
#include <utility>


Ruler::Ruler(const ScreenInfoProvider &screenInfoProvider, const UnitsProvider &unitsProvider, bool flip,
             QWidget* parent, QRgb backgroundColor, QRgb borderColor, QRgb opacity) :
        Graphic(screenInfoProvider, unitsProvider, parent),
        m_backgroundBrush(backgroundColor),
        m_linePen(QBrush(borderColor), k_lineWidth),
        m_font("FreeSans", 10),
        m_fontMetrics(m_font),
        m_flip(flip) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::WindowTransparentForInput, true);

    if (parent == nullptr) {
        setWindowOpacity(Colors::opacityToFraction(opacity));
    } else {
        auto* effect = new QGraphicsOpacityEffect();
        effect->setOpacity(Colors::opacityToFraction(opacity));
        setGraphicsEffect(effect);
    }

    connect(Colors::getChangeNotifier(), &Colors::ChangeNotifier::colorChanged, this, &Ruler::colorChanged);

    m_font.setLetterSpacing(QFont::PercentageSpacing, 120);
}

void Ruler::colorChanged(Colors::Item item, QRgb color) {
    switch (item) {
        case Colors::RulerBack:
            setColors(color, Colors::get(Colors::RulerBorder));
            break;
        case Colors::RulerBorder:
            setColors(Colors::get(Colors::RulerBack), color);
            break;
        case Colors::RulerOpacity:
            setOpacity(Colors::opacityToPercent(color));
            break;
        default:
            break;
    }
}

void Ruler::setColors(QRgb background, QRgb border) {
    m_backgroundBrush.setColor(background);
    m_linePen.setColor(border);

    repaint();
}

void Ruler::setOpacity(int opacityPercent) {
    if (parent() == nullptr) {
        setWindowOpacity(opacityPercent / 100.0);
    } else {
        auto* effect = dynamic_cast<QGraphicsOpacityEffect*>(graphicsEffect());
        effect->setOpacity(opacityPercent / 100.0);
    }
}

void Ruler::setPosition(const QPoint& origin, int length, int angle) {
    m_angle = angle;
    m_length = length;
    m_angleFraction = (m_angle % 90) / 90.0;

    const int screenIdx = m_screenInfo.screenForPoint(origin);
    const QSizeF res = m_screenInfo.getScreenRes(screenIdx);

    m_majorTickHeight = convertToPixels(InchesId, res, m_angleFraction, k_majorTickHeight, k_majorTickMinHeight);
    m_minorTickHeight = convertToPixels(InchesId, res, m_angleFraction, k_minorTickHeight, k_minorTickMinHeight);
    m_labelBottomMargin = convertToPixels(InchesId, res, m_angleFraction,k_labelBottomMargin, k_labelBottomMinMargin);
    m_labelTopMargin = convertToPixels(InchesId, res, m_angleFraction, k_labelTopMargin, k_labelTopMinMargin);

    const int rulerThk = m_majorTickHeight + m_labelBottomMargin + m_labelTopMargin + m_fontMetrics.height();

    const QRect globalRulerRect(origin.x(), m_flip ? origin.y() : (origin.y() - rulerThk), m_length, rulerThk);
    const QRect boundingRect = Geometry::rotateAround(m_angle, origin).mapRect(globalRulerRect);
    setGeometry(boundingRect);

    const int xoLocal = origin.x() - x();
    const int yoLocal = origin.y() - y();

    m_rulerRect.setRect(xoLocal, m_flip ? yoLocal : (yoLocal - rulerThk), m_length, rulerThk);
    m_rulerTransform = Geometry::rotateAround(m_angle, xoLocal, yoLocal);
}

int Ruler::convertToPixels(LinearUnitsId unitsId, const QSizeF& res, double angleFrac, double value, int minValue) {
    const QSize convertedValue = m_unitsProvider.convertToPixels(unitsId, res, value, minValue);
    return MathUtils::linearInterpolate(convertedValue.height(), convertedValue.width(), angleFrac);
}

int Ruler::convertToPixels(const QSizeF& res, double angleFrac, double value, int minValue) {
    return convertToPixels(m_unitsProvider.getLinearUnitsId(), res, angleFrac, value, minValue);
}

void Ruler::setIndicator(int indicatorIdx, int position) {
    m_indicators.at(indicatorIdx) = position;
    repaint();
}

void Ruler::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setFont(m_font);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(m_linePen);
    painter.setBrush(m_backgroundBrush);

    const int majorTickCount = m_unitsProvider.getMajorTickCount();

    for (int idx = 0; idx < m_screenInfo.getNumScreens(); idx++) {
        const QRect intersectRect = m_screenInfo.getScreenRect(idx).intersected(geometry());
        if (intersectRect.isEmpty()) {
            return;
        }

        const QSizeF res = m_screenInfo.getScreenRes(idx);
        const QSizeF minorTickIncr = m_unitsProvider.getMinorTickIncr(intersectRect);
        const double effectiveMinorTickIncr = MathUtils::linearInterpolate(minorTickIncr.width(),
                                                                           minorTickIncr.height(),
                                                                           m_angleFraction);

        std::vector<std::pair<int, QString>> labels;
        std::vector<QLine> lines;
        int l;          // NOLINT(cppcoreguidelines-init-variables)
        int tick;       // NOLINT(cppcoreguidelines-init-variables)
        double p;       // NOLINT(cppcoreguidelines-init-variables)
        for (l = 0, p = 0.0, tick = 0; l < m_length;
             tick++, p += effectiveMinorTickIncr, l = convertToPixels(res, m_angleFraction, p, 1)) {
            const int x = m_rulerRect.x() + l;
            const bool isMajorTick = ((tick % majorTickCount) == 0);
            const int tickHeight = isMajorTick ? m_majorTickHeight : m_minorTickHeight;

            // Tick marks
            if (m_flip) {
                lines.emplace_back(x, m_rulerRect.top(), x, m_rulerRect.top() + tickHeight);
            } else {
                lines.emplace_back(x, m_rulerRect.bottom(), x, m_rulerRect.bottom() - tickHeight);
            }

            // Labels
            if (isMajorTick && p > 0.0) {
                labels.emplace_back(x, m_unitsProvider.format(Width, p));
            }
        }

        // Indicators
        for (const int indicatorX : m_indicators) {
            if (indicatorX != k_unusedIndicator) {
                const int x = m_rulerRect.x() + indicatorX;
                lines.emplace_back(x, m_rulerRect.top(), x, m_rulerRect.bottom());
            }
        }

        const QRect clipRect(intersectRect.topLeft() - geometry().topLeft(), intersectRect.size());
        painter.save();
        painter.setClipRect(clipRect);
        painter.setTransform(m_rulerTransform);
        painter.drawRect(m_rulerRect);
        painter.drawLines(lines.data(), static_cast<int>(lines.size()));

        const int labelBottom = m_flip
                ? m_rulerRect.top() + m_majorTickHeight + m_labelBottomMargin
                : m_rulerRect.bottom() - m_majorTickHeight - m_labelBottomMargin;

        // Ensure text is not drawn upside down.
        if (m_angle >= 90 || m_angle <= -90) {
            const int y = m_flip ? labelBottom + m_fontMetrics.descent() : labelBottom - m_fontMetrics.ascent();
            for (const std::pair<int, QString> &label: labels) {
                const int x = label.first - m_fontMetrics.boundingRect(label.second).width() / 2;
                painter.save();
                painter.setTransform(Geometry::rotateAround(180, label.first, y) * m_rulerTransform);
                painter.drawText(x, y, label.second);
                painter.restore();
            }
        } else {
            const int y = m_flip ? labelBottom + m_fontMetrics.ascent() : labelBottom - m_fontMetrics.descent();
            for (const std::pair<int, QString> &label: labels) {
                const int x = label.first - m_fontMetrics.boundingRect(label.second).width() / 2;
                painter.drawText(x, y, label.second);
            }
        }

        painter.restore();
    }
}
