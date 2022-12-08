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
#include <QToolButton>
#include <QIcon>
#include <QKeySequence>


MagnifierSection::MagnifierSection() {
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    auto* magnifierLayout = new QHBoxLayout();
    auto* magnifier = new Magnifier();
    magnifierLayout->addWidget(magnifier);
    layout->addLayout(magnifierLayout);

    m_zoomInAction = new QAction(tr("Zoom In"), this);
    m_zoomInAction->setShortcut(QKeySequence(QKeySequence::ZoomIn));
    connect(m_zoomInAction, &QAction::triggered, magnifier, &Magnifier::zoomIn);
    connect(magnifier, &Magnifier::zoomChanged, this, [this](int zoomIndex) {
        m_zoomInAction->setEnabled(zoomIndex < static_cast<int>(Magnifier::getZoomFactors().size() - 1));
    });

    m_zoomOutAction = new QAction(tr("Zoom Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    connect(m_zoomOutAction, &QAction::triggered, magnifier, &Magnifier::zoomOut);
    connect(magnifier, &Magnifier::zoomChanged, this, [this](int zoomIndex) {
        m_zoomOutAction->setEnabled(zoomIndex > 0);
    });

    m_gridAction = new QAction(tr("Magnifier &Grid"), this);
    m_gridAction->setCheckable(true);
    connect(m_gridAction, &QAction::triggered, magnifier, &Magnifier::setGrid);
    connect(magnifier, &Magnifier::gridChanged, m_gridAction, &QAction::setChecked);

    auto* colorLayout = new QHBoxLayout();

    auto* colorData = new ColorData();
    colorLayout->addWidget(colorData);

    auto* copyButton = new QPushButton(QIcon(":/images/Clipboard.svg"), "");
    copyButton->setIconSize(QSize(k_buttonIconSize, k_buttonIconSize));
    copyButton->setFixedSize(QSize(k_buttonSize, k_buttonSize));
    colorLayout->addWidget(copyButton);

    QIcon freezeIcon;
    freezeIcon.addFile(":/images/PauseOff.svg", QSize(), QIcon::Normal, QIcon::Off);
    freezeIcon.addFile(":/images/PauseOn.svg", QSize(), QIcon::Normal, QIcon::On);

    m_freezeAction = new QAction(tr("&Freeze Magnifier"), this);
    m_freezeAction->setCheckable(true);
    m_freezeAction->setShortcut(QKeySequence("Ctrl+M"));
    m_freezeAction->setIcon(freezeIcon);
    connect(m_freezeAction, &QAction::triggered, magnifier, &Magnifier::setFreeze);
    connect(magnifier, &Magnifier::freezeChanged, m_freezeAction, &QAction::setChecked);

    auto* freezeButton = new QToolButton();
    freezeButton->setDefaultAction(m_freezeAction);
    freezeButton->setIconSize(QSize(k_buttonIconSize, k_buttonIconSize));
    freezeButton->setFixedSize(QSize(k_buttonSize, k_buttonSize));
    freezeButton->setToolTip(tr("Freeze magnifier"));
    colorLayout->addWidget(freezeButton);
    layout->addLayout(colorLayout);

    auto* zoom = new MagnifierZoom(magnifier);
    layout->addWidget(zoom);

    magnifier->setGrid(k_initialShowGrid);
    magnifier->setFreeze(k_initialFreeze);
    magnifier->setZoom(k_initialZoomIndex);
}
