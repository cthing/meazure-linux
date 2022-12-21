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

#include "Handle.h"
#include <meazure/utils/MathUtils.h>
#include <QMouseEvent>
#include <QPainter>


Handle::Handle(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
               QWidget *parent, const QString& tooltip, int id, const QRgb backgroundColor, QRgb highlightColor,
               QRgb borderColor, QRgb opacity) :
        Graphic(parent),
        m_screenProvider(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_id(id),
        m_backgroundBrush(backgroundColor),
        m_highlightBrush(highlightColor),
        m_highlightPen(QBrush(highlightColor), k_outlineWidth),
        m_borderPen(QBrush(borderColor), k_outlineWidth) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);

    const QPoint screenCenter = m_screenProvider.getCenter();
    const int screenIndex = m_screenProvider.screenForPoint(screenCenter);
    const QSizeF screenRes = m_screenProvider.getScreenRes(screenIndex);

    QSize actualSize = m_unitsProvider.convertToPixels(InchesId, screenRes, k_size, k_sizeMin);
    actualSize.rwidth() = MathUtils::makeOddUp(actualSize.width());       // Must be odd
    actualSize.rheight() = MathUtils::makeOddUp(actualSize.height());
    setFixedSize(actualSize);

    m_centerPosition = QPoint((actualSize.width() - 1) / 2, (actualSize.height() - 1) / 2);

    setWindowOpacity(Colors::opacityToFraction(opacity));

    if (!tooltip.isEmpty()) {
        setToolTip(tooltip);
    }

    m_flashTimer.setTimerType(Qt::PreciseTimer);
    m_flashTimer.setInterval(100);
    connect(&m_flashTimer, &QTimer::timeout, this, &Handle::flashHandler);
}

void Handle::setColors(QRgb background, QRgb highlight, QRgb border) {
    m_backgroundBrush.setColor(background);
    m_highlightBrush.setColor(highlight);
    m_highlightPen.setColor(highlight);
    m_borderPen.setColor(border);

    repaint();
}

void Handle::setOpacity(int opacityPercent) {
    setWindowOpacity(opacityPercent / 100.0);
}

void Handle::setPosition(const QPoint &center) {
    const QPoint topLeft = center - m_centerPosition;
    move(topLeft);
    emit moved(*this, m_id, center);
}

void Handle::flash(int flashCount) {
    m_highlight = false;
    m_flashCountDown = flashCount;
    m_flashTimer.start();
}

void Handle::strobe() {
    flash(k_strobeCount);
}

void Handle::flashHandler() {
    m_flashCountDown--;
    if (m_flashCountDown < 0) {
        m_flashTimer.stop();
        m_highlight = false;
    } else {
        m_highlight = !m_highlight;
    }

    repaint();
}

void Handle::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(m_highlight ? m_highlightPen : m_borderPen);
    painter.setBrush(m_pointerOver ? m_highlightBrush : m_backgroundBrush);
    painter.drawEllipse(1, 1, width() - 2, height() - 2);
}

void Handle::enterEvent(QEnterEvent* event) {
    m_pointerOver = true;

    repaint();

    const QPoint center = findCenter(event->position().toPoint());
    emit entered(*this, m_id, center, event->modifiers());
}

void Handle::leaveEvent(QEvent*) {
    m_pointerOver = false;

    repaint();

    emit departed(*this, m_id);
}

void Handle::mousePressEvent(QMouseEvent* event) {
    m_initialGrabPosition = event->position().toPoint();

    const QPoint center = findCenter(event->position().toPoint());
    emit selected(*this, m_id, center, event->modifiers());
}

void Handle::mouseReleaseEvent(QMouseEvent *event) {
    const QPoint center = findCenter(event->position().toPoint());
    emit deselected(*this, m_id, center, event->modifiers());
}

void Handle::mouseMoveEvent(QMouseEvent* event) {
    const QPoint center = findCenter(event->position().toPoint());
    emit dragged(*this, m_id, center, event->modifiers());
}

QPoint Handle::findCenter(const QPoint& point) const {
    const QPoint center(point - (m_initialGrabPosition - m_centerPosition));
    const QPoint globalCenter(mapToGlobal(center));
    return m_screenProvider.constrainPosition(globalCenter);
}
