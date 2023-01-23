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

#include "PrecisionPrefsPage.h"
#include <meazure/utils/LayoutUtils.h>
#include <QGridLayout>


PrecisionPrefsPage::PrecisionPrefsPage(const ScreenInfo* screenInfo, UnitsMgr* unitsMgr) : // NOLINT(cppcoreguidelines-pro-type-member-init)
        PrefsPage(screenInfo, unitsMgr),
        m_model(new PrecisionPrefsModel(unitsMgr, this)) {
    createUI();
    configure();

    unitsSelected(0);
}

void PrecisionPrefsPage::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* unitsLabel = new QLabel(tr("Units:"));
    m_unitsCombo = new QComboBox();
    m_defaultButton = new QPushButton(tr("Default"));
    m_defaultButton->setToolTip(tr("Restore defaults for current units"));
    m_defaultAllButton = new QPushButton(tr("Default All"));
    m_defaultAllButton->setToolTip(tr("Restore defaults for all units"));

    auto createField = []() {
        auto* field = new IntegerDataField(k_charWidth, true);
        field->setRange(k_minPrecision, k_maxPrecision);
        field->setSingleStep(1);
        return field;
    };

    m_xCoordField = createField();
    m_yCoordField = createField();
    m_widthField = createField();
    m_heightField = createField();
    m_distanceField = createField();
    m_areaField = createField();
    m_resXField = createField();
    m_resYField = createField();
    m_angleField = createField();

    m_xCoordLabel = new QLabel(tr("X:"));
    m_yCoordLabel = new QLabel(tr("Y:"));
    m_widthLabel = new QLabel(tr("W:"));
    m_heightLabel = new QLabel(tr("H:"));
    m_distanceLabel = new QLabel(tr("D:"));
    m_areaLabel = new QLabel(tr("Ar:"));
    m_resXLabel = new QLabel(tr("Rx:"));
    m_resYLabel = new QLabel(tr("Ry:"));
    m_angleLabel = new QLabel(tr("A:"));

    auto* layout = new QGridLayout();
    setLayout(layout);

    layout->setContentsMargins(k_contentMargin);

    auto* unitsLayout = new QHBoxLayout();
    unitsLayout->addWidget(unitsLabel);
    unitsLayout->addWidget(m_unitsCombo);
    unitsLayout->addWidget(m_defaultButton);
    unitsLayout->addWidget(m_defaultAllButton);
    unitsLayout->addStretch(k_stretch1);
    layout->addLayout(unitsLayout, k_row0, k_col0, k_rowspan1, k_colspan6);

    layout->addWidget(m_xCoordLabel,   k_row1, k_col0, Qt::AlignRight);
    layout->addWidget(m_xCoordField,   k_row1, k_col1, Qt::AlignLeft);
    layout->addWidget(m_yCoordLabel,   k_row1, k_col2, Qt::AlignRight);
    layout->addWidget(m_yCoordField,   k_row1, k_col3, Qt::AlignLeft);
    layout->addWidget(m_distanceLabel, k_row1, k_col4, Qt::AlignRight);
    layout->addWidget(m_distanceField, k_row1, k_col5, Qt::AlignLeft);

    layout->addWidget(m_widthLabel,    k_row2, k_col0, Qt::AlignRight);
    layout->addWidget(m_widthField,    k_row2, k_col1, Qt::AlignLeft);
    layout->addWidget(m_heightLabel,   k_row2, k_col2, Qt::AlignRight);
    layout->addWidget(m_heightField,   k_row2, k_col3, Qt::AlignLeft);
    layout->addWidget(m_areaLabel,     k_row2, k_col4, Qt::AlignRight);
    layout->addWidget(m_areaField,     k_row2, k_col5, Qt::AlignLeft);

    layout->addWidget(m_resXLabel,     k_row3, k_col0, Qt::AlignRight);
    layout->addWidget(m_resXField,     k_row3, k_col1, Qt::AlignLeft);
    layout->addWidget(m_resYLabel,     k_row3, k_col2, Qt::AlignRight);
    layout->addWidget(m_resYField,     k_row3, k_col3, Qt::AlignLeft);
    layout->addWidget(m_angleLabel,    k_row3, k_col4, Qt::AlignRight);
    layout->addWidget(m_angleField,    k_row3, k_col5, Qt::AlignLeft);

    layout->setColumnStretch(k_col6, k_stretch1);
}

