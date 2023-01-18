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

#include "Units.h"
#include "CustomUnits.h"
#include <QString>
#include <QSize>
#include <QSizeF>
#include <QPoint>
#include <QPointF>


/// Interface for units related information.
///
struct UnitsProvider {

    UnitsProvider() = default;
    virtual ~UnitsProvider() = default;

    UnitsProvider(const UnitsProvider&) = delete;
    UnitsProvider(UnitsProvider&&) = delete;
    UnitsProvider& operator=(const UnitsProvider&) = delete;

    /// Returns the identifier for the current linear measurement units.
    ///
    /// @return Identifier for the current linear measurement units.
    ///
    [[nodiscard]] virtual LinearUnitsId getLinearUnitsId() const = 0;

    /// Returns the current linear measurement units.
    ///
    /// @return Current linear units.
    ///
    [[nodiscard]] virtual LinearUnits* getLinearUnits() const = 0;

    /// Returns the linear measurement units object corresponding to the specified identifier.
    ///
    /// @param[in] unitsId Units identifier
    /// @return Units object corresponding to the specified identifier.
    ///
    [[nodiscard]] virtual LinearUnits* getLinearUnits(LinearUnitsId unitsId) const = 0;

    /// Returns the linear measurement units object corresponding to the specified identifier string (e.g. "px").
    ///
    /// @param[in] unitsStr Linear units identifier string.
    ///
    /// @return Linear measurement units object.
    ///
    [[nodiscard]] virtual LinearUnits* getLinearUnits(const QString& unitsStr) const = 0;

    /// Returns the identifier for the current angular measurement units.
    ///
    /// @return Identifier for the current angular measurement units.
    ///
    [[nodiscard]] virtual AngularUnitsId getAngularUnitsId() const = 0;

    /// Returns the current angular measurement units.
    ///
    /// @return Current angular units.
    ///
    [[nodiscard]] virtual AngularUnits* getAngularUnits() const = 0;

    /// Returns the angular measurement units object corresponding to the specified identifier.
    ///
    /// @param[in] unitsId Units identifier
    /// @return Units object corresponding to the specified identifier.
    ///
    [[nodiscard]] virtual AngularUnits* getAngularUnits(AngularUnitsId unitsId) const = 0;

    /// Returns the angular measurement units object corresponding to the specified identifier string (e.g. "deg").
    ///
    /// @param[in] unitsStr Angular units identifier string.
    ///
    /// @return Angular measurement units object.
    ///
    [[nodiscard]] virtual AngularUnits* getAngularUnits(const QString& unitsStr) const = 0;

    /// Returns the user defined units object. This is not necessarily the current measurement units object.
    ///
    /// @return User defined measurement units object.
    ///
    [[nodiscard]] virtual CustomUnits* getCustomUnits() = 0;

    /// Returns the user defined units object. This is not necessarily the current measurement units object.
    ///
    /// @return User defined measurement units object.
    ///
    [[nodiscard]] virtual const CustomUnits* getCustomUnits() const = 0;

    /// Returns the orientation of the y-axis.
    ///
    /// @return If true is returned, the origin of the coordinate system is located at the lower left of the
    ///         primary screen, and the positive y-axis is pointing upward. If false is returned, the origin is
    ///         located at the upper left corner and the positive y-axis is pointing down. Note that the actual
    ///         location of the origin is determined by the setOrigin method.
    ///
    [[nodiscard]] virtual bool isInvertY() const = 0;

    /// Returns the location of the origin of the coordinate system.
    ///
    /// @return Location of the origin of the coordinate system, in pixels.
    ///
    [[nodiscard]] virtual QPoint getOrigin() const = 0;

    /// Uses the current linear measurement units to convert the two specified points from pixels to the current units,
    /// and then calculates the width and height of the rectangle formed by the two points.
    ///
    /// @param[in] p1 One point defining the rectangle, in pixels.
    /// @param[in] p2 The other point defining the rectangle, in pixels.
    ///
    /// @return The width and height of the rectangle formed by the two specified points converted to the current
    ///         linear measurement units. Regardless of the relative locations of the two points, the width and
    ///         height are always positive. In addition, the width and height are inclusive (e.g. p1.x = 1, p2.x = 3,
    ///         width = p2.x - p1.x + 1 = 3).
    ///
    [[nodiscard]] virtual QSizeF getWidthHeight(const QPoint& p1, const QPoint& p2) const = 0;

    /// Converts the specified angle to the current angular units and formats the result for display.
    ///
    /// @param[in] angle Angle to be converted and displayed, in radians.
    /// @return String containing the converted and formatted angle data.
    ///
    [[nodiscard]] virtual QString formatConvertAngle(double angle) const = 0;

