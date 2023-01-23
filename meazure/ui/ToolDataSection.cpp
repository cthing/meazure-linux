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

#include "ToolDataSection.h"
#include <meazure/tools/RadioToolTraits.h>
#include <meazure/units/Units.h>
#include <meazure/utils/LayoutUtils.h>
#include <QGridLayout>


ToolDataSection::ToolDataSection(const UnitsMgr* unitsMgr, const ToolMgr* toolMgr) : // NOLINT(cppcoreguidelines-pro-type-member-init)
        m_unitsMgr(unitsMgr) {
    createFields();

    connect(toolMgr, &ToolMgr::radioToolSelected, this, &ToolDataSection::radioToolSelected);
    connect(toolMgr, &ToolMgr::xy1PositionChanged, this, &ToolDataSection::xy1PositionChanged);
    connect(toolMgr, &ToolMgr::xy2PositionChanged, this, &ToolDataSection::xy2PositionChanged);
    connect(toolMgr, &ToolMgr::xyvPositionChanged, this, &ToolDataSection::xyvPositionChanged);
    connect(toolMgr, &ToolMgr::widthHeightChanged, this, &ToolDataSection::widthHeightChanged);
    connect(toolMgr, &ToolMgr::distanceChanged, this, &ToolDataSection::distanceChanged);
    connect(toolMgr, &ToolMgr::angleChanged, this, &ToolDataSection::angleChanged);
    connect(toolMgr, &ToolMgr::aspectChanged, this, &ToolDataSection::aspectChanged);
    connect(toolMgr, &ToolMgr::areaChanged, this, &ToolDataSection::areaChanged);

    connect(m_x1Field, &DoubleDataField::valueChanged, toolMgr, &ToolMgr::setX1Position);
    connect(m_y1Field, &DoubleDataField::valueChanged, toolMgr, &ToolMgr::setY1Position);
    connect(m_x2Field, &DoubleDataField::valueChanged, toolMgr, &ToolMgr::setX2Position);
    connect(m_y2Field, &DoubleDataField::valueChanged, toolMgr, &ToolMgr::setY2Position);
    connect(m_xvField, &DoubleDataField::valueChanged, toolMgr, &ToolMgr::setXVPosition);
    connect(m_yvField, &DoubleDataField::valueChanged, toolMgr, &ToolMgr::setYVPosition);

    connect(m_x1Field, &DoubleDataField::stepRequested, toolMgr, &ToolMgr::stepX1Position);
    connect(m_y1Field, &DoubleDataField::stepRequested, toolMgr, &ToolMgr::stepY1Position);
    connect(m_x2Field, &DoubleDataField::stepRequested, toolMgr, &ToolMgr::stepX2Position);
    connect(m_y2Field, &DoubleDataField::stepRequested, toolMgr, &ToolMgr::stepY2Position);
    connect(m_xvField, &DoubleDataField::stepRequested, toolMgr, &ToolMgr::stepXVPosition);
    connect(m_yvField, &DoubleDataField::stepRequested, toolMgr, &ToolMgr::stepYVPosition);

    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, &ToolDataSection::linearUnitsChanged);
    connect(m_unitsMgr, &UnitsMgr::precisionsChanged, this, &ToolDataSection::linearUnitsChanged);
    connect(m_unitsMgr, &UnitsMgr::angularUnitsChanged, this, &ToolDataSection::angularUnitsChanged);
    connect(m_unitsMgr, &UnitsMgr::precisionsChanged, this, &ToolDataSection::angularUnitsChanged);
}

