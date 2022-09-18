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
#include "DataField.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>


ScreenDataSection::ScreenDataSection() {
    createFields();
}

void ScreenDataSection::createFields() {
    auto* wLabel = new QLabel(tr("W:"));
    auto* wEdit = new DataField(fieldWidth, false);

    auto* hLabel = new QLabel(tr("H:"));
    auto* hEdit = new DataField(fieldWidth, false);
    auto* hUnits = new QLabel(tr("in"));
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(hEdit);
    hLayout->addWidget(hUnits);

    auto* calButton = new QPushButton(QIcon(":/images/CalWarning.svg"), "");
    calButton->setToolTip(tr("Calibrate screen resolution"));
    hLayout->addSpacing(10);
    hLayout->addWidget(calButton);

    auto* rxLabel = new QLabel(tr("Rx:"));
    auto* rxEdit = new DataField(fieldWidth, false);

    auto* ryLabel = new QLabel(tr("Ry:"));
    auto* ryEdit = new DataField(fieldWidth, false);
    auto* ryUnits = new QLabel(tr("px / in"));
    auto* ryLayout = new QHBoxLayout();
    ryLayout->addWidget(ryEdit);
    ryLayout->addWidget(ryUnits);

    auto* layout = new QGridLayout();
    layout->addWidget(wLabel, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(wEdit, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(hLabel, 0, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(hLayout, 0, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(rxLabel, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(rxEdit, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(ryLabel, 1, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(ryLayout, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    setLayout(layout);
}