    /// Formats the specified linear measurement value for display.
    ///
    /// @param[in] id Identifies the linear measurement for use in determining the display precision.
    /// @param[in] value Linear measurement value to format, in current units.
    ///
    /// @return String containing the formatted measurement data.
    ///
    [[nodiscard]] virtual QString format(LinearMeasurementId id, double value) const = 0;

    /// Formats the specified angular measurement value for display.
    ///
    /// @param[in] id Identifies the angular measurement for use in determining the display precision.
    /// @param[in] value Angular measurement value to format, in current units.
    ///
    /// @return String containing the formatted measurement data.
    ///
    [[nodiscard]] virtual QString format(AngularMeasurementId id, double value) const = 0;

    /// Converts the specified coordinate from pixels to the desired units. This conversion takes into account the
    /// location of the origin and the orientation of the y-axis.
    ///
    /// @param[in] pos Coordinates in pixels to convert to the desired units.
    ///
    /// @return Coordinates converted from pixels to the desired units. The conversion takes into account the
    ///         location of the origin and the orientation of the y-axis.
    ///
    [[nodiscard]] virtual QPointF convertCoord(const QPoint& pos) const = 0;

    /// Converts the specified position from pixels to the desired units. This conversion does not take into account
    /// the location of the origin nor does it compensate for the orientation of the y-axis.
    ///
    /// @param[in] pos Position in pixels to convert to the desired units.
    ///
    /// @return Position converted from pixels to the desired units. The conversion does not take into account the
    ///         location of the origin nor does it compensate for the orientation of the y-axis.
    ///
    [[nodiscard]] virtual QPointF convertPos(const QPoint& pos) const = 0;

    /// Converts from the current units to pixels. The conversion takes into account the location of the origin and
    /// the orientation of the y-axis. The conversion is performed for the specified coordinate located on the
    /// specified axis.
    ///
    /// @param[in] dir Axis on which the conversion should take place.
    /// @param[in] wnd Window used to determine the screen resolution.
    /// @param[in] pos Coordinate to convert to pixels.
    ///
    /// @return Coordinate converted from the current units to pixels. The conversion takes into account the location
    ///         of the origin and the orientation of the y-axis.
    ///
    [[nodiscard]] virtual double unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos) const = 0;

    /// Converts from the current units to pixels. The conversion takes into account the location of the origin
    /// and the orientation of the y-axis.
    ///
    /// @param[in] pos Coordinates to convert to pixels.
    ///
    /// @return Coordinates converted from the current units to pixels. The conversion takes into account the
    ///         location of the origin and the orientation of the y-axis.
    ///
    [[nodiscard]] virtual QPoint unconvertCoord(const QPointF& pos) const = 0;

    /// Converts from the current units to pixels. The conversion does not take into account the location of the
    /// origin nor the orientation of the y-axis.
    ///
    /// @param[in] pos Position to convert to pixels.
    ///
    /// @return Position converted from the current units to pixels. The conversion does not take into account the
    ///         location of the origin nor the orientation of the y-axis.
    ///
    [[nodiscard]] virtual QPoint unconvertPos(const QPointF& pos) const = 0;

    /// Converts the specified resolution in pixels/inch to the desired units.
    ///
    /// @param[in] res Resolution in pixels/inch to convert to the desired units.
    ///
    /// @return Resolution converted from pixels/inch to the desired units.
    ///
    [[nodiscard]] virtual QSizeF convertRes(const QSizeF& res) const = 0;

    /// Converts the specified value from the specified units to pixels. A minimum pixel value is specified in case
    /// the resolution is such that the conversion to pixels results in a value that is too small.
    ///
    /// @param[in] id Linear units identifier.
    /// @param[in] res Screen resolution to use to perform the conversion.
    /// @param[in] value Value to be converted to pixels.
    /// @param[in] minPixels If the converted pixel value in less than this minimum value, the minimum value
    ///         is returned.
    ///
    /// @return X and Y pixel values.
    ///
    [[nodiscard]] virtual QSize convertToPixels(LinearUnitsId id, const QSizeF& res, double value,
                                                int minPixels) const = 0;

    /// Converts the specified angle value from its native radians to the desired units.
    ///
    /// @param[in] angle Value to be converted.
    ///
    /// @return Angle value converted to the desired units.
    ///
    [[nodiscard]] virtual double convertAngle(double angle) const = 0;

    /// Returns the number of minor ticks to display before a major tick mark is displayed on the measurement rulers.
    ///
    /// @return Number of minor tick marks to display before displaying a major tick mark.
    ///
    [[nodiscard]] virtual int getMajorTickCount() const = 0;

    /// Returns the increment between minor tick marks on the measurement rulers.
    ///
    /// @param[in] rect Rectangle in screen coordinates, used to determine the current screen resolution.
    ///
    /// @return Increment for the minor tick marks, in the current units.
    ///
    [[nodiscard]] virtual QSizeF getMinorTickIncr(const QRect& rect) const = 0;
};
