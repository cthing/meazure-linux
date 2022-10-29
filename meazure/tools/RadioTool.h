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
#include <QObject>


class RadioTool : public Tool {

    Q_OBJECT

public:
    explicit RadioTool(const UnitsProvider& unitsProvider, QObject* parent = nullptr);

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
};
