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
#include <QWidget>
#include <QLabel>
#include <QRect>
#include <QMargins>


/// A popup data display window. A data display window is attached to each radio tool's crosshair and a corner of the
/// Window Tool. When the user hovers over a crosshair or the user drags the crosshair, the data display window is
/// shown and provides a continuous readout of the measurements appropriate to the tool. The user can set a preference
/// to not display the data windows.
///
class ToolDataWindow : public QWidget {

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

public slots:
    void xy1PositionChanged(QPointF coord);
    void xy2PositionChanged(QPointF coord);
    void xyvPositionChanged(QPointF coord);
    void widthHeightChanged(QSizeF widthHeight);
    void distanceChanged(double distance);
    void angleChanged(double angle);
    void aspectChanged(double aspect);
    void areaChanged(double area);

private:
    static constexpr QMargins k_targetMargins { 5, 5, 5, 5 };

    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_units;

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
