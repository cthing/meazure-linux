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

#include "UnitsPrefsPage.h"
#include <meazure/utils/LayoutUtils.h>
#include <meazure/units/CustomUnits.h>
#include <QVBoxLayout>
#include <limits>


UnitsPrefsPage::UnitsPrefsPage(const ScreenInfo* screenInfo, UnitsMgr* unitsMgr) : // NOLINT(cppcoreguidelines-pro-type-member-init)
        PrefsPage(screenInfo, unitsMgr),
        m_model(new UnitsPrefsModel(unitsMgr, this)) {
    createUI();
    configure();

    nameAbbrevChanged();
}

void UnitsPrefsPage::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* nameLabel = new QLabel(tr("Display Name:"));
    m_nameField = new QLineEdit();
    m_nameField->setWhatsThis(tr("Sets the human readable name for the custom units."));

    auto* nameInstr = new QLabel(tr("(%1 character limit)").arg(k_maxNameLength));

    auto* abbrevLabel = new QLabel(tr("Abbreviation:"));
    m_abbrevField = new QLineEdit();
    m_abbrevField->setWhatsThis(tr("Sets the abbreviation for the custom units."));

    auto* abbrevInstr = new QLabel(tr("(%1 character limit)").arg(k_maxAbbrevLength));

    auto* settingInstr = new QLabel(tr("Set a resolution dependent or independent scale factor:"));

    m_factorField = new DoubleDataField(k_factorWidth, false);
    m_factorField->setWhatsThis(tr("Sets the scale factor for the custom units."));

    m_basisCombo = new QComboBox();
    m_basisCombo->setToolTip(tr("Select units basis"));
    m_basisCombo->setWhatsThis(tr("Select whether the custom units are based on resolution dependent or "
                                  "independent units."));

    m_factorLabel = new QLabel();

    m_clearButton = new QPushButton(tr("Clear"));
    m_clearButton->setToolTip(tr("Clear custom units settings"));
    m_clearButton->setWhatsThis(tr("Clear all custom units settings."));

    m_precisionButton = new QPushButton(tr("Set Display Precision"));
    m_precisionButton->setToolTip(tr("Set custom units decimal precisions."));
    m_precisionButton->setWhatsThis(tr("Sets the decimal precisions for the custom units."));

    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* nameAbbrevLayout = new QGridLayout();

    nameAbbrevLayout->addWidget(nameLabel, k_row0, k_col0, Qt::AlignRight);

    auto* nameLayout = new QHBoxLayout();
    nameLayout->addWidget(m_nameField);
    nameLayout->addWidget(nameInstr);
    nameAbbrevLayout->addLayout(nameLayout, k_row0, k_col1, Qt::AlignLeft);

    nameAbbrevLayout->addWidget(abbrevLabel, k_row1, k_col0, Qt::AlignRight);

    auto* abbrevLayout = new QHBoxLayout();
    abbrevLayout->addWidget(m_abbrevField);
    abbrevLayout->addWidget(abbrevInstr);
    nameAbbrevLayout->addLayout(abbrevLayout, k_row1, k_col1, Qt::AlignLeft);

    layout->addLayout(nameAbbrevLayout);

    layout->addSpacing(k_verticalSpace);

    layout->addWidget(settingInstr);

    auto* factorLayout = new QHBoxLayout();
    factorLayout->addWidget(m_factorField);
    factorLayout->addWidget(m_basisCombo);
    factorLayout->addWidget(m_factorLabel);
    factorLayout->addStretch(k_stretch1);
    layout->addLayout(factorLayout);

    layout->addSpacing(k_verticalSpace);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_clearButton);
    buttonLayout->addSpacing(k_buttonSpace);
    buttonLayout->addWidget(m_precisionButton);
    buttonLayout->addStretch(k_stretch1);
    layout->addLayout(buttonLayout);

    layout->addStretch(k_stretch1);
}

