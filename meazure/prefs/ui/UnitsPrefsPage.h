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
#include "PrefsPageId.h"
#include <meazure/prefs/models/UnitsPrefsModel.h>
#include <meazure/ui/fields/DoubleDataField.h>
#include <meazure/ui/fields/LineEdit.h>
#include <meazure/units/UnitsMgr.h>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>


/// Custom measurement units definition user interface.
///
class UnitsPrefsPage : public PrefsPage {

    Q_OBJECT

public:
    explicit UnitsPrefsPage(const ScreenInfo* screenInfo, UnitsMgr* unitsMgr);

    PrefsPageId getId() override {
        return PrefsPageId::UnitsPage;
    }

    QString getName() override {
        return tr("Custom Units");
    }

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

signals:
    void linearPrecisionsRequested(LinearUnitsId);

private slots:
    void nameAbbrevChanged();
    void updateFactorLabels(const QString& abbrev);
    void basisSelected(int index);

private:
    static constexpr int k_maxNameLength { 16 };
    static constexpr int k_maxAbbrevLength { 2 };
    static constexpr int k_factorWidth { 10 };
    static constexpr int k_verticalSpace { 10 };
    static constexpr int k_buttonSpace { 10 };
    static constexpr double k_minFactor { 0.000001 };

    void createUI();
    void configure();
    [[nodiscard]] bool isDefined() const;
    void setFactorEnabled(bool enable);

    UnitsPrefsModel* m_model;
    LineEdit* m_nameField;
    LineEdit* m_abbrevField;
    QLabel* m_settingInstr;
    DoubleDataField* m_factorField;
    QComboBox* m_basisCombo;
    QLabel* m_factorLabel;
    QPushButton* m_clearButton;
    QPushButton* m_precisionButton;
};
