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

#include "Grid.h"
#include <meazure/utils/MathUtils.h>
#include <meazure/utils/Geometry.h>
#include <QTransform>
#include <QPainter>
#include <QLine>
#include <vector>

Grid::Grid(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
           QWidget* parent, QRgb lineColor, int lineWidth) :
        Graphic(parent),
        m_screenInfo(screenInfoProvider),
        m_unitsProvider(unitsProvider),
        m_pen(QBrush(lineColor), lineWidth),
        m_gridRect(0, 0, 100, 100) {
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::WindowTransparentForInput);
}

void Grid::setColor(QRgb color) {
    m_pen.setColor(color);
    repaint();
}

void Grid::setLineWidth(int width) {
    m_pen.setWidth(width);
    repaint();
}

void Grid::setPosition(int x, int y, int width, int height, int angle) {
    m_angle = angle;

    const QRect requestedDimensions(x, y, width, height);
    const QPoint requestedCenter = requestedDimensions.center();

    const QTransform boundingTransform = Geometry::rotateAround(m_angle, requestedCenter);
    const QRect boundingRect = boundingTransform.mapRect(requestedDimensions);

    setGeometry(boundingRect);

    m_gridRect.setRect((boundingRect.width() - requestedDimensions.width()) / 2,
                       (boundingRect.height() - requestedDimensions.height()) / 2,
                       requestedDimensions.width(),
                       requestedDimensions.height());

    m_gridTransform = Geometry::rotateAround(m_angle, m_gridRect.center());

}

void Grid::setSpacing(double hSpacing, double vSpacing) {
    m_hSpacing = hSpacing;
    m_vSpacing = vSpacing;
    repaint();
}

void Grid::setUnits(LinearUnitsId units) {
    m_units = units;
    repaint();
}

void Grid::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(m_pen);

    for (int idx = 0; idx < m_screenInfo.getNumScreens(); idx++) {
        const QRect intersectRect = m_screenInfo.getScreenRect(idx).intersected(geometry());
        if (!intersectRect.isEmpty()) {
            int hPixelSpacing;      // NOLINT(cppcoreguidelines-init-variables)
            int vPixelSpacing;      // NOLINT(cppcoreguidelines-init-variables)
            if (m_units == PixelsId) {
                hPixelSpacing = qRound(m_hSpacing);
                vPixelSpacing = static_cast<int>(m_vSpacing);
            } else {
                const double angleFrac = (m_angle % 90) / 90.0;
                const QSizeF res = m_screenInfo.getScreenRes(idx);
                const QSizeF effectiveRes(MathUtils::linearInterpolate(res.width(), res.height(), angleFrac),
                                          MathUtils::linearInterpolate(res.height(), res.width(), angleFrac));
                const QSizeF unitsRes = m_unitsProvider.getLinearUnits(m_units)->convertRes(effectiveRes);
                hPixelSpacing = qRound(m_hSpacing * unitsRes.width());
                vPixelSpacing = qRound(m_vSpacing * unitsRes.height());
            }

            if (hPixelSpacing < k_minSpacing) {
                hPixelSpacing = k_minSpacing;
            }
            if (vPixelSpacing < k_minSpacing) {
                vPixelSpacing = k_minSpacing;
            }

            std::vector<QLine> lines;
            for (int x = m_gridRect.left(); x <= m_gridRect.right(); x += hPixelSpacing) {
                lines.emplace_back(x, m_gridRect.bottom(), x, m_gridRect.top());
            }
            for (int y = m_gridRect.top(); y <= m_gridRect.bottom(); y += vPixelSpacing) {
                lines.emplace_back(m_gridRect.left(), y, m_gridRect.right(), y);
            }

            const QRect clipRect(intersectRect.topLeft() - geometry().topLeft(), intersectRect.size());
            painter.save();
            painter.setClipRect(clipRect);
            painter.setTransform(m_gridTransform);
            painter.drawLines(lines.data(), static_cast<int>(lines.size()));
            painter.restore();
        }
    }
}
