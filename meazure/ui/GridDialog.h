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
#include <meazure/units/Units.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/tools/GridTool.h>
#include <meazure/units/CustomUnits.h>
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QShowEvent>
#include <QSizeF>


class GridDialog : public QDialog {

    Q_OBJECT

public:
    explicit GridDialog(GridTool* gridTool, const ScreenInfoProvider& screenInfoProvider,
                        const UnitsMgr& unitsMgr, QWidget* parent);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void linkSpacing(bool link);
    void unitsSelected(int index);
    void spacingChanged(double hspace, double vspace, bool linkSpacing, LinearUnitsId units);
    void reset();
    void reject() override;
    void customUnitsChanged(const CustomUnits* customUnits);

private:
    static constexpr int k_fieldShortWidth { 7 };   // Characters

    void createUI();
    void configureSpacingUI();
    void configureOrientationUI();
    void updateUI();
    void saveState();
    void restoreState();

    GridTool* m_gridTool;
    const ScreenInfoProvider& m_screenInfo;
    const UnitsMgr& m_unitsMgr;
    DataField* m_hSpacingField;
    DataField* m_vSpacingField;
    QComboBox* m_spacingUnitsCombo;
    QCheckBox* m_linkSpacingCheck;
    DataField* m_xPositionField;
    DataField* m_yPositionField;
    DataField* m_widthField;
    DataField* m_heightField;
    DataField* m_angleField;

    int m_origX;
    int m_origY;
    int m_origWidth;
    int m_origHeight;
    int m_origAngle;
    double m_origSpacingH;
    double m_origSpacingV;
    bool m_origLinkedSpacing;
    LinearUnitsId m_origUnits;
};
