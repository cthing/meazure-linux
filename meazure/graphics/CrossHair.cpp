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

#include "CrossHair.h"
#include "Colors.h"
#include "Plotter.h"
#include <meazure/utils/MathUtils.h>
#include <QPoint>
#include <QPainterPath>
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>


CrossHair::CrossHair(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                     QWidget *parent, const QString& tooltip, int id, const QRgb backgroundColor, QRgb hiliteColor,
                     QRgb borderColor, QRgb opacity) :
        Graphic(parent),
        m_screenProvider(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_id(id),
        m_backgroundBrush(backgroundColor),
        m_hiliteBrush(hiliteColor),
        m_hilitePen(QBrush(hiliteColor), k_outlineWidth),
        m_borderPen(QBrush(borderColor), k_outlineWidth) {

    const QPoint screenCenter = m_screenProvider.getCenter();
    const int screenIndex = m_screenProvider.screenForPoint(screenCenter);
    const QSizeF screenRes = m_screenProvider.getScreenRes(screenIndex);

    QSize actualSize = m_unitsProvider.convertToPixels(InchesId, screenRes, k_outerSize, k_outerSizeMin);
    actualSize.rwidth() = MathUtils::makeOddUp(actualSize.width());       // Must be odd
    actualSize.rheight() = MathUtils::makeOddUp(actualSize.height());
    setFixedSize(actualSize);

    m_centerPosition = QPoint((actualSize.width() - 1) / 2, (actualSize.height() - 1) / 2);
    m_crossHair = generateCrossHair(screenRes, actualSize);

    if (parent == nullptr) {
        setWindowOpacity(Colors::opacityToFraction(opacity));
    } else {
        auto* effect = new QGraphicsOpacityEffect();
        effect->setOpacity(Colors::opacityToFraction(opacity));
        setGraphicsEffect(effect);
    }

    if (!tooltip.isEmpty()) {
        setToolTip(tooltip);
    }

    connect(Colors::getChangeNotifier(), &Colors::ChangeNotifier::colorChanged, this, &CrossHair::colorChanged);

    m_flashTimer.setTimerType(Qt::PreciseTimer);
    m_flashTimer.setInterval(100);
    connect(&m_flashTimer, &QTimer::timeout, this, &CrossHair::flashHandler);
}

void CrossHair::colorChanged(Colors::Item item, QRgb color) {
    switch (item) {
        case Colors::CrossHairBack:
            setColors(color, Colors::get(Colors::CrossHairHilite), Colors::get(Colors::CrossHairBorder));
            break;
        case Colors::CrossHairHilite:
            setColors(Colors::get(Colors::CrossHairBack), color, Colors::get(Colors::CrossHairBorder));
            break;
        case Colors::CrossHairBorder:
            setColors(Colors::get(Colors::CrossHairBack), Colors::get(Colors::CrossHairHilite), color);
            break;
        case Colors::CrossHairOpacity:
            setOpacity(Colors::opacityToPercent(color));
            break;
        default:
            break;
    }
}

void CrossHair::setColors(QRgb background, QRgb hilite, QRgb border) {
    m_backgroundBrush.setColor(background);
    m_hiliteBrush.setColor(hilite);
    m_hilitePen.setColor(hilite);
    m_borderPen.setColor(border);

    repaint();
}

void CrossHair::setOpacity(int opacityPercent) {
    if (parent() == nullptr) {
        setWindowOpacity(opacityPercent / 100.0);
    } else {
        auto* effect = dynamic_cast<QGraphicsOpacityEffect*>(graphicsEffect());
        effect->setOpacity(opacityPercent / 100.0);
    }
}

void CrossHair::setPosition(const QPoint &center) {
    const QPoint topLeft = center - m_centerPosition;
    move(topLeft);
    emit moved(*this, m_id, center);
}

void CrossHair::flash(int flashCount) {
    m_hilite = false;
    m_flashCountDown = flashCount;
    m_flashTimer.start();
}

void CrossHair::strobe() {
    flash(k_strobeCount);
}

void CrossHair::flashHandler() {
    m_flashCountDown--;
    if (m_flashCountDown < 0) {
        m_flashTimer.stop();
        m_hilite = false;
    } else {
        m_hilite = !m_hilite;
    }

    repaint();
}

void CrossHair::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    painter.fillRect(rect(), m_pointerOver ? m_hiliteBrush : m_backgroundBrush);

    const QPen& outlinePen = m_hilite ? m_hilitePen : m_borderPen;
    painter.strokePath(m_crossHair, outlinePen);
}

void CrossHair::resizeEvent(QResizeEvent*) {
    const QRegion maskedRegion(m_crossHair.toFillPolygon().toPolygon());
    setMask(maskedRegion);
}

void CrossHair::enterEvent(QEnterEvent* event) {
    m_pointerOver = true;

    repaint();

    const QPoint center = findCenter(event->position().toPoint());
    emit entered(*this, m_id, center, event->modifiers());
}

void CrossHair::leaveEvent(QEvent*) {
    m_pointerOver = false;

    repaint();

    emit departed(*this, m_id);
}

void CrossHair::mousePressEvent(QMouseEvent* event) {
    m_initialGrabPosition = event->position().toPoint();

    const QPoint center = findCenter(event->position().toPoint());
    emit selected(*this, m_id, center, event->modifiers());
}

void CrossHair::mouseReleaseEvent(QMouseEvent *event) {
    const QPoint center = findCenter(event->position().toPoint());
    emit deselected(*this, m_id, center, event->modifiers());
}

void CrossHair::mouseMoveEvent(QMouseEvent* event) {
    const QPoint center = findCenter(event->position().toPoint());
    emit dragged(*this, m_id, center, event->modifiers());
}

QPoint CrossHair::findCenter(const QPoint& point) const {
    const QPoint center(point - (m_initialGrabPosition - m_centerPosition));
    const QPoint globalCenter(mapToGlobal(center));
    return m_screenProvider.constrainPosition(globalCenter);
}

QPainterPath CrossHair::generateCrossHair(const QSizeF& screenRes, const QSize& size) const {
    QSize actualPetalWidth = m_unitsProvider.convertToPixels(InchesId, screenRes, k_petalWidth, k_petalWidthMin);
    actualPetalWidth.rwidth() = MathUtils::makeOddUp(actualPetalWidth.width());       // Must be odd
    actualPetalWidth.rheight() = MathUtils::makeOddUp(actualPetalWidth.height());

    QPainterPath path;

    Plotter::plotCrosshair(size, actualPetalWidth, k_centerOffset, [&path](int x, int y, int width, int height) {
        path.addRect(x, y, width, height);
    });

    return path.simplified();
}