void PrecisionPrefsPage::configure() {
    // Signals

    connect(m_model, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

    // Units combo

    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(PixelsId)->getLengthLabel(), PixelsId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(PointsId)->getLengthLabel(), PointsId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(TwipsId)->getLengthLabel(), TwipsId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(InchesId)->getLengthLabel(), InchesId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(CentimetersId)->getLengthLabel(), CentimetersId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(MillimetersId)->getLengthLabel(), MillimetersId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(PicasId)->getLengthLabel(), PicasId);
    m_unitsCombo->addItem(tr("custom"), CustomId);

    m_unitsCombo->insertSeparator(200);

    m_unitsCombo->addItem(m_unitsMgr->getAngularUnits(DegreesId)->getLabel(), k_angularIdMask | DegreesId);
    m_unitsCombo->addItem(m_unitsMgr->getAngularUnits(RadiansId)->getLabel(), k_angularIdMask | RadiansId);

    connect(m_unitsCombo, &QComboBox::activated, this, &PrecisionPrefsPage::unitsSelected);

    // Defaults buttons

    connect(m_defaultButton, &QPushButton::clicked, this, &PrecisionPrefsPage::restore);
    connect(m_defaultAllButton, &QPushButton::clicked, this, &PrecisionPrefsPage::restoreAll);

    // Fields

    connect(m_xCoordField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(XCoord, value);
    });
    connect(m_yCoordField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(YCoord, value);
    });
    connect(m_widthField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(Width, value);
    });
    connect(m_heightField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(Height, value);
    });
    connect(m_distanceField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(Distance, value);
    });
    connect(m_areaField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(Area, value);
    });
    connect(m_resXField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(ResX, value);
    });
    connect(m_resYField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(ResY, value);
    });
    connect(m_angleField, &IntegerDataField::valueChanged, this, [this](int value) {
        precisionFieldChanged(Angle, value);
    });

    /// Model

    for (const LinearUnitsId unitsId : LinearUnitsIdIter()) {
        connect(m_model->m_linearPrecisions.at(unitsId), &PreferenceBase::valueChanged,
                this, [this, unitsId](QVariant value) {
            precisionModelChanged(unitsId, value);
        });
    }
    for (const AngularUnitsId unitsId : AngularUnitsIdIter()) {
        connect(m_model->m_angularPrecisions.at(unitsId), &PreferenceBase::valueChanged,
                this, [this, unitsId](QVariant value) {
            precisionModelChanged(unitsId, value);
        });
    }
}

void PrecisionPrefsPage::unitsSelected(int index) {
    const int data = m_unitsCombo->itemData(index).toInt();
    const bool isLinear = (data & k_angularIdMask) == 0;
    if (isLinear) {
        const auto unitsId = static_cast<LinearUnitsId>(data);
        const Preference<Units::DisplayPrecisions>* precisions = m_model->m_linearPrecisions.at(unitsId);

        m_xCoordField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::XCoord));
        m_yCoordField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::YCoord));
        m_widthField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::Width));
        m_heightField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::Height));
        m_distanceField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::Distance));
        m_areaField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::Area));
        m_resXField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::ResX));
        m_resYField->setValueQuietly(precisions->getValue().at(LinearMeasurementId::ResY));
    } else {
        const auto unitsId = static_cast<AngularUnitsId>(data & ~k_angularIdMask);
        const Preference<Units::DisplayPrecisions>* precisions = m_model->m_angularPrecisions.at(unitsId);

        m_angleField->setValueQuietly(precisions->getValue().at(AngularMeasurementId::Angle));
    }

    m_xCoordField->setEnabled(isLinear);
    m_yCoordField->setEnabled(isLinear);
    m_widthField->setEnabled(isLinear);
    m_heightField->setEnabled(isLinear);
    m_distanceField->setEnabled(isLinear);
    m_areaField->setEnabled(isLinear);
    m_resXField->setEnabled(isLinear);
    m_resYField->setEnabled(isLinear);
    m_angleField->setEnabled(!isLinear);

    m_xCoordLabel->setEnabled(isLinear);
    m_yCoordLabel->setEnabled(isLinear);
    m_widthLabel->setEnabled(isLinear);
    m_heightLabel->setEnabled(isLinear);
    m_distanceLabel->setEnabled(isLinear);
    m_areaLabel->setEnabled(isLinear);
    m_resXLabel->setEnabled(isLinear);
    m_resYLabel->setEnabled(isLinear);
    m_angleLabel->setEnabled(!isLinear);
}

