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

#include "MainView.h"
#include <QVBoxLayout>


MainView::MainView(PrefsDialog* prefsDialog) :
        m_toolDataSection(new ToolDataSection()),
        m_screenDataSection(new ScreenDataSection(prefsDialog)),
        m_magnifierSection(new MagnifierSection) {
    auto* layout = new QVBoxLayout();
    setLayout(layout);

    layout->addWidget(m_toolDataSection);
    layout->addWidget(m_screenDataSection);
    layout->addWidget(m_magnifierSection);
}

QAction* MainView::getMagnifierZoomInAction() const {
    return m_magnifierSection->getZoomInAction();
}

QAction* MainView::getMagnifierZoomOutAction() const {
    return m_magnifierSection->getZoomOutAction();
}

QAction* MainView::getMagnifierGridAction() const {
    return m_magnifierSection->getGridAction();
}

QAction* MainView::getMagnifierFreezeAction() const {
    return m_magnifierSection->getFreezeAction();
}

std::vector<QAction*> MainView::getColorFormatActions() const {
    return m_magnifierSection->getColorFormatActions();
}

QAction* MainView::getCopyColorAction() const {
    return m_magnifierSection->getCopyColorAction();
}

ToolDataSection* MainView::getToolDataSection() const {
    return m_toolDataSection;
}

ScreenDataSection* MainView::getScreenDataSection() const {
    return m_screenDataSection;
}

MagnifierSection* MainView::getMagnifierSection() const {
    return m_magnifierSection;
}

void MainView::writeConfig(Config& config) const {
    m_magnifierSection->writeConfig(config);
}

void MainView::readConfig(const Config& config) {
    m_magnifierSection->readConfig(config);
}
