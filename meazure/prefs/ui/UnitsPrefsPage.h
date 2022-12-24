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
#include <meazure/prefs/models/UnitsPrefsModel.h>
#include <meazure/ui/DataField.h>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>


/// Custom measurement units definition user interface.
///
class UnitsPrefsPage : public PrefsPage {

public:
    UnitsPrefsPage();

    PrefsPageId getId() override {
        return PrefsPageId::UnitsPage;
    }

    QString getName() override {
        return tr("Custom Units");
    }

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

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
    QLineEdit* m_nameField;
    QLineEdit* m_abbrevField;
    DataField* m_factorField;
    QComboBox* m_basisCombo;
    QLabel* m_factorLabel;
    QPushButton* m_clearButton;
    QPushButton* m_precisionButton;
};
