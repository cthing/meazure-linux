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
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/graphics/CrossHair.h>
#include <QObject>


class RadioTool : public Tool {

    Q_OBJECT

public:
    explicit RadioTool(const ScreenInfoProvider& screenInfo, const UnitsProvider& unitsProvider,
                       QObject* parent = nullptr);

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

    /// Visually flashes the tool. Typically, if a tool uses crosshairs, their borders are cycled a number of times
    /// between their hilite color and their normal color. Flashing the tool helps the user find it on the
    /// screen. This base class implementation does nothing.
    ///
    virtual void flash();

    /// Visually strobes the tool. Typically, if a tool uses crosshairs, their borders are cycled once between a
    /// hilite color and their normal. In addition, if the tool has a data window attached to the crosshairs, their
    /// text is flashed. Examples of when strobing the tool is done is to visually confirm region capture and position
    /// recording. This base class implementation does nothing.
    ///
    virtual void strobe();

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
    static constexpr double k_crosshairOffset { CrossHair::getDefaultSize() / 2.0 + 0.05 };
};
