/*
 * Copyright 2023 C Thing Software
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

#include "WaylandAlert.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

WaylandAlert::WaylandAlert() {
    setWindowTitle(tr("Wayland Not Supported"));

    auto* message = new QLabel(tr("<h2>Meazure is not supported on Wayland</h2>"
                                  "<p style=\"font-size: 10pt;\">"
                                  "Due to significant design limitations, Wayland<br>"
                                  "does not provide the capabilities required by<br>"
                                  "Meazure. It is hoped that these capabilities<br>"
                                  "will be added to Wayland in the near future.<br><br>"
                                  "To run Meazure, please use the X Window System.</p>"));

    auto* exitButton = new QPushButton(tr("Exit"));
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);

    auto* layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(message);
    layout->addSpacing(10);
    layout->addWidget(exitButton);
    layout->setAlignment(exitButton, Qt::AlignHCenter);
}
