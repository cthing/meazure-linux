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

#include "GridDialog.h"
#include <meazure/graphics/Grid.h>
#include <meazure/utils/LayoutUtils.h>
#include <meazure/utils/HelpUtils.h>
#include <QLabel>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QWhatsThis>
#include <cmath>

GridDialog::GridDialog(GridTool* gridTool, const ScreenInfoProvider* screenInfoProvider,  // NOLINT(cppcoreguidelines-pro-type-member-init)
                       const UnitsMgr* unitsMgr, QWidget *parent) :
        QDialog(parent),
        m_gridTool(gridTool),
        m_screenInfo(screenInfoProvider),
        m_unitsMgr(unitsMgr) {
    setWindowTitle(tr("Grid Adjust"));

    createUI();
    configureSpacingUI();
    configureOrientationUI();

    connect(m_gridTool, &GridTool::spacingChanged, this, &GridDialog::spacingChanged);

    connect(m_hSpacingField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setHorizontalSpacing);
    connect(m_vSpacingField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setVerticalSpacing);
    connect(m_linkSpacingCheck, &QCheckBox::clicked, this, &GridDialog::linkSpacing);
    connect(m_spacingUnitsCombo, &QComboBox::activated, this, &GridDialog::unitsSelected);

    connect(m_xPositionField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setXPosition);
    connect(m_yPositionField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setYPosition);
    connect(m_widthField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setWidth);
    connect(m_heightField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setHeight);
    connect(m_angleField, &DoubleDataField::valueChanged, m_gridTool, &GridTool::setAngle);
}

