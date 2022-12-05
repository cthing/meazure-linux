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

#include "DataField.h"
#include <meazure/tools/RadioTool.h>
#include <meazure/units/Units.h>
#include <QGroupBox>
#include <QLabel>


/// Presents the tool measurement information.
///
class ToolDataSection : public QGroupBox {

    Q_OBJECT

public:
    ToolDataSection();

private slots:
    /// Called when a radio tool has been selected.
    ///
    /// @param[in] tool Newly selected radio tool
    ///
    void radioToolSelected(RadioTool& tool);

    /// Called when the linear units have changed.
    ///
    /// @param[in] unitsId Identifier of the newly selected units
    ///
    void linearUnitsChanged(LinearUnitsId unitsId);

    /// Called when the angular units have changed.
    ///
    /// @param[in] unitsId Identifier of the newly selected units
    ///
    void angularUnitsChanged(AngularUnitsId unitsId);

    /// Called when a tool's position 1 has changed.
    ///
    /// @param[in] coord Coordinates for the position in the current units, origin and y-axis direction
    ///
    void xy1PositionChanged(QPointF coord, QPoint rawPos);

    /// Called when a tool's position 2 has changed.
    ///
    /// @param[in] coord Coordinates for the position in the current units, origin and y-axis direction
    ///
    void xy2PositionChanged(QPointF coord, QPoint rawPos);

    /// Called when a tool's vertex position has changed.
    ///
    /// @param[in] coord Coordinates for the position in the current units, origin and y-axis direction
    ///
    void xyvPositionChanged(QPointF coord, QPoint rawPos);

    /// Called when a tool's width and height change
    ///
    /// @param[in] widthHeight New width and height in the current units
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

private:
    static constexpr int k_fieldShortWidth { 7 };   // Characters
    static constexpr int k_fieldLongWidth { 20 };   // Characters

    /// Creates the text fields that provide the tool measurement display.
    ///
    void createFields();

    QLabel* m_x1Label;
    QLabel* m_y1Label;
    QLabel* m_x2Label;
    QLabel* m_y2Label;
    QLabel* m_xvLabel;
    QLabel* m_yvLabel;
    QLabel* m_wLabel;
    QLabel* m_hLabel;
    QLabel* m_dLabel;
    QLabel* m_aLabel;
    QLabel* m_asLabel;
    QLabel* m_arLabel;
    DataField* m_x1Field;
    DataField* m_y1Field;
    DataField* m_x2Field;
    DataField* m_y2Field;
    DataField* m_xvField;
    DataField* m_yvField;
    DataField* m_wField;
    DataField* m_hField;
    DataField* m_dField;
    DataField* m_aField;
    DataField* m_asField;
    DataField* m_arField;
    QLabel* m_y1Units;
    QLabel* m_y2Units;
    QLabel* m_yvUnits;
    QLabel* m_hUnits;
    QLabel* m_dUnits;
    QLabel* m_aUnits;
    QLabel* m_arUnits;
};
