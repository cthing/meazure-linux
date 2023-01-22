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
#include "MagnifierControls.h"
#include <QHBoxLayout>
#include <QKeySequence>


MagnifierSection::MagnifierSection() :
        m_magnifier(new Magnifier()),
        m_magnifierControls(new MagnifierControls(m_magnifier)),
        m_colorDisplay(new ColorDisplay()) {
    auto* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 5, 0, 0);
    setLayout(layout);

    // Use an HBox to center magnifier horizontally
    auto* magnifierLayout = new QHBoxLayout();
    magnifierLayout->addWidget(m_magnifier);
    layout->addLayout(magnifierLayout);

    layout->addWidget(m_magnifierControls);

    m_zoomInAction = new QAction(tr("Zoom In"), this);
    m_zoomInAction->setShortcut(QKeySequence(QKeySequence::ZoomIn));
    connect(m_zoomInAction, &QAction::triggered, m_magnifier, &Magnifier::zoomIn);
    connect(m_magnifier, &Magnifier::zoomChanged, this, [this](int zoomIndex) {
        m_zoomInAction->setEnabled(zoomIndex < static_cast<int>(Magnifier::getZoomFactors().size() - 1));
    });

    m_zoomOutAction = new QAction(tr("Zoom Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    connect(m_zoomOutAction, &QAction::triggered, m_magnifier, &Magnifier::zoomOut);
    connect(m_magnifier, &Magnifier::zoomChanged, this, [this](int zoomIndex) {
        m_zoomOutAction->setEnabled(zoomIndex > 0);
    });

    m_gridAction = new QAction(tr("Magnifier &Grid"), this);
    m_gridAction->setCheckable(true);
    connect(m_gridAction, &QAction::triggered, m_magnifier, &Magnifier::setGrid);
    connect(m_magnifier, &Magnifier::gridChanged, m_gridAction, &QAction::setChecked);

    layout->addWidget(m_colorDisplay);
    connect(m_magnifier, &Magnifier::currentColorChanged, m_colorDisplay, &ColorDisplay::setColor);

    m_magnifier->setGrid(k_initialShowGrid);
    m_magnifier->setFreeze(k_initialFreeze);
    m_magnifier->setZoom(k_initialZoomIndex);
    m_colorDisplay->setColorFormat(k_initialColorFmt);
}

void MagnifierSection::saveToProfile(Profile& profile) const {
    m_magnifier->saveToProfile(profile);
    m_colorDisplay->saveToProfile(profile);
}

void MagnifierSection::loadFromProfile(Profile& profile) {
    m_magnifier->loadFromProfile(profile);
    m_colorDisplay->saveToProfile(profile);
}

void MagnifierSection::hardReset() {
    m_magnifier->setGrid(k_initialShowGrid);
    m_magnifier->setFreeze(k_initialFreeze);
    m_magnifier->setZoom(k_initialZoomIndex);
    m_colorDisplay->setColorFormat(RGBFmt);
}
