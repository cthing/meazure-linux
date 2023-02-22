/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "MagnifierSection.h"
#include "MagnifierControls.h"
#include <QHBoxLayout>
#include <QKeySequence>
#include <QActionGroup>


MagnifierSection::MagnifierSection(const ScreenInfoProvider* screenInfo, const ToolMgr* toolMgr) :
        m_magnifier(new Magnifier(screenInfo, toolMgr)),
        m_magnifierControls(new MagnifierControls(m_magnifier, screenInfo)),
        m_colorDisplay(new ColorDisplay(screenInfo)) {
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
    m_zoomInAction->setStatusTip(tr("Increase magnifier zoom"));
    m_zoomInAction->setWhatsThis(tr("Increases the magnifier zoom around the active position."));
    connect(m_zoomInAction, &QAction::triggered, m_magnifier, &Magnifier::zoomIn);
    connect(m_magnifier, &Magnifier::zoomChanged, this, [this](int zoomIndex) {
        m_zoomInAction->setEnabled(zoomIndex < static_cast<int>(Magnifier::getZoomFactors().size() - 1));
    });

    m_zoomOutAction = new QAction(tr("Zoom Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    m_zoomOutAction->setStatusTip(tr("Decrease magnifier zoom"));
    m_zoomOutAction->setWhatsThis(tr("Decreases the magnifier zoom around the active position."));
    connect(m_zoomOutAction, &QAction::triggered, m_magnifier, &Magnifier::zoomOut);
    connect(m_magnifier, &Magnifier::zoomChanged, this, [this](int zoomIndex) {
        m_zoomOutAction->setEnabled(zoomIndex > 0);
    });

    auto* gridTypeGroup = new QActionGroup(this);
    gridTypeGroup->setExclusive(true);

    auto createGridAction = [this, gridTypeGroup](const QString& text, const QString& statusTip,
            const QString& whatsThis, Magnifier::GridType gridType) {
        auto* action = new QAction(text, gridTypeGroup);
        action->setCheckable(true);
        action->setStatusTip(statusTip);
        action->setWhatsThis(whatsThis);
        connect(action, &QAction::triggered, this, [this, gridType]() {
            m_magnifier->setGridType(gridType);
        });
        connect(m_magnifier, &Magnifier::gridTypeChanged, this, [action, gridType](Magnifier::GridType type) {
            action->setChecked(type == gridType);
        });
        m_gridActions.push_back(action);
    };

    createGridAction(tr("&None"), tr("No magnifier grid"), tr("Do not show a grid in the magnifier."),
                     Magnifier::GridType::None);
    createGridAction(tr("&Dark"), tr("Dark colored grid"), tr("Use a dark colored grid in the magnifier."),
                     Magnifier::GridType::Dark);
    createGridAction(tr("&Light"), tr("Light colored grid"), tr("Use a light colored grid in the magnifier."),
                     Magnifier::GridType::Light);

    layout->addWidget(m_colorDisplay);
    connect(m_magnifier, &Magnifier::currentColorChanged, m_colorDisplay, &ColorDisplay::setColor);

    m_magnifier->setGridType(k_initialGridType);
    m_magnifier->setFreeze(k_initialFreeze);
    m_magnifier->setZoom(k_initialZoomIndex);
    m_colorDisplay->setColorFormat(k_initialColorFmt);
}

void MagnifierSection::writeConfig(Config& config) const {
    m_magnifier->writeConfig(config);
    m_colorDisplay->writeConfig(config);
}

void MagnifierSection::readConfig(const Config& config) {
    m_magnifier->readConfig(config);
    m_colorDisplay->readConfig(config);
}

void MagnifierSection::hardReset() {
    m_magnifier->setGridType(k_initialGridType);
    m_magnifier->setFreeze(k_initialFreeze);
    m_magnifier->setZoom(k_initialZoomIndex);
    m_colorDisplay->setColorFormat(RGBFmt);
}