void GridDialog::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* spacingLabel = new QLabel(tr("<b>Grid Spacing</b>"));

    auto* horizontalLabel = new QLabel(tr("H:"));
    m_hSpacingField = new DoubleDataField(k_fieldShortWidth, true);
    m_hSpacingField->setWhatsThis(tr("Height of a grid cell."));
    auto* verticalLabel = new QLabel(tr("V:"));
    m_vSpacingField = new DoubleDataField(k_fieldShortWidth, true);
    m_vSpacingField->setWhatsThis(tr("Width of a grid cell."));
    m_spacingUnitsCombo = new QComboBox();
    m_spacingUnitsCombo->setWhatsThis(tr("Units for the grid cell width and height."));
    auto* spacingUnitsLayout = new QHBoxLayout();
    spacingUnitsLayout->addWidget(m_vSpacingField);
    spacingUnitsLayout->addWidget(m_spacingUnitsCombo);

    m_linkSpacingCheck = new QCheckBox(tr("Link horizontal and vertical"));
    m_linkSpacingCheck->setWhatsThis(tr("Ensures the grid cell width is used for the cell height."));

    auto* orientationLabel = new QLabel(tr("<b>Grid Orientation</b>"));

    auto* xPositionLabel = new QLabel(tr("X:"));
    m_xPositionField = new DoubleDataField(k_fieldShortWidth, true);
    m_xPositionField->setWhatsThis(tr("Origin of the grid along the x-axis, in pixels."));
    auto* yPositionLabel = new QLabel(tr("Y:"));
    m_yPositionField = new DoubleDataField(k_fieldShortWidth, true);
    m_yPositionField->setWhatsThis(tr("Origin of the grid along the y-axis, in pixels."));
    auto* positionUnitsLabel = new QLabel(tr("px"));
    auto* xyLayout = new QHBoxLayout();
    xyLayout->addWidget(m_yPositionField);
    xyLayout->addWidget(positionUnitsLabel);

    auto* widthLabel = new QLabel(tr("W:"));
    m_widthField = new DoubleDataField(k_fieldShortWidth, true);
    m_widthField->setWhatsThis(tr("Overall width of the grid, in pixels."));
    auto* heightLabel = new QLabel(tr("H:"));
    m_heightField = new DoubleDataField(k_fieldShortWidth, true);
    m_heightField->setWhatsThis(tr("Overall height of the grid, in pixels."));
    auto* whUnitsLabel = new QLabel(tr("px"));
    auto* whLayout = new QHBoxLayout();
    whLayout->addWidget(m_heightField);
    whLayout->addWidget(whUnitsLabel);

    auto* angleLabel = new QLabel(tr("A:"));
    m_angleField = new DoubleDataField(k_fieldShortWidth, true);
    m_angleField->setWhatsThis(tr("Angle of the grid, in degrees."));
    auto* angleUnitLabel = new QLabel(tr("deg"));
    auto* angleLayout = new QHBoxLayout();
    angleLayout->addWidget(m_angleField);
    angleLayout->addWidget(angleUnitLabel);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel
            | QDialogButtonBox::RestoreDefaults | QDialogButtonBox::Help);

    auto* okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setToolTip(tr("Accept grid settings"));
    okButton->setWhatsThis(tr("Accepts the grid settings and closes the dialog."));

    auto* cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setToolTip(tr("Discard grid settings"));
    cancelButton->setWhatsThis(tr("Discards the grid settings and closes the dialog."));

    auto* restoreButton = buttonBox->button(QDialogButtonBox::RestoreDefaults);
    restoreButton->setText(tr("Defaults"));
    restoreButton->setToolTip(tr("Restore default grid settings"));
    restoreButton->setWhatsThis(tr("Restores defaults for all grid settings."));

    auto* helpButton = buttonBox->button(QDialogButtonBox::Help);
    helpButton->setText("");
    helpButton->setIcon(HelpUtils::getHelpIcon(style()));

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &GridDialog::reject);
    connect(restoreButton, &QPushButton::clicked, this, &GridDialog::softReset);
    connect(helpButton, &QPushButton::clicked, this, []() {
        QWhatsThis::enterWhatsThisMode();
    });

    auto* layout = new QGridLayout();

    layout->addWidget(spacingLabel, k_row0, k_col0, k_rowspan1, k_colspan5, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(horizontalLabel,    k_row1, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_hSpacingField,    k_row1, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(verticalLabel,      k_row1, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(spacingUnitsLayout, k_row1, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_linkSpacingCheck, k_row2, k_col1, k_rowspan1, k_colspan4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(orientationLabel, k_row3, k_col0, k_rowspan1, k_colspan5, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(xPositionLabel,   k_row4, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_xPositionField, k_row4, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(yPositionLabel,   k_row4, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(xyLayout,         k_row4, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(widthLabel,   k_row5, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_widthField, k_row5, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(heightLabel,  k_row5, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(whLayout,     k_row5, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(angleLabel,  k_row6, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(angleLayout, k_row6, k_col1, k_rowspan1, k_colspan4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(buttonBox, k_row7, k_col0, k_rowspan1, k_colspan5);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

void GridDialog::configureSpacingUI() {
    for (const LinearUnitsId unitsId : LinearUnitsIdIter()) {
        m_spacingUnitsCombo->addItem(m_unitsMgr->getLinearUnits(unitsId)->getLengthLabel(), unitsId);
    }

    customUnitsChanged();
    connect(m_unitsMgr, &UnitsMgr::customUnitsChanged, this, &GridDialog::customUnitsChanged);
}

void GridDialog::configureOrientationUI() {
    m_xPositionField->setDecimalsQuietly(0);
    m_xPositionField->setSingleStep(1);

    m_yPositionField->setDecimalsQuietly(0);
    m_yPositionField->setSingleStep(1);

    m_widthField->setDecimalsQuietly(0);
    m_widthField->setRangeQuietly(GridTool::k_minSize, GridTool::k_maxSize);
    m_widthField->setSingleStep(1);

    m_heightField->setDecimalsQuietly(0);
    m_heightField->setRangeQuietly(GridTool::k_minSize, GridTool::k_maxSize);
    m_heightField->setSingleStep(1);

    m_angleField->setDecimalsQuietly(0);
    m_angleField->setRangeQuietly(0, 359);
    m_angleField->setWrapping(true);
    m_angleField->setSingleStep(1);
}

void GridDialog::updateUI() {
    spacingChanged(m_gridTool->getHorizontalSpacing(), m_gridTool->getVerticalSpacing(),
                   m_gridTool->isLinkedSpacing(), m_gridTool->getSpacingUnits());

    m_xPositionField->setValueQuietly(m_gridTool->getXPosition());
    m_yPositionField->setValueQuietly(m_gridTool->getYPosition());
    m_widthField->setValueQuietly(m_gridTool->getWidth());
    m_heightField->setValueQuietly(m_gridTool->getHeight());
    m_angleField->setValueQuietly(m_gridTool->getAngle());
}

void GridDialog::unitsSelected(int index) {
    const LinearUnitsId requestedUnitsId = static_cast<LinearUnitsId>(m_spacingUnitsCombo->itemData(index).toUInt());
    const LinearUnitsId existingUnitsId = m_gridTool->getSpacingUnits();

    const LinearUnits* requestedUnits = m_unitsMgr->getLinearUnits(requestedUnitsId);
    const LinearUnits* existingUnits = m_unitsMgr->getLinearUnits(existingUnitsId);

    const QSizeF res = m_screenInfo->getScreenRes(0);
    const int hspacePixels = existingUnits->convertToPixels(res, m_gridTool->getHorizontalSpacing(), 1).width();
    const int vspacePixels = existingUnits->convertToPixels(res, m_gridTool->getVerticalSpacing(), 1).height();

    const double hspace = requestedUnits->fromPixels(res).width() * hspacePixels;
    const double vspace = requestedUnits->fromPixels(res).height() * vspacePixels;

    const bool link = m_gridTool->isLinkedSpacing();

    m_gridTool->setSpacing(hspace, vspace, link, requestedUnitsId);
}

void GridDialog::spacingChanged(double hspace, double vspace, bool linkSpacing, LinearUnitsId unitsId) {
    const int idx = m_spacingUnitsCombo->findData(unitsId);
    if (idx != -1) {
        m_spacingUnitsCombo->setCurrentIndex(idx);
    }
    const LinearUnits* units = m_unitsMgr->getLinearUnits(unitsId);
    const int widthPrecision = units->getDisplayPrecision(Width);
    if (m_hSpacingField->decimals() != widthPrecision) {
        m_hSpacingField->setDecimalsQuietly(widthPrecision);
    }
    const int heightPrecision = units->getDisplayPrecision(Height);
    if (m_vSpacingField->decimals() != heightPrecision) {
        m_vSpacingField->setDecimalsQuietly(heightPrecision);
    }

    const double hstep = std::pow(10, -widthPrecision);
    const double vstep = std::pow(10, -heightPrecision);
    if (m_hSpacingField->minimum() != hstep) {
        m_hSpacingField->setMinimumQuietly(hstep);
    }
    if (m_vSpacingField->minimum() != vstep) {
        m_vSpacingField->setMinimumQuietly(vstep);
    }
    if (m_hSpacingField->singleStep() != hstep) {
        m_hSpacingField->setSingleStep(hstep);
    }
    if (m_vSpacingField->singleStep() != vstep) {
        m_vSpacingField->setSingleStep(vstep);
    }

    if (m_hSpacingField->value() != hspace) {
        m_hSpacingField->setValueQuietly(hspace);
    }
    if (m_vSpacingField->value() != vspace) {
        m_vSpacingField->setValueQuietly(vspace);
    }

    if (m_vSpacingField->isReadOnly() != linkSpacing) {
        m_vSpacingField->setReadOnly(linkSpacing);
    }
    if (m_linkSpacingCheck->isChecked() != linkSpacing) {
        m_linkSpacingCheck->setChecked(linkSpacing);
    }
}

void GridDialog::linkSpacing(bool link) {
    m_vSpacingField->setReadOnly(link);
    m_gridTool->setLinkedSpacing(link);
}

void GridDialog::customUnitsChanged() {
    const CustomUnits* customUnits = m_unitsMgr->getCustomUnits();
    const int customItemIdx = m_spacingUnitsCombo->findData(CustomId);

    if (customItemIdx >= 0) {
        auto* model = dynamic_cast<QStandardItemModel*>(m_spacingUnitsCombo->model());
        const bool defined = customUnits->haveCustomUnits();

        m_spacingUnitsCombo->setItemText(customItemIdx, defined ? customUnits->getLengthLabel() : tr("[custom]"));
        model->item(customItemIdx)->setEnabled(defined);

        if (!defined && (m_spacingUnitsCombo->currentIndex() == customItemIdx)) {
            const int pixelItemIdx = m_spacingUnitsCombo->findData(PixelsId);
            unitsSelected(pixelItemIdx);
        }
    }
}

void GridDialog::saveState() {
    m_origX = m_gridTool->getXPosition();
    m_origY = m_gridTool->getYPosition();
    m_origWidth = m_gridTool->getWidth();
    m_origHeight = m_gridTool->getHeight();
    m_origAngle = m_gridTool->getAngle();
    m_origSpacingH = m_gridTool->getHorizontalSpacing();
    m_origSpacingV = m_gridTool->getVerticalSpacing();
    m_origLinkedSpacing = m_gridTool->isLinkedSpacing();
    m_origUnits = m_gridTool->getSpacingUnits();
}

void GridDialog::restoreState() {
    m_gridTool->setOrientation(m_origX, m_origY, m_origWidth, m_origHeight, m_origAngle);
    m_gridTool->setSpacing(m_origSpacingH, m_origSpacingV, m_origLinkedSpacing, m_origUnits);
}

void GridDialog::softReset() {
    m_gridTool->softReset();
    updateUI();
}

void GridDialog::showEvent(QShowEvent *event) {
    saveState();
    updateUI();
    QDialog::showEvent(event);
}

void GridDialog::reject() {
    restoreState();
    updateUI();
    QDialog::reject();
}
