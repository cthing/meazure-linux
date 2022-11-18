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
    DataField(int charWidth, bool showButtons, bool readOnly = false, bool nativeStepHandling = true,
              QWidget *parent = nullptr);

    /// Calculates the size of the field based on the desired character width.
    ///
    [[nodiscard]] QSize sizeHint() const override;

    /// Calculates the minimum size of the field based on the desired character width.
    ///
    [[nodiscard]] QSize minimumSizeHint() const override;

    /// Sets the value in the spin box without emitting any signals.
    ///
    /// @param[in] value Spin box value
    ///
    void setValueQuietly(double value);

    /// Sets the numerical precision for the spin box value without emitting any signals.
    ///
    /// @param[in] precision Spin box numerical precision
    ///
    void setDecimalsQuietly(int precision);

    /// Sets the minimum and maximum values for the field without emitting any signals.
    ///
    /// @param[in] minimum Minimum field value
    /// @param[in] maximum Maximum field value
    ///
    void setRangeQuietly(double minimum, double maximum);

    /// Sets the minimum value for the field without emitting any signals.
    ///
    /// @param[in] minimum Minimum field value
    ///
    void setMinimumQuietly(double minimum);

signals:
    /// Emitted to indicate that the user has either pressed an up/down arrow button, up/down arrow key, or page
    /// up/down key while the field has keyboard focus.
    ///
    /// @param[in] steps The number of steps to increment or decrement the current tool position. The number of steps
    ///     is greater for the page up/down keys. A positive value indicates an up arrow or key and a negative value
    ///     indicates an down arrow or key.
    ///
    void stepRequested(int steps);

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    static constexpr int k_cursorBlinkingSpace { 2 };

    void emitSteps(int numSteps);

    int m_charWidth;
    QColor m_defaultBackground;
    QColor m_readOnlyBackground { 240, 240, 240 };
    bool m_nativeStepHandling;
};
