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
#include <QApplication>
#include <meazure/App.h>


MainView::MainView() {
    auto* layout = new QVBoxLayout();

    auto* toolDataSection = new ToolDataSection();
    layout->addWidget(toolDataSection);

    auto* screenDataSection = new ScreenDataSection();
    layout->addWidget(screenDataSection);

    auto* magnifierSection = new MagnifierSection();
    layout->addWidget(magnifierSection);

    setLayout(layout);

    App* app = reinterpret_cast<App*>(QApplication::instance());

    m_crossHair = new CrossHair(app->getScreenInfo(), app->getUnitsMgr(), nullptr, 100, "Hello World");
    m_crossHair->show();
    m_crossHair->setPosition(QPoint(500, 700));
    m_crossHair->flash();
}

MainView::~MainView() {
    delete m_crossHair;
}
