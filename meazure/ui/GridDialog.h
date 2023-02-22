/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "fields/DoubleDataField.h"
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
    explicit GridDialog(GridTool* gridTool, const ScreenInfoProvider* screenInfoProvider,
                        const UnitsMgr* unitsMgr, QWidget* parent);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void linkSpacing(bool link);
    void unitsSelected(int index);
    void spacingChanged(double hspace, double vspace, bool linkSpacing, LinearUnitsId units);
    void softReset();
    void reject() override;
    void customUnitsChanged();

private:
    static constexpr int k_fieldShortWidth { 7 };   // Characters

    void createUI();
    void configureSpacingUI();
    void configureOrientationUI();
    void updateUI();
    void saveState();
    void restoreState();

    GridTool* m_gridTool;
    const ScreenInfoProvider* m_screenInfo;
    const UnitsMgr* m_unitsMgr;
    DoubleDataField* m_hSpacingField;
    DoubleDataField* m_vSpacingField;
    QComboBox* m_spacingUnitsCombo;
    QCheckBox* m_linkSpacingCheck;
    DoubleDataField* m_xPositionField;
    DoubleDataField* m_yPositionField;
    DoubleDataField* m_widthField;
    DoubleDataField* m_heightField;
    DoubleDataField* m_angleField;

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
