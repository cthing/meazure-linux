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

#include <meazure/units/UnitsProvider.h>
#include <meazure/tools/RadioToolTraits.h>
#include <meazure/graphics/Colors.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include <QFrame>
#include <QLabel>
#include <QRect>
#include <QMargins>
#include <QTimer>
#include <QColor>


/// A popup data display window. A data display window is attached to each radio tool's crosshair and a corner of the
/// Window Tool. When the user hovers over a crosshair or the user drags the crosshair, the data display window is
/// shown and provides a continuous readout of the measurements appropriate to the tool. The user can set a preference
/// to not display the data windows.
///
class ToolDataWindow : public QFrame {

    Q_OBJECT

public:
    /// Constructs a tool data window.
    ///
    /// @param[in] screenInfoProvider Screen information
    /// @param[in] unitsProvider Measurement units
    /// @param[in] traits Describes the data to be displayed on the window
    /// @param[in] parent Parent widget for the window or nullptr for a top level window
    /// @param[in] opacity Crosshair opacity
    ///
    explicit ToolDataWindow(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                            RadioToolTraits traits, QWidget* parent = nullptr,
                            QRgb opacity = Colors::get(Colors::CrossHairOpacity));

    /// Sets the data window opacity.
    ///
    /// @param[in] opacity Opacity of the data window ranging from 0 (transparent) to 255 (opaque)
    ///
    void setOpacity(int opacity);

    /// Moves the data window near the specified target rectangle. The data window will be positioned such that it
    /// never covers the target rectangle and is fully visible on the screen.
    ///
    /// @param[in] target Rectangle near which the data window will be moved, in global pixel coordinates
    ///
    void moveNear(const QRect& target);

    /// Visually strobes the data window by cycling the text between the background and foreground colors once.
    /// Typically, this is done to indicate a position has been logged.
    ///
    void strobe();

public slots:
    /// Called when current tool's position 1 has changed.
    ///
    /// @param[in] coord Tool coordinates in the current linear units, origin and y-axis direction.
    ///
    void xy1PositionChanged(QPointF coord);

    /// Called when current tool's position 2 has changed.
    ///
    /// @param[in] coord Tool coordinates in the current linear units, origin and y-axis direction.
    ///
    void xy2PositionChanged(QPointF coord);

    /// Called when current tool's vertex position has changed.
    ///
    /// @param[in] coord Tool coordinates in the current linear units, origin and y-axis direction.
    ///
    void xyvPositionChanged(QPointF coord);

    /// Called when the current tool's width or height has changed.
    ///
    /// @param[in] widthHeight Width and height of the tool in the current linear units.
    ///
    void widthHeightChanged(QSizeF widthHeight);

    /// Called when the current tool's distance has changed (e.g. hypot of the Rectangle tool).
    ///
    /// @param[in] distance Tool distance in the current linear units.
    ///
    void distanceChanged(double distance);

    /// Called when the current tool's angle has changed.
    ///
    /// @param[in] angle Tool angle in the current angular units.
    ///
    void angleChanged(double angle);

    /// Called when the current tool's aspect ratio has changed (e.g. Rectangle tool aspect ratio).
    ///
    /// @param[in] aspect Tool aspect ratio
    ///
    void aspectChanged(double aspect);

    /// Called when the current tool's area has changed (e.g. area of the Rectangle tool).
    ///
    /// @param[in] area Tool area
    ///
    void areaChanged(double area);

private slots:
    /// Called to cycle the data window text between its normal color and the window background color.
    ///
    void flashHandler();

private:
    static constexpr QMargins k_targetMargins { 5, 5, 5, 5 };

    /// Sets the data window's background and text colors.
    ///
    void setColors();

    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_units;
    QTimer m_flashTimer;
    int m_flashCountDown { -1 };
    bool m_showText { true };

    QColor m_backgroundColor;
    QColor m_textColor;

    QLabel* m_x1Value;
    QLabel* m_y1Value;
    QLabel* m_x2Value;
    QLabel* m_y2Value;
    QLabel* m_xvValue;
    QLabel* m_yvValue;
    QLabel* m_wValue;
    QLabel* m_hValue;
    QLabel* m_dValue;
    QLabel* m_aValue;
    QLabel* m_asValue;
    QLabel* m_arValue;
};
