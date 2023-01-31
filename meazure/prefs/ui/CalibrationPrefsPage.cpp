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

#include "CalibrationPrefsPage.h"
#include <meazure/utils/LayoutUtils.h>
#include <QGridLayout>
#include <QSignalBlocker>


CalibrationPrefsPage::CalibrationPrefsPage(ScreenInfo* screenInfo, const UnitsMgr* unitsMgr) :  // NOLINT(cppcoreguidelines-pro-type-member-init)
        PrefsPage(screenInfo, unitsMgr),
        m_model(new CalibrationPrefsModel(screenInfo, this)) {
    createUI();
    configure();
}

void CalibrationPrefsPage::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* screenLabel = new QLabel(tr("Screen:"));
    m_screenCombo = new QComboBox();
    m_screenCombo->setToolTip(tr("Screen to calibrate"));
    m_screenCombo->setWhatsThis(tr("Selects screen to calibrate."));

    m_systemRadio = new QRadioButton(tr("Use operating system resolution"));
    m_systemRadio->setWhatsThis(tr("Uses the resolution provided by the operating system."));

    m_manualRadio = new QRadioButton(tr("Set manually"));
    m_manualRadio->setWhatsThis(tr("Uses the resolution determined by manual calibration."));

    m_unitsLabel = new QLabel(tr("Calibration units:"));

    m_unitsCombo = new QComboBox();
    m_unitsCombo->setWhatsThis(tr("Units in which the calibration is performed."));

    m_resXLabel = new QLabel(tr("Rx:"));
    m_resYLabel = new QLabel(tr("Ry:"));
    m_widthLabel = new QLabel(tr("W:"));
    m_heightLabel = new QLabel(tr("H:"));

    m_resXField = new DoubleDataField(k_charWidth, false);
    m_resXField->setWhatsThis(tr("Horizontal resolution"));

    m_resYField = new DoubleDataField(k_charWidth, false);
    m_resYField->setWhatsThis(tr("Vertical resolution"));

    m_widthField = new DoubleDataField(k_charWidth, false);
    m_widthField->setWhatsThis("Screen width in the specified units.");

    m_heightField = new DoubleDataField(k_charWidth, false);
    m_heightField->setWhatsThis("Screen height in the specified units.");

    m_resUnitsLabel = new QLabel();
    m_sizeUnitsLabel = new QLabel();
    m_calibrationInstrLabel = new QLabel(tr(
            "Calibrate by entering the pixel pitch (Rx, Ry), or screen dimensions (W, H), or by adjusting the calipers "
            "using a ruler.<br><br>Browse monitor specifications at "
            "<a href='https://www.displayspecifications.com'>DisplaySpecifications.com</a>"));
    m_calibrationInstrLabel->setTextFormat(Qt::RichText);
    m_calibrationInstrLabel->setOpenExternalLinks(true);
    m_calibrationInstrLabel->setWordWrap(true);
    m_caliperInstr = tr("Adjust caliper to the %1 mark on a ruler");
    m_caliperInstrLabel = new QLabel(m_caliperInstr);

    m_hCaliper = new Caliper(Caliper::Horizontal, k_hCaliperLength);
    m_hCaliper->setWhatsThis(tr("Caliper for horizontal resolution calibration."));

    m_vCaliper = new Caliper(Caliper::Vertical, k_vCaliperLength);
    m_vCaliper->setWhatsThis(tr("Caliper for vertical resolution calibration."));

    auto* layout = new QGridLayout();
    setLayout(layout);

    auto* screenLayout = new QHBoxLayout();
    screenLayout->addWidget(screenLabel);
    screenLayout->addWidget(m_screenCombo);
    screenLayout->addStretch(k_stretch1);
    layout->addLayout(screenLayout, k_row0, k_col0);

    layout->addWidget(m_systemRadio, k_row1, k_col0);
    layout->addWidget(m_manualRadio, k_row2, k_col0);

    auto* unitsLayout = new QHBoxLayout();
    unitsLayout->addWidget(m_unitsLabel);
    unitsLayout->addWidget(m_unitsCombo);
    unitsLayout->addStretch(k_stretch1);

    auto* manualFieldsLayout = new QGridLayout();
    manualFieldsLayout->addLayout(unitsLayout,             k_row0, k_col1, k_rowspan1, k_colspan5);
    manualFieldsLayout->addWidget(m_resXLabel,             k_row1, k_col1, Qt::AlignRight);
    manualFieldsLayout->addWidget(m_resXField,             k_row1, k_col2, Qt::AlignLeft);
    manualFieldsLayout->addWidget(m_resYLabel,             k_row1, k_col3, Qt::AlignRight);
    manualFieldsLayout->addWidget(m_resYField,             k_row1, k_col4, Qt::AlignLeft);
    manualFieldsLayout->addWidget(m_resUnitsLabel,         k_row1, k_col5, Qt::AlignLeft);
    manualFieldsLayout->addWidget(m_widthLabel,            k_row2, k_col1, Qt::AlignRight);
    manualFieldsLayout->addWidget(m_widthField,            k_row2, k_col2, Qt::AlignLeft);
    manualFieldsLayout->addWidget(m_heightLabel,           k_row2, k_col3, Qt::AlignRight);
    manualFieldsLayout->addWidget(m_heightField,           k_row2, k_col4, Qt::AlignLeft);
    manualFieldsLayout->addWidget(m_sizeUnitsLabel,        k_row2, k_col5, Qt::AlignLeft);
    manualFieldsLayout->addWidget(m_caliperInstrLabel,     k_row3, k_col1, k_rowspan1, k_colspan7);
    manualFieldsLayout->addWidget(m_hCaliper,              k_row4, k_col1, k_rowspan1, k_colspan7);
    manualFieldsLayout->addWidget(m_calibrationInstrLabel, k_row0, k_col7, k_rowspan3, k_colspan1, Qt::AlignTop);
    manualFieldsLayout->setColumnMinimumWidth(k_col0, 30);
    manualFieldsLayout->setColumnMinimumWidth(k_col6, 5);
    layout->addLayout(manualFieldsLayout, k_row3, k_col0);

    layout->addWidget(m_vCaliper, k_row0, k_col1, k_rowspan4, k_colspan1, Qt::AlignBottom);
}

