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

#include "PrefsPage.h"
#include "Caliper.h"
#include <meazure/prefs/models/CalibrationPrefsModel.h>
#include <meazure/ui/fields/DoubleDataField.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsMgr.h>
#include <QComboBox>
#include <QRadioButton>
#include <QLabel>


class CalibrationPrefsPage : public PrefsPage {

public:
    CalibrationPrefsPage(ScreenInfo* screenInfo, const UnitsMgr* unitsMgr);

    PrefsPageId getId() override {
        return PrefsPageId::CalibrationPage;
    }

    QString getName() override {
        return tr("Calibration");
    };

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

private slots:
    void screenChanged();
    void calibrationChanged();
    void unitsChanged();
    void resolutionChanged();

private:
    static constexpr int k_charWidth { 8 };
    static constexpr int k_hCaliperLength { 250 };
    static constexpr int k_vCaliperLength { 210 };

    void createUI();
    void configure();

    CalibrationPrefsModel* m_model;
    QComboBox* m_screenCombo;
    QRadioButton* m_systemRadio;
    QRadioButton* m_manualRadio;
    QLabel* m_unitsLabel;
    QComboBox* m_unitsCombo;
    QLabel* m_resXLabel;
    QLabel* m_resYLabel;
    QLabel* m_widthLabel;
    QLabel* m_heightLabel;
    DoubleDataField* m_resXField;
    DoubleDataField* m_resYField;
    DoubleDataField* m_widthField;
    DoubleDataField* m_heightField;
    QLabel* m_resUnitsLabel;
    QLabel* m_sizeUnitsLabel;
    QLabel* m_calibrationInstrLabel;
    QString m_caliperInstr;
    QLabel* m_caliperInstrLabel;
    Caliper* m_hCaliper;
    Caliper* m_vCaliper;
};