void ToolDataSection::createFields() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    m_x1Label = new QLabel(tr("X1:"));
    m_x1Field = new DoubleDataField(k_fieldShortWidth, true, false, false);

    m_y1Label = new QLabel(tr("Y1:"));
    m_y1Field = new DoubleDataField(k_fieldShortWidth, true, false, false);
    m_y1Units = new QLabel();
    auto* y1Layout = new QHBoxLayout();
    y1Layout->addWidget(m_y1Field);
    y1Layout->addWidget(m_y1Units);

    m_x2Label = new QLabel(tr("X2:"));
    m_x2Field = new DoubleDataField(k_fieldShortWidth, true, false, false);

    m_y2Label = new QLabel(tr("Y2:"));
    m_y2Field = new DoubleDataField(k_fieldShortWidth, true, false, false);
    m_y2Units = new QLabel();
    auto* y2Layout = new QHBoxLayout();
    y2Layout->addWidget(m_y2Field);
    y2Layout->addWidget(m_y2Units);

    m_xvLabel = new QLabel(tr("XV:"));
    m_xvField = new DoubleDataField(k_fieldShortWidth, true, false, false);

    m_yvLabel = new QLabel(tr("YV:"));
    m_yvField = new DoubleDataField(k_fieldShortWidth, true, false, false);
    m_yvUnits = new QLabel();
    auto* yvLayout = new QHBoxLayout();
    yvLayout->addWidget(m_yvField);
    yvLayout->addWidget(m_yvUnits);

    m_wLabel = new QLabel(tr("W:"));
    m_wField = new DoubleDataField(k_fieldShortWidth, false);

    m_hLabel = new QLabel(tr("H:"));
    m_hField = new DoubleDataField(k_fieldShortWidth, false);
    m_hUnits = new QLabel();
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_hField);
    hLayout->addWidget(m_hUnits);

    m_dLabel = new QLabel(tr("D:"));
    m_dField = new DoubleDataField(k_fieldShortWidth, false);
    m_dUnits = new QLabel();
    auto* dLayout = new QHBoxLayout();
    dLayout->addWidget(m_dField);
    dLayout->addWidget(m_dUnits);

    m_aLabel = new QLabel(tr("A:"));
    m_aField = new DoubleDataField(k_fieldShortWidth, false);
    m_aUnits = new QLabel();
    auto* aLayout = new QHBoxLayout();
    aLayout->addWidget(m_aField);
    aLayout->addWidget(m_aUnits);

    m_asLabel = new QLabel(tr("As:"));
    m_asField = new DoubleDataField(k_fieldLongWidth, false);

    m_arLabel = new QLabel(tr("Ar:"));
    m_arField = new DoubleDataField(k_fieldLongWidth, false);
    m_arUnits = new QLabel();
    auto* arLayout = new QHBoxLayout();
    arLayout->addWidget(m_arField);
    arLayout->addWidget(m_arUnits);

    auto* layout = new QGridLayout();
    layout->addWidget(m_x1Label, k_row0, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_x1Field, k_row0, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_y1Label, k_row0, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(y1Layout,  k_row0, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_x2Label, k_row1, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_x2Field, k_row1, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_y2Label, k_row1, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(y2Layout,  k_row1, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_xvLabel, k_row2, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_xvField, k_row2, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_yvLabel, k_row2, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(yvLayout,  k_row2, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_wLabel, k_row3, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_wField, k_row3, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_hLabel, k_row3, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(hLayout,  k_row3, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_dLabel, k_row4, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(dLayout,  k_row4, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_aLabel, k_row4, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(aLayout,  k_row4, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_asLabel, k_row5, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_asField, k_row5, k_col1, k_rowspan1, k_colspan4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(m_arLabel, k_row6, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(arLayout,  k_row6, k_col1, k_rowspan1, k_colspan4, Qt::AlignLeft | Qt::AlignVCenter);

    setLayout(layout);
}

void ToolDataSection::radioToolSelected(RadioTool& tool) {
    const RadioToolTraits traits = tool.getTraits();

    auto configure = [&traits](RadioToolTrait availableTrait, RadioToolTrait readOnlyTrait, QLabel* label,
                               DoubleDataField* field, QLabel* units = nullptr) {
        const bool enabled = ((traits & availableTrait) != 0);
        const bool readOnly = ((traits & readOnlyTrait) != 0);

        label->setEnabled(enabled);
        field->setEnabled(enabled);
        if (enabled) {
            field->setReadOnly(readOnly);
        }
        if (units != nullptr) {
            units->setEnabled(enabled);
        }
    };

    configure(XY1Available, XY1ReadOnly, m_x1Label, m_x1Field);
    configure(XY1Available, XY1ReadOnly, m_y1Label, m_y1Field, m_y1Units);

    configure(XY2Available, XY2ReadOnly, m_x2Label, m_x2Field);
    configure(XY2Available, XY2ReadOnly, m_y2Label, m_y2Field, m_y2Units);

    configure(XYVAvailable, XYVReadOnly, m_xvLabel, m_xvField);
    configure(XYVAvailable, XYVReadOnly, m_yvLabel, m_yvField, m_yvUnits);

    configure(WHAvailable, WHReadOnly, m_wLabel, m_wField);
    configure(WHAvailable, WHReadOnly, m_hLabel, m_hField, m_hUnits);

    configure(DistAvailable, DistReadOnly, m_dLabel, m_dField, m_dUnits);
    configure(AngleAvailable, AngleReadOnly, m_aLabel, m_aField, m_aUnits);
    configure(AreaAvailable, AreaReadOnly, m_arLabel, m_arField, m_arUnits);
    configure(AspectAvailable, AspectReadOnly, m_asLabel, m_asField);
}

void ToolDataSection::linearUnitsChanged() {
    const LinearUnits* linearUnits = m_unitsMgr->getLinearUnits();

    const QString lengthLabel = linearUnits->getLengthLabel();
    m_y1Units->setText(lengthLabel);
    m_y2Units->setText(lengthLabel);
    m_yvUnits->setText(lengthLabel);
    m_hUnits->setText(lengthLabel);
    m_dUnits->setText(lengthLabel);
    m_arUnits->setText(linearUnits->getAreaLabel());

    m_x1Field->setDecimalsQuietly(linearUnits->getDisplayPrecision(XCoord));
    m_y1Field->setDecimalsQuietly(linearUnits->getDisplayPrecision(YCoord));
    m_x2Field->setDecimalsQuietly(linearUnits->getDisplayPrecision(XCoord));
    m_y2Field->setDecimalsQuietly(linearUnits->getDisplayPrecision(YCoord));
    m_xvField->setDecimalsQuietly(linearUnits->getDisplayPrecision(XCoord));
    m_yvField->setDecimalsQuietly(linearUnits->getDisplayPrecision(YCoord));
    m_wField->setDecimals(linearUnits->getDisplayPrecision(Width));
    m_hField->setDecimals(linearUnits->getDisplayPrecision(Height));
    m_dField->setDecimals(linearUnits->getDisplayPrecision(Distance));
    m_asField->setDecimals(LinearUnits::k_aspectPrecision);
    m_arField->setDecimals(linearUnits->getDisplayPrecision(Area));
}

void ToolDataSection::angularUnitsChanged() {
    const AngularUnits* angularUnits = m_unitsMgr->getAngularUnits();

    m_aUnits->setText(angularUnits->getLabel());

    m_aField->setDecimals(angularUnits->getDisplayPrecision(Angle));
}

void ToolDataSection::xy1PositionChanged(QPointF coord, QPoint) {
    m_x1Field->setValueQuietly(coord.x());
    m_y1Field->setValueQuietly(coord.y());
}

void ToolDataSection::xy2PositionChanged(QPointF coord, QPoint) {
    m_x2Field->setValueQuietly(coord.x());
    m_y2Field->setValueQuietly(coord.y());
}

void ToolDataSection::xyvPositionChanged(QPointF coord, QPoint) {
    m_xvField->setValueQuietly(coord.x());
    m_yvField->setValueQuietly(coord.y());
}

void ToolDataSection::widthHeightChanged(QSizeF widthHeight) {
    m_wField->setValue(widthHeight.width());
    m_hField->setValue(widthHeight.height());
}

void ToolDataSection::distanceChanged(double distance) {
    m_dField->setValue(distance);
}

void ToolDataSection::angleChanged(double angle) {
    m_aField->setValue(angle);
}

void ToolDataSection::aspectChanged(double aspect) {
    m_asField->setValue(aspect);
}

void ToolDataSection::areaChanged(double area) {
    m_arField->setValue(area);
}
