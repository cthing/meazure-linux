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

#pragma once

#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/config/Config.h>
#include <QWidget>
#include <QPoint>
#include <QImage>
#include <QTimer>
#include <QTransform>
#include <QPen>
#include <QLine>
#include <array>
#include <vector>


/// Displays a magnified image of an area of the screen.
///
class Magnifier : public QWidget {

    Q_OBJECT

public:
    using ZoomFactors = std::array<int, 8>;

    Magnifier();

    static constexpr const ZoomFactors& getZoomFactors() {
        return k_zoomFactors;
    }

    void writeConfig(Config& config) const;
    void readConfig(const Config& config);

public slots:
    void setZoom(int zoomIndex);
    void zoomIn();
    void zoomOut();
    void setFreeze(bool frozen);
    void setGrid(bool show);

signals:
    void zoomChanged(int zoomIndex);
    void freezeChanged(bool frozen);
    void gridChanged(bool shown);
    void currentColorChanged(QRgb color);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void setCurPos(QPoint rawPos);
    void periodicGrab();

private:
    static constexpr int k_size { 281 };        ///< Magnifier size, pixels
    static constexpr int k_updateRate { 50 };   ///< Magnifier refresh rate, in milliseconds.
    static constexpr ZoomFactors k_zoomFactors { 1, 2, 3, 4, 6, 8, 16, 32 };
    static constexpr int k_gridMinIndex { 4 };

    void grabScreen();

    const ScreenInfoProvider& m_screenInfo;
    QPoint m_curPos { -1, -1 };
    QImage m_image;
    QPen m_gridPen;
    QPen m_centerMarkerPen;
    QRect m_centerMarker;
    QTimer m_grabTimer;
    int m_zoomIndex { 0 };
    QTransform m_zoomTransform;
    std::vector<QLine> m_gridLines;
    bool m_showGrid { true };
    QRgb m_currentColor { 1 };
};
