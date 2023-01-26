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

#include "AbstractDataField.h"
#include <QSpinBox>


/// Text field for presenting an integer measurement value.
///
class IntegerDataField : public AbstractDataField<QSpinBox, int> {

    Q_OBJECT

public:
    /// Constructs a data field.
    ///
    /// @param[in] charWidth Width of the field in number of visible characters
    /// @param[in] showButtons Specify true to show the up/down arrow buttons
    /// @param[in] readOnly Configures the field to be read only
    /// @param[in] nativeStepHandling Specifies whether the data field should use its internal step handling for
    ///     arrow buttons and arrow keys
    /// @param[in] parent Parent for the widget
    ///
    IntegerDataField(int charWidth, bool showButtons, bool readOnly = false, bool nativeStepHandling = true,
                     QWidget *parent = nullptr);

signals:
    /// Emitted to indicate that the user has either pressed an up/down arrow button, up/down arrow key, or page
    /// up/down key while the field has keyboard focus.
    ///
    /// @param[in] steps The number of steps to increment or decrement the current tool position. The number of steps
    ///     is greater for the page up/down keys. A positive value indicates an up arrow or key and a negative value
    ///     indicates an down arrow or key.
    ///
    void stepRequested(int steps);

private:
    void emitSteps(int numSteps) override;
};
