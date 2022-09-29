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

#include <QDoubleSpinBox>
#include <QSize>


/// Text field for presenting a measurement value.
///
class DataField : public QDoubleSpinBox {
public:
    /// Constructs a data field.
    ///
    /// @param[in] charWidth Width of the field in number of visible characters
    /// @param[in] showButtons Specify true to show the up/down arrow buttons
    /// @param[in] parent Parent for the widget
    ///
    DataField(int charWidth, bool showButtons, QWidget *parent = nullptr);

    /// Calculates the size of the field based on the desired character width.
    ///
    [[nodiscard]] QSize sizeHint() const override;

    /// Calculates the minimum size of the field based on the desired character width.
    ///
    [[nodiscard]] QSize minimumSizeHint() const override;

private:
    static constexpr int cursorBlinkingSpace { 2 };

    int m_charWidth;
};