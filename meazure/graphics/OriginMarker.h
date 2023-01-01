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

#include "Graphic.h"
#include "Colors.h"
#include "Dimensions.h"
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <QObject>
#include <QPen>
#include <QPoint>
#include <QSize>


/// Graphic to indicate the origin of a coordinate system and the positive direction for the x and y axes.
///
class OriginMarker : public Graphic {

    Q_OBJECT

public:
    explicit OriginMarker(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                          QWidget* parent = nullptr, QRgb lineColor = Colors::get(Colors::LineFore),
                          int lineWidth = Dimensions::getLineWidth());

    /// Sets the rectangle line color.
    ///
    /// @param[in] color Line foreground color
    ///
    void setColor(QRgb color);

    /// Sets the position of the marker.
    ///
    /// @param[in] origin Location of the origin of the marker, in pixels
    /// @param[in] inverted true if the y-axis is inverted
    ///
    void setPosition(const QPoint& origin, bool inverted);

public slots:
    /// Sets the line width.
    ///
    /// @param[in] width Line width, in pixels
    ///
    void setLineWidth(int width);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void colorChanged(Colors::Item item, QRgb color);

private:
    static constexpr double k_axesLength { 0.25 };      ///< Length of each axis line, inches
    static constexpr int k_axesLengthMin { 10 };        ///< Minimum length of each axis line, pixels

    QPen m_pen;
    QPoint m_origin { 0, 0 };
    bool m_inverted { false };
    QSize m_axisLength { k_axesLengthMin, k_axesLengthMin };
};