void CalibrationPrefsPage::configure() {
    // Dirty signal

    connect(m_model, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

    // Screen select

    for (int i = 0; i < m_screenInfo->getNumScreens(); i++) {
        m_screenCombo->addItem(m_screenInfo->getScreenName(i), i);
    }

    connect(m_screenCombo, &QComboBox::activated, this, [this](int itemIndex) {
        const int screenIndex = m_screenCombo->itemData(itemIndex).toInt();
        m_model->m_screenIndex->setValue(screenIndex);
    });
    connect(m_model, &CalibrationPrefsModel::screenIndexChanged, this, &CalibrationPrefsPage::screenChanged);

    // Manual or system resolution

    connect(m_manualRadio, &QRadioButton::toggled, m_model, &CalibrationPrefsModel::setUseManualRes);
    connect(m_model, &CalibrationPrefsModel::useManualResChanged, this, &CalibrationPrefsPage::calibrationChanged);

    // Manual resolution units

    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(InchesId)->getLengthLabel(), InchesId);
    m_unitsCombo->addItem(m_unitsMgr->getLinearUnits(CentimetersId)->getLengthLabel(), CentimetersId);
    connect(m_unitsCombo, &QComboBox::activated, this, [this](int itemIndex) {
        const LinearUnitsId unitsId = static_cast<LinearUnitsId>(m_unitsCombo->itemData(itemIndex).toUInt());
        m_model->setCalInInches(unitsId == InchesId);
    });
    connect(m_model, &CalibrationPrefsModel::calInInchesChanged, this, &CalibrationPrefsPage::unitsChanged);

    // Manual resolution

    connect(m_resXField, &DoubleDataField::valueChanged, this, [this](double resX) {
        m_model->setManualResX(m_model->isCalInInches() ? resX : resX * CentimeterUnits::k_cmPerIn);
    });

    connect(m_resYField, &DoubleDataField::valueChanged, this, [this](double resY) {
        m_model->setManualResY(m_model->isCalInInches() ? resY : resY * CentimeterUnits::k_cmPerIn);
    });

    // Manual screen size

    connect(m_widthField, &DoubleDataField::valueChanged, this, [this](double width) {
        const double widthInches = m_model->isCalInInches() ? width : width / CentimeterUnits::k_cmPerIn;
        m_model->setManualWidth(widthInches);
    });
    connect(m_heightField, &DoubleDataField::valueChanged, this, [this](double height) {
        const double heightInches = m_model->isCalInInches() ? height : height / CentimeterUnits::k_cmPerIn;
        m_model->setManualHeight(heightInches);
    });

    // Resolution calipers

    connect(m_hCaliper, &Caliper::jawPositionChanged, this, [this](int resX) {
        m_model->setManualResX(m_model->isCalInInches() ? resX : resX * CentimeterUnits::k_cmPerIn / 2.0);
    });
    connect(m_vCaliper, &Caliper::jawPositionChanged, this, [this](int resY) {
        m_model->setManualResY(m_model->isCalInInches() ? resY : resY * CentimeterUnits::k_cmPerIn / 2.0);
    });

    // Resolution model

    connect(m_model, &CalibrationPrefsModel::manualResChanged, this, &CalibrationPrefsPage::resolutionChanged);
}

