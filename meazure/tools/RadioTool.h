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

#include "Tool.h"
#include "RadioToolTraits.h"
#include <meazure/units/UnitsProvider.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/graphics/Crosshair.h>
#include <QObject>
#include <QImage>
#include <QPointF>


class RadioTool : public Tool {

    Q_OBJECT

public:
    explicit RadioTool(const ScreenInfo& screenInfo, const UnitsProvider& unitsProvider, QObject* parent = nullptr);

    /// Provides the flags that describe which measurements are provided by the tool and which ones are settable.
    ///
    /// @return Flags describing the measurements provided by the tool.
    ///
    [[nodiscard]] virtual RadioToolTraits getTraits() const = 0;

    /// Obtains brief instructions for using the tool. Typically, these instructions are displayed on the status
    /// bar.
    ///
    /// @return Tool usage instructions.
    ///
    [[nodiscard]] virtual QString getInstructions() const = 0;

    [[nodiscard]] bool isRadioTool() override {
        return true;
    }

    /// Sets whether the tool crosshairs are visible.
    ///
    /// @param[in] enable true to make the crosshairs visible
    ///
    virtual void setCrosshairsEnabled(bool enable);

    /// Indicates whether the tool can grab a rectangular region of the screen. Typically, any tool that provides
    /// a position, width and height measurement, can grab the corresponding region of the screen.
    ///
    /// @return true if the tool can perform a grabRegion operation.
    ///
    [[nodiscard]] virtual bool canGrabRegion() const {
        return isEnabled() && ((getTraits() & WHAvailable) != 0);
    }

    /// If the tool can grab a region of the screen, this method grabs that region into a pixmap.
    ///
    /// @return Image containing the contents of the tools region. If the tool cannot perform a grab, a null
    ///     QImage is returned (i.e. isNull() is true).
    ///
    [[nodiscard]] virtual QImage grabRegion() const {
        return {};
    }

    /// Visually flashes the tool. Typically, if a tool uses crosshairs, their borders are cycled a number of times
    /// between their highlight color and their normal color. Flashing the tool helps the user find it on the
    /// screen. This base class implementation does nothing.
    ///
    virtual void flash();

    /// Visually strobes the tool. Typically, if a tool uses crosshairs, their borders are cycled once between a
    /// highlight color and their normal. In addition, if the tool has a data window attached to the crosshairs, their
    /// text is flashed. Examples of when strobing the tool is done is to visually confirm region capture and position
    /// recording. This base class implementation does nothing.
    ///
    virtual void strobe();

    /// Indicates whether the derived tool uses crosshairs.
    ///
    /// @return This base class implementation always returns <b>true</b>.
    ///
    [[nodiscard]] virtual bool hasCrosshairs() const;

    /// Sets the radio tool's position 1 coordinates.
    ///
    /// @param[in] position Coordinates in the current units, origin and y-axis direction.
    ///
    virtual void setXY1Position(const QPointF& position);

    /// Sets the radio tool's position 1 x coordinate.
    ///
    /// @param[in] x X coordinate in the current units, origin and y-axis direction.
    ///
    virtual void setX1Position(double x);

    /// Sets the radio tool's position 1 y coordinate.
    ///
    /// @param[in] y Y coordinate in the current units, origin and y-axis direction.
    ///
    virtual void setY1Position(double y);

    /// Sets the radio tool's position 2 coordinates.
    ///
    /// @param[in] position Coordinates in the current units, origin and y-axis direction.
    ///
    virtual void setXY2Position(const QPointF& position);

    /// Sets the radio tool's position 2 x coordinate.
    ///
    /// @param[in] x X coordinate in the current units, origin and y-axis direction.
    ///
    virtual void setX2Position(double x);

    /// Sets the radio tool's position 2 y coordinate.
    ///
    /// @param[in] y Y coordinate in the current units, origin and y-axis direction.
    ///
    virtual void setY2Position(double y);

    /// Sets the radio tool's vertex position coordinates.
    ///
    /// @param[in] position Coordinates in the current units, origin and y-axis direction.
    ///
    virtual void setXYVPosition(const QPointF& position);

    /// Sets the radio tool's vertex position x coordinate.
    ///
    /// @param[in] x X coordinate in the current units, origin and y-axis direction.
    ///
    virtual void setXVPosition(double x);

    /// Sets the radio tool's vertex position y coordinate.
    ///
    /// @param[in] y Y coordinate in the current units, origin and y-axis direction.
    ///
    virtual void setYVPosition(double y);

    /// Increments or decrements the radio tool's position 1 x coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the x coordinate
    ///
    virtual void stepX1Position(int numSteps);

    /// Increments or decrements the radio tool's position 1 y coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the y coordinate
    ///
    virtual void stepY1Position(int numSteps);

    /// Increments or decrements the radio tool's position 2 x coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the x coordinate
    ///
    virtual void stepX2Position(int numSteps);

    /// Increments or decrements the radio tool's position 2 y coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the y coordinate
    ///
    virtual void stepY2Position(int numSteps);

    /// Increments or decrements the radio tool's vertex position x coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the x coordinate
    ///
    virtual void stepXVPosition(int numSteps);

    /// Increments or decrements the radio tool's vertex position y coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the y coordinate
    ///
    virtual void stepYVPosition(int numSteps);

protected:
    /// Offset, in inches, from the a given point so that there will be a margin around crosshairs.
    /// When drawing lines to crosshairs, this offset will ensure that there is a bit of a margin
    /// around the crosshair such that the line will come close to but not touch the crosshair.
    ///
    static constexpr double k_crosshairOffset { Crosshair::getDefaultSize() / 2.0 + 0.07 };
};