void UnitsPrefsPage::configure() {
    // Signals

    connect(m_model, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

    // Name and abbreviation fields

    const int charWidth = QFontMetrics(m_nameField->font()).maxWidth();

    m_nameField->setMaxLength(k_maxNameLength);
    m_nameField->setFixedWidth((k_maxNameLength + 1) * charWidth);
    connect(m_nameField, &QLineEdit::textEdited, this, [this](const QString& abbrev) {
        m_model->m_name->setValue(abbrev);
    });
    connect(m_nameField, &QLineEdit::textChanged, this, &UnitsPrefsPage::nameAbbrevChanged);
    connect(m_model->m_name, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        m_nameField->setText(value.value<QString>());
    });

    m_abbrevField->setMaxLength(k_maxAbbrevLength);
    m_abbrevField->setFixedWidth((k_maxAbbrevLength + 1) * charWidth);
    connect(m_abbrevField, &QLineEdit::textEdited, this, [this](const QString& abbrev) {
        m_model->m_abbrev->setValue(abbrev);
    });
    connect(m_abbrevField, &QLineEdit::textChanged, this, &UnitsPrefsPage::nameAbbrevChanged);
    connect(m_abbrevField, &QLineEdit::textChanged, this, &UnitsPrefsPage::updateFactorLabels);
    connect(m_model->m_abbrev, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        m_abbrevField->setText(value.value<QString>());
    });

    // Scale factor

    m_factorField->setRange(k_minFactor, std::numeric_limits<float>::max());
    m_factorField->setDecimals(std::numeric_limits<float>::digits10);
    m_factorField->setSingleStep(0);
    connect(m_factorField, &DoubleDataField::valueChanged, this, [this](double value) {
        m_model->m_scaleFactor->setValue(value);
    });
    connect(m_model->m_scaleFactor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        m_factorField->setValueQuietly(value.value<double>());
    });

    // Scale basis units

    m_basisCombo->addItem("px", CustomUnits::PixelBasis);
    m_basisCombo->addItem("in", CustomUnits::InchBasis);
    m_basisCombo->addItem("cm", CustomUnits::CentimeterBasis);
    connect(m_basisCombo, &QComboBox::activated, this, &UnitsPrefsPage::basisSelected);
    connect(m_model->m_scaleBasis, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        m_basisCombo->setCurrentIndex(value.value<int>());
    });

    // Clear units button

    connect(m_clearButton, &QPushButton::clicked, this, [this]() {
        m_nameField->clear();
        m_abbrevField->clear();

        m_model->m_name->setValue("");
        m_model->m_abbrev->setValue("");
    });

    // Precision button

    connect(m_precisionButton, &QPushButton::clicked, this, [this]() {
        emit pageRequested(PrefsPageId::PrecisionPage);
    });
}

void UnitsPrefsPage::initialize() {
    m_model->initialize();
}

void UnitsPrefsPage::apply() {
    m_model->apply();
}

bool UnitsPrefsPage::isDirty() const {
    return m_model->isDirty();
}

bool UnitsPrefsPage::isDefined() const {
    return !m_nameField->text().isEmpty() && !m_abbrevField->text().isEmpty();
}

void UnitsPrefsPage::nameAbbrevChanged() {
    setFactorEnabled(isDefined());

    m_clearButton->setEnabled(isDefined());
}

void UnitsPrefsPage::setFactorEnabled(bool enable) {
    m_factorField->setEnabled(enable);
    m_basisCombo->setEnabled(enable);
    m_factorLabel->setEnabled(enable);
}

void UnitsPrefsPage::updateFactorLabels(const QString& abbrev) {
    m_factorLabel->setText(tr("equals 1 %1").arg(abbrev.isEmpty() ? tr("custom unit") : abbrev));
}

void UnitsPrefsPage::basisSelected(int index) {
    auto basis = m_basisCombo->itemData(index).value<CustomUnits::ScaleBasis>();
    m_model->m_scaleBasis->setValue(basis);
}
