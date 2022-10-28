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

#include "ScreenDataSection.h"
#include <meazure/units/UnitsMgr.h>
#include <meazure/App.h>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>


ScreenDataSection::ScreenDataSection() {    // NOLINT(cppcoreguidelines-pro-type-member-init)
    createFields();

    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, &ScreenDataSection::linearUnitsChanged);
}

void ScreenDataSection::createFields() {
    auto* wLabel = new QLabel(tr("W:"));
    m_wField = new DataField(k_fieldWidth, false, true);

    auto* hLabel = new QLabel(tr("H:"));
    m_hField = new DataField(k_fieldWidth, false, true);
    m_hUnits = new QLabel();
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_hField);
    hLayout->addWidget(m_hUnits);

    auto* calButton = new QPushButton(QIcon(":/images/CalWarning.svg"), "");
    calButton->setToolTip(tr("Calibrate screen resolution"));
    hLayout->addSpacing(10);
    hLayout->addWidget(calButton);

    auto* rxLabel = new QLabel(tr("Rx:"));
    m_rxField = new DataField(k_fieldWidth, false, true);

    auto* ryLabel = new QLabel(tr("Ry:"));
    m_ryField = new DataField(k_fieldWidth, false, true);
    m_ryUnits = new QLabel();
    auto* ryLayout = new QHBoxLayout();
    ryLayout->addWidget(m_ryField);
    ryLayout->addWidget(m_ryUnits);

    auto* layout = new QGridLayout();
    layout->addWidget(wLabel, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_wField, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(hLabel, 0, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(hLayout, 0, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(rxLabel, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_rxField, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(ryLabel, 1, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(ryLayout, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    setLayout(layout);
}

void ScreenDataSection::linearUnitsChanged(LinearUnitsId) {
    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();
    const LinearUnits* linearUnits = unitsMgr.getLinearUnits();

    m_hUnits->setText(linearUnits->getLengthLabel());
    m_ryUnits->setText(linearUnits->getResLabel());

    m_wField->setDecimals(linearUnits->getDisplayPrecision(Width));
    m_hField->setDecimals(linearUnits->getDisplayPrecision(Height));
    m_rxField->setDecimals(linearUnits->getDisplayPrecision(ResX));
    m_ryField->setDecimals(linearUnits->getDisplayPrecision(ResY));
}
