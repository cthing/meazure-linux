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
#include <meazure/units/Units.h>
#include <QObject>
#include <QPen>
#include <QRectF>
#include <QSizeF>
#include <QTransform>


/// A grid graphical element. Draws a rectangular grid with configurable size, grid spacing and rotation. The grid
/// spacing can be specified in any of the supported measurement units.
///
class Grid : public Graphic {

    Q_OBJECT

public:
    static constexpr int k_minSpacing { 5 };       // Minimum grid spacing, pixels

    /// Constructs a grid.
    ///
    /// @param[in] screenInfoProvider Information about the display screens
    /// @param[in] unitsProvider Measurement units
    /// @param[in] parent Parent widget for the grid or nullptr for a top level grid
    /// @param[in] lineColor Grid line foreground color
    /// @param[in] lineWidth Width of line, in pixels
    ///
    explicit Grid(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                  QWidget* parent = nullptr, QRgb lineColor = Colors::get(Colors::LineFore),
                  int lineWidth = Dimensions::getLineWidth());

    /// Sets the rectangle line color.
    ///
    /// @param[in] color Line foreground color
    ///
    void setColor(QRgb color);

    /// Sets the position of the upper left corner of the grid, its width, height, and rotation angle. The width and
    /// height of the grid are independent of rotation (i.e. it is not the width and height of the bounding rectangle).
    ///
    /// @param[in] x Position of the left corner of the grid, in pixels. Note that the actual left position will differ
    ///         from that specified if rotation is specified.
    /// @param[in] y Position of the top corner of the grid, in pixel. Note that the actual
    ///         top position will differ from that specified if rotation is specified.
    /// @param[in] width Width of the grid, in pixels. This is the width of the grid regardless of rotation
    ///         (i.e. it is not the width of the bounding rectangle).
    /// @param[in] height Height of the grid, in pixels. This is the height of the grid regardless of rotation
    ///         (i.e. it is not the height of the bounding rectangle).
    /// @param[in] angle Rotation angle of the grid, in degrees. A positive angle rotates the grid counterclockwise.
    ///         Rotation is performed around the center of the grid.
    ///
    void setPosition(int x, int y, int width, int height, int angle);

    /// Sets the vertical and horizontal space between grid lines.
    ///
    /// @param[in] hSpacing Horizontal grid line spacing, expressed in the units specified by setUnits
    /// @param[in] vSpacing Vertical grid line spacing, expressed in the units specified by setUnits
    ///
    void setSpacing(double hSpacing, double vSpacing);

    /// Specifies the units for the grid spacing.
    ///
    /// @param[in] units Units for the grid spacing
    ///
    void setUnits(LinearUnitsId units);

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
    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_unitsProvider;
    QPen m_pen;
    QRect m_gridRect;                       ///< Grid rectangle positioned for rotation, in pixels
    QTransform m_gridTransform;             ///< Rotational transform for the grid
    int m_angle { 0 };                      ///< Degrees
    double m_hSpacing { 50 };               ///< Horizontal line spacing, in m_units
    double m_vSpacing { 50 };               ///< Vertical line spacing, in m_units
    LinearUnitsId m_units { PixelsId };     ///< Units for the grid spacing
};
