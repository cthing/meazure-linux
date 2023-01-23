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

#include "PrefsPage.h"
#include "PrefsPageId.h"
#include <meazure/prefs/models/PrecisionPrefsModel.h>
#include <meazure/ui/fields/IntegerDataField.h>
#include <meazure/units/UnitsMgr.h>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>


/// User interface for measurement units display precisions. Each measurement unit has associated with it a set of
/// display precisions, one for each measurement type (e.g. width, height, x, y).
///
class PrecisionPrefsPage : public PrefsPage {

public:
    explicit PrecisionPrefsPage(UnitsMgr& unitsMgr);

    PrefsPageId getId() override {
        return PrefsPageId::PrecisionPage;
    }

    QString getName() override {
        return tr("Precision");
    }

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

private slots:
    void unitsSelected(int index);
    void restore();
    void restoreAll();

private:
    static constexpr int k_charWidth { 2 };
    static constexpr int k_minPrecision { 0 };
    static constexpr int k_maxPrecision { 6 };
    static constexpr QMargins k_contentMargin { 10, 10, 10, 10 };   // Page margin, pixels
    static constexpr int k_angularIdMask { 0x20 };

    void createUI();
    void configure();

    void precisionFieldChanged(LinearMeasurementId measurementId, int value);
    void precisionFieldChanged(AngularMeasurementId measurementId, int value);

    void precisionModelChanged(LinearUnitsId unitsId, QVariant& value);
    void precisionModelChanged(AngularUnitsId unitsId, QVariant& value);

    UnitsMgr& m_unitsMgr;
    PrecisionPrefsModel* m_model;
    QComboBox* m_unitsCombo;
    QPushButton* m_defaultButton;
    QPushButton* m_defaultAllButton;
    QLabel* m_xCoordLabel;
    QLabel* m_yCoordLabel;
    QLabel* m_widthLabel;
    QLabel* m_heightLabel;
    QLabel* m_distanceLabel;
    QLabel* m_areaLabel;
    QLabel* m_resXLabel;
    QLabel* m_resYLabel;
    QLabel* m_angleLabel;
    IntegerDataField* m_xCoordField;
    IntegerDataField* m_yCoordField;
    IntegerDataField* m_widthField;
    IntegerDataField* m_heightField;
    IntegerDataField* m_distanceField;
    IntegerDataField* m_areaField;
    IntegerDataField* m_resXField;
    IntegerDataField* m_resYField;
    IntegerDataField* m_angleField;
};
