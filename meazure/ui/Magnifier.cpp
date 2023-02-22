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

#include "Magnifier.h"
#include <meazure/utils/MathUtils.h>
#include <QPainter>
#include <QBrush>
#include <QColor>


Magnifier::Magnifier(const ScreenInfoProvider* screenInfo, const ToolMgr* toolMgr) :
        m_screenInfo(screenInfo),
        m_darkGridPen(QBrush(QColor(k_darkGridColor)), 1),
        m_lightGridPen(QBrush(QColor(k_lightGridColor)), 1),
        m_centerMarkerPen(QBrush(QColor(255, 0, 0)), 1) {
    const int screenIdx = m_screenInfo->screenForWindow(this);
    const QSizeF& platformScale = m_screenInfo->getPlatformScale(screenIdx);
    m_width = MathUtils::makeOddDown(qRound(k_size * platformScale.width()));
    m_height = MathUtils::makeOddDown(qRound(k_size * platformScale.height()));

    setFixedSize(m_width, m_height);

    setStatusTip(tr("Magnified active position"));
    setWhatsThis(tr("Magnifies the area around the active position"));

    m_grabTimer.setTimerType(Qt::PreciseTimer);
    m_grabTimer.setInterval(k_updateRate);
    connect(&m_grabTimer, &QTimer::timeout, this, &Magnifier::periodicGrab);

    connect(toolMgr, &ToolMgr::activePositionChanged, this, &Magnifier::setCurPos);

    setZoom(m_zoomIndex);
    setGridType(m_gridType);

    m_grabTimer.start();
}

void Magnifier::writeConfig(Config& config) const {
    config.writeInt("ZoomIndex", m_zoomIndex);
    config.writeInt("MagGridType", m_gridType);
}

void Magnifier::readConfig(const Config& config) {
    setZoom(config.readInt("ZoomIndex", m_zoomIndex));
    setGridType(static_cast<GridType>(config.readInt("MagGridType", m_gridType)));
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
        const int originX = m_width * (1 - zoomFactor) / 2;
        const int originY = m_height * (1 - zoomFactor) / 2;
        m_zoomTransform = QTransform::fromTranslate(originX, originY).scale(zoomFactor, zoomFactor);

        const int markerCoordX = (m_width - zoomFactor) / 2;
        const int markerCoordY = (m_height - zoomFactor) / 2;
        m_centerMarker = QRect(markerCoordX, markerCoordY, zoomFactor, zoomFactor);

        m_gridLines.clear();
        const int coordMaxX = m_width - 1;
        const int coordMaxY = m_height - 1;
        for (int x = markerCoordX; x > 0; x -= zoomFactor) {
            m_gridLines.emplace_back(x, 0, x, coordMaxX);
        }
        for (int x = markerCoordX + zoomFactor; x < m_width; x += zoomFactor) {
            m_gridLines.emplace_back(x, 0, x, coordMaxX);
        }
        for (int y = markerCoordY; y > 0; y -= zoomFactor) {
            m_gridLines.emplace_back(0, y, coordMaxY, y);
        }
        for (int y = markerCoordY + zoomFactor; y < m_height; y += zoomFactor) {
            m_gridLines.emplace_back(0, y, coordMaxY, y);
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

void Magnifier::setGridType(GridType type) {
    m_gridType = type;
    repaint();

    emit gridTypeChanged(type);
}

void Magnifier::setCurPos(QPoint rawPos) {
    m_curPos = rawPos;
}

void Magnifier::periodicGrab() {
    grabScreen();
    repaint();
}

void Magnifier::grabScreen() {
    const int cx = m_width / 2;
    const int cy = m_height / 2;
    const int x = m_curPos.x() - cx;
    const int y = m_curPos.y() - cy;
    m_image = m_screenInfo->grabScreen(x, y, m_width, m_height);

    const QRgb color = m_image.pixel(cx, cy);
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
    if (m_gridType != None && (m_zoomIndex >= k_gridMinIndex)) {
        painter.setPen(m_gridType == Dark ? m_darkGridPen : m_lightGridPen);
        painter.drawLines(m_gridLines.data(), static_cast<int>(m_gridLines.size()));
    }

    // Center marker
    painter.setPen(m_centerMarkerPen);
    painter.drawRect(m_centerMarker);

    // Border
    painter.setPen(m_darkGridPen);
    painter.drawRect(0, 0, m_width - 1, m_height - 1);
}
