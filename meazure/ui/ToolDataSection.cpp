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
#include "DataField.h"
#include <QLabel>
#include <QGridLayout>


ToolDataSection::ToolDataSection() {
    createFields();
}

void ToolDataSection::createFields() {
    auto* x1Label = new QLabel(tr("X1:"));
    auto* x1Edit = new DataField(fieldShortWidth, true);

    auto* y1Label = new QLabel(tr("Y1:"));
    auto* y1Edit = new DataField(fieldShortWidth, true);
    auto* y1Units = new QLabel(tr("in"));
    auto* y1Layout = new QHBoxLayout();
    y1Layout->addWidget(y1Edit);
    y1Layout->addWidget(y1Units);

    auto* x2Label = new QLabel(tr("X2:"));
    auto* x2Edit = new DataField(fieldShortWidth, true);

    auto* y2Label = new QLabel(tr("Y2:"));
    auto* y2Edit = new DataField(fieldShortWidth, true);
    auto* y2Units = new QLabel(tr("in"));
    auto* y2Layout = new QHBoxLayout();
    y2Layout->addWidget(y2Edit);
    y2Layout->addWidget(y2Units);

    auto* xvLabel = new QLabel(tr("XV:"));
    auto* xvEdit = new DataField(fieldShortWidth, true);

    auto* yvLabel = new QLabel(tr("YV:"));
    auto* yvEdit = new DataField(fieldShortWidth, true);
    auto* yvUnits = new QLabel(tr("in"));
    auto* yvLayout = new QHBoxLayout();
    yvLayout->addWidget(yvEdit);
    yvLayout->addWidget(yvUnits);

    auto* wLabel = new QLabel(tr("W:"));
    auto* wEdit = new DataField(fieldShortWidth, false);

    auto* hLabel = new QLabel(tr("H:"));
    auto* hEdit = new DataField(fieldShortWidth, false);
    auto* hUnits = new QLabel(tr("in"));
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(hEdit);
    hLayout->addWidget(hUnits);

    auto* dLabel = new QLabel(tr("D:"));
    auto* dEdit = new DataField(fieldShortWidth, false);
    auto* dUnits = new QLabel(tr("in"));
    auto* dLayout = new QHBoxLayout();
    dLayout->addWidget(dEdit);
    dLayout->addWidget(dUnits);

    auto* aLabel = new QLabel(tr("A:"));
    auto* aEdit = new DataField(fieldShortWidth, false);
    auto* aUnits = new QLabel(tr("deg"));
    auto* aLayout = new QHBoxLayout();
    aLayout->addWidget(aEdit);
    aLayout->addWidget(aUnits);

    auto* asLabel = new QLabel(tr("As:"));
    auto* asEdit = new DataField(fieldLongWidth, false);

    auto* arLabel = new QLabel(tr("Ar:"));
    auto* arEdit = new DataField(fieldLongWidth, false);
    auto* arUnits = new QLabel(tr("sq. in"));
    auto* arLayout = new QHBoxLayout();
    arLayout->addWidget(arEdit);
    arLayout->addWidget(arUnits);

    auto* layout = new QGridLayout();
    layout->addWidget(x1Label, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(x1Edit, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(y1Label, 0, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(y1Layout, 0, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(x2Label, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(x2Edit, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(y2Label, 1, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(y2Layout, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(xvLabel, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(xvEdit, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(yvLabel, 2, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(yvLayout, 2, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(wLabel, 3, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(wEdit, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(hLabel, 3, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(hLayout, 3, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(dLabel, 4, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(dLayout, 4, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(aLabel, 4, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(aLayout, 4, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(asLabel, 5, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(asEdit, 5, 1, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(arLabel, 6, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(arLayout, 6, 1, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    setLayout(layout);
}