void PrecisionPrefsPage::precisionFieldChanged(LinearMeasurementId measurementId, int value) {
    const int data = m_unitsCombo->currentData().toInt();
    const bool isLinear = (data & k_angularIdMask) == 0;
    if (isLinear) {
        const auto unitsId = static_cast<LinearUnitsId>(data);
        m_model->setPrecision(unitsId, measurementId, value);
    }
}

void PrecisionPrefsPage::precisionFieldChanged(AngularMeasurementId measurementId, int value) {
    const int data = m_unitsCombo->currentData().toInt();
    const bool isLinear = (data & k_angularIdMask) == 0;
    if (!isLinear) {
        const auto unitsId = static_cast<AngularUnitsId>(data & ~k_angularIdMask);
        m_model->setPrecision(unitsId, measurementId, value);
    }
}

void PrecisionPrefsPage::precisionModelChanged(LinearUnitsId unitsId, QVariant& value) {
    const int data = m_unitsCombo->currentData().toInt();
    const bool isLinear = (data & k_angularIdMask) == 0;
    if (isLinear && static_cast<LinearUnitsId>(data) == unitsId) {
        const Units::DisplayPrecisions* precisions = static_cast<Units::DisplayPrecisions*>(value.data());
        m_xCoordField->setValueQuietly(precisions->at(LinearMeasurementId::XCoord));
        m_yCoordField->setValueQuietly(precisions->at(LinearMeasurementId::YCoord));
        m_widthField->setValueQuietly(precisions->at(LinearMeasurementId::Width));
        m_heightField->setValueQuietly(precisions->at(LinearMeasurementId::Height));
        m_distanceField->setValueQuietly(precisions->at(LinearMeasurementId::Distance));
        m_areaField->setValueQuietly(precisions->at(LinearMeasurementId::Area));
        m_resXField->setValueQuietly(precisions->at(LinearMeasurementId::ResX));
        m_resYField->setValueQuietly(precisions->at(LinearMeasurementId::ResY));
    }
}

void PrecisionPrefsPage::precisionModelChanged(AngularUnitsId unitsId, QVariant& value) {
    const int data = m_unitsCombo->currentData().toInt();
    const bool isLinear = (data & k_angularIdMask) == 0;
    if (!isLinear && static_cast<AngularUnitsId>(data & ~k_angularIdMask) == unitsId) {
        const Units::DisplayPrecisions* precisions = static_cast<Units::DisplayPrecisions*>(value.data());
        m_angleField->setValueQuietly(precisions->at(AngularMeasurementId::Angle));
    }
}

void PrecisionPrefsPage::restore() {
    const int data = m_unitsCombo->currentData().toInt();
    const bool isLinear = (data & k_angularIdMask) == 0;
    if (isLinear) {
        const auto unitsId = static_cast<LinearUnitsId>(data);
        m_model->m_linearPrecisions.at(unitsId)->restore();
    } else {
        const auto unitsId = static_cast<AngularUnitsId>(data & ~k_angularIdMask);
        m_model->m_angularPrecisions.at(unitsId)->restore();
    }
}

void PrecisionPrefsPage::restoreAll() {
    for (const auto& entry : m_model->m_linearPrecisions) {
        entry.second->restore();
    }
    for (const auto& entry : m_model->m_angularPrecisions) {
        entry.second->restore();
    }
}

void PrecisionPrefsPage::initialize() {
    m_model->initialize();
}

void PrecisionPrefsPage::apply() {
    m_model->apply();
}

bool PrecisionPrefsPage::isDirty() const {
    return m_model->isDirty();
}
