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

#include "Magnifier.h"
#include <QPainter>
#include <QBrush>
#include <QColor>


Magnifier::Magnifier(const ScreenInfoProvider* screenInfo, const ToolMgr* toolMgr) :
        m_screenInfo(screenInfo),
        m_gridPen(QBrush(QColor(0, 0, 0)), 1),
        m_centerMarkerPen(QBrush(QColor(255, 0, 0)), 1) {
    setFixedSize(k_size, k_size);

    setStatusTip(tr("Magnified active position"));
    setWhatsThis(tr("Magnifies the area around the active position"));

    m_grabTimer.setTimerType(Qt::PreciseTimer);
    m_grabTimer.setInterval(k_updateRate);
    connect(&m_grabTimer, &QTimer::timeout, this, &Magnifier::periodicGrab);

    connect(toolMgr, &ToolMgr::activePositionChanged, this, &Magnifier::setCurPos);

    setZoom(m_zoomIndex);
    setGrid(m_showGrid);

    m_grabTimer.start();
}

void Magnifier::writeConfig(Config& config) const {
    config.writeInt("ZoomIndex", m_zoomIndex);
    config.writeBool("MagGrid", m_showGrid);
}

void Magnifier::readConfig(const Config& config) {
    setZoom(config.readInt("ZoomIndex", m_zoomIndex));
    setGrid(config.readBool("MagGrid", m_showGrid));
}

void Magnifier::zoomIn() {
    setZoom(m_zoomIndex + 1);
}

void Magnifier::zoomOut() {
    setZoom(m_zoomIndex - 1);
}

void Magnifier::setZoom(int zoomIndex) {
    if (zoomIndex >= 0 && zoomIndex < static_cast<int>(k_zoomFactors.size())) {
        m_zoomIndex = zoomIndex;

        const int zoomFactor = k_zoomFactors[zoomIndex];
        const int origin = k_size * (1 - zoomFactor) / 2;
        m_zoomTransform = QTransform::fromTranslate(origin, origin).scale(zoomFactor, zoomFactor);

        const int pixelWidth = zoomFactor + 1;

        const int markerCoord = (k_size - zoomFactor) / 2;
        m_centerMarker = QRect(markerCoord, markerCoord, pixelWidth, pixelWidth);

        m_gridLines.clear();
        const int coordMax = k_size - 1;
        for (int x = markerCoord; x > 0; x -= pixelWidth) {
            m_gridLines.emplace_back(x, 0, x, coordMax);
        }
        for (int x = markerCoord + pixelWidth; x < k_size; x += pixelWidth) {
            m_gridLines.emplace_back(x, 0, x, coordMax);
        }
        for (int y = markerCoord; y > 0; y -= pixelWidth) {
            m_gridLines.emplace_back(0, y, coordMax, y);
        }
        for (int y = markerCoord + pixelWidth; y < k_size; y += pixelWidth) {
            m_gridLines.emplace_back(0, y, coordMax, y);
        }

        repaint();

        emit zoomChanged(zoomIndex);
    }
}

void Magnifier::setFreeze(bool frozen) {
    if (frozen) {
        m_grabTimer.stop();
    } else {
        m_grabTimer.start();
    }

    emit freezeChanged(frozen);
}

void Magnifier::setGrid(bool show) {
    m_showGrid = show;
    repaint();

    emit gridChanged(show);
}

void Magnifier::setCurPos(QPoint rawPos) {
    m_curPos = rawPos;
}

void Magnifier::periodicGrab() {
    grabScreen();
    repaint();
}

void Magnifier::grabScreen() {
    const int c = k_size / 2;
    const int x = m_curPos.x() - c;
    const int y = m_curPos.y() - c;
    m_image = m_screenInfo->grabScreen(x, y, k_size, k_size);

    const QRgb color = m_image.pixel(c, c);
    if (color != m_currentColor) {
        m_currentColor = color;
        emit currentColorChanged(color);
    }
}

void Magnifier::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    // Image
    painter.save();
    painter.setTransform(m_zoomTransform);
    painter.drawImage(0, 0, m_image);
    painter.restore();

    // Grid
    if (m_showGrid && (m_zoomIndex >= k_gridMinIndex)) {
        painter.setPen(m_gridPen);
        painter.drawLines(m_gridLines.data(), static_cast<int>(m_gridLines.size()));
    }

    // Center marker
    painter.setPen(m_centerMarkerPen);
    painter.drawRect(m_centerMarker);

    // Border
    painter.setPen(m_gridPen);
    painter.drawRect(0, 0, k_size - 1, k_size - 1);
}
