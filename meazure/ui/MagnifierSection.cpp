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

#include "MagnifierSection.h"
#include "Magnifier.h"
#include "MagnifierZoom.h"
#include "ColorData.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>


MagnifierSection::MagnifierSection() {
    createComponents();
}

void MagnifierSection::createComponents() {
    auto* layout = new QVBoxLayout();

    auto* magnifier = new Magnifier();
    layout->addWidget(magnifier);

    auto* colorLayout = new QHBoxLayout();

    auto* colorData = new ColorData();
    colorLayout->addWidget(colorData);

    auto* copyButton = new QPushButton(QIcon(":/images/Clipboard.svg"), "");
    colorLayout->addWidget(copyButton);

    auto* pauseButton = new QPushButton(QIcon(":/images/Pause.svg"), "");
    colorLayout->addWidget(pauseButton);
    layout->addLayout(colorLayout);

    auto* zoom = new MagnifierZoom();

    layout->addWidget(zoom);

    setLayout(layout);
}
