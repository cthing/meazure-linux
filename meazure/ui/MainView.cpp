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
#include "ToolDataSection.h"
#include "ScreenDataSection.h"
#include "MagnifierSection.h"
#include <QVBoxLayout>


MainView::MainView() : m_magnifierSection(new MagnifierSection) {
    auto* layout = new QVBoxLayout();

    auto* toolDataSection = new ToolDataSection();
    layout->addWidget(toolDataSection);

    auto* screenDataSection = new ScreenDataSection();
    layout->addWidget(screenDataSection);

    layout->addWidget(m_magnifierSection);

    setLayout(layout);
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