void CalibrationPrefsPage::calibrationChanged() {
    const bool enable = m_model->isUseManualRes();

    m_unitsLabel->setEnabled(enable);
    m_unitsCombo->setEnabled(enable);
    m_resXLabel->setEnabled(enable);
    m_resXField->setEnabled(enable);
    m_resYLabel->setEnabled(enable);
    m_resYField->setEnabled(enable);
    m_resUnitsLabel->setEnabled(enable);
    m_widthLabel->setEnabled(enable);
    m_widthField->setEnabled(enable);
    m_heightLabel->setEnabled(enable);
    m_heightField->setEnabled(enable);
    m_sizeUnitsLabel->setEnabled(enable);
    m_calibrationInstrLabel->setEnabled(enable);
    m_caliperInstrLabel->setEnabled(enable);

    // Only enable calipers if on the same screen as this page.

    const int currentScreenIndex = m_screenInfo->screenForWindow(this);
    const bool caliperEnable = enable && (m_model->m_screenIndex->getValue() == currentScreenIndex);

    m_hCaliper->setEnabled(caliperEnable);
    m_vCaliper->setEnabled(caliperEnable);
}

void CalibrationPrefsPage::screenChanged() {
    const QSignalBlocker signalBlocker(m_screenCombo);
    const QSignalBlocker unitsComboBlocker(m_unitsCombo);
    const QSignalBlocker manualRadioBlocker(m_manualRadio);

    const int screenIndex = m_model->m_screenIndex->getValue();
    const int screenItemIndex = m_screenCombo->findData(screenIndex);
    m_screenCombo->setCurrentIndex(screenItemIndex);

    if (m_model->isUseManualRes()) {
        m_manualRadio->setChecked(true);
    } else {
        m_systemRadio->setChecked(true);
    }

    unitsChanged();
}

void CalibrationPrefsPage::unitsChanged() {
    const QSignalBlocker unitsComboBlocker(m_unitsCombo);
    const QSignalBlocker hCaliperBlocker(m_hCaliper);
    const QSignalBlocker vCaliperBlocker(m_vCaliper);

    const bool useInches = m_model->isCalInInches();

    const int unitsItemIndex = m_unitsCombo->findData(useInches ? InchesId : CentimetersId);
    m_unitsCombo->setCurrentIndex(unitsItemIndex);

    if (useInches) {
        m_resUnitsLabel->setText("px/in");
        m_sizeUnitsLabel->setText("in");
        m_caliperInstrLabel->setText(m_caliperInstr.arg("1 inch"));
    } else {
        m_resUnitsLabel->setText("px/cm");
        m_sizeUnitsLabel->setText("cm");
        m_caliperInstrLabel->setText(m_caliperInstr.arg("2 centimeter"));
    }

    resolutionChanged();
}

void CalibrationPrefsPage::resolutionChanged() {
    const QSignalBlocker hCaliperBlocker(m_hCaliper);
    const QSignalBlocker vCaliperBlocker(m_vCaliper);

    const bool useInches = m_model->isCalInInches();

    const double resX = m_model->getManualResX();
    m_resXField->setValueQuietly(useInches ? resX : resX / CentimeterUnits::k_cmPerIn);
    const double resY = m_model->getManualResY();
    m_resYField->setValueQuietly(useInches ? resY : resY / CentimeterUnits::k_cmPerIn);

    const double width = m_model->getManualWidth();
    m_widthField->setValueQuietly(useInches ? width : width * CentimeterUnits::k_cmPerIn);
    const double height = m_model->getManualHeight();
    m_heightField->setValueQuietly(useInches ? height : height * CentimeterUnits::k_cmPerIn);

    const int xpos = qRound(useInches ? resX : 2.0 * resX / CentimeterUnits::k_cmPerIn);
    const int ypos = qRound(useInches ? resY : 2.0 * resY / CentimeterUnits::k_cmPerIn);
    m_hCaliper->setJawPosition(xpos);
    m_vCaliper->setJawPosition(ypos);
}

void CalibrationPrefsPage::initialize() {
    const int currentScreenIndex = m_screenInfo->screenForWindow(this);
    m_model->initialize(currentScreenIndex);
}

void CalibrationPrefsPage::apply() {
    m_model->apply();
}

[[nodiscard]] bool CalibrationPrefsPage::isDirty() const {
    return m_model->isDirty();
}
