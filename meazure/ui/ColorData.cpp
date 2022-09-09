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

#include "ColorData.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>


ColorData::ColorData() {
    createControls();
}

void ColorData::createControls() {
    auto* layout = new QHBoxLayout();

    auto* colorSpace = new QLabel(tr("RGB:"));
    layout->addWidget(colorSpace);

    auto* colorField = new QLineEdit();
    layout->addWidget(colorField);

    auto* swatch = new QWidget();
    swatch->setFixedSize(70, 20);
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, Qt::red);
    swatch->setAutoFillBackground(true);
    swatch->setPalette(pal);
    layout->addWidget(swatch);

    setLayout(layout);
}
