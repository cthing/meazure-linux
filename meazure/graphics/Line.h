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


/// A line graphical element. The line is used by many tools including the Line and Grid tools. The line is positioned
/// by specifying the location of its two end points.
///
class Line : public Graphic {

    Q_OBJECT

public:
    /// Constructs a line.
    ///
    /// @param[in] screenInfoProvider Information about the display screens
    /// @param[in] unitsProvider Measurement units
    /// @param[in] offset Distance, in inches, to shrink line from start and end points. The typical use for this
    ///     parameter is in the Line tool. Shrinking the line allows the contents of the end points to be clear
    ///     for viewing and measuring.
    /// @param[in] parent Parent widget for the line or nullptr for a top level line
    /// @param[in] lineColor Line foreground color
    ///
    explicit Line(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                  double offset = 0.0, QWidget* parent = nullptr, QRgb lineColor = Colors::get(Colors::LineFore));

    /// Sets the line color.
    ///
    /// @param[in] color Line foreground color
    ///
    void setColor(QRgb color);

    /// Sets the position of the line.
    ///
    /// @param[in] startPoint Location of the start of the line, in pixels
    /// @param[in] endPoint Location of the end of the line, in pixels
    ///
    void setPosition(const QPoint& startPoint, const QPoint& endPoint);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static constexpr int k_lineWidth { 2 };     // Pixels

    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_unitsProvider;
    double m_offset;
    QColor m_color;
    QPoint m_start;
    QPoint m_end;
};