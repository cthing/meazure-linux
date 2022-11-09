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
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <QPoint>
#include <QPen>


/// A circle graphical element. The circle is positioned by specifying the center and it is sized by specifying a
/// point on the perimeter.
///
class Circle : public Graphic {

    Q_OBJECT

public:
    /// Constructs a circle.
    ///
    /// @param[in] screenInfoProvider Information about the display screens
    /// @param[in] unitsProvider Measurement units
    /// @param[in] gap Gap, in inches, on the perimeter to accommodate a crosshair.
    /// @param[in] parent Parent widget for the line or nullptr for a top level circle
    /// @param[in] lineColor Line foreground color
    ///
    explicit Circle(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                    double gap = 0.0, QWidget* parent = nullptr, QRgb lineColor = Colors::get(Colors::LineFore));

    /// Sets the line color.
    ///
    /// @param[in] color Line foreground color
    ///
    void setColor(QRgb color);

    /// Sets the position of the circle.
    ///
    /// @param[in] centerPoint Location of the center of the circle, in pixels
    /// @param[in] perimeterPoint Location on the perimeter of the circle, in pixels
    ///
    void setPosition(const QPoint& centerPoint, const QPoint& perimeterPoint);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static constexpr int k_lineWidth { 1 };     // Pixels

    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_unitsProvider;
    double m_gap;
    QPen m_pen;
    QPoint m_center;
    QPoint m_perimeter;
    double m_radius { 1.0 };
};