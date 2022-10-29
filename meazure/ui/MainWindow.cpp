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

#include "MainWindow.h"
#include "MainView.h"
#include <meazure/App.h>
#include <meazure/tools/ToolMgr.h>
#include <meazure/tools/CursorTool.h>
#include <meazure/tools/PointTool.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/units/Units.h>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QActionGroup>


MainWindow::MainWindow() {      // NOLINT(cppcoreguidelines-pro-type-member-init)
    createCentralWidget();
    createActions();
    createMenus();
    createToolbar();

    const ToolMgr& toolMgr = App::instance()->getToolMgr();
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, &MainWindow::radioToolSelected);

    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, &MainWindow::linearUnitsChanged);
    connect(&unitsMgr, &UnitsMgr::angularUnitsChanged, this, &MainWindow::angularUnitsChanged);

    m_cursorToolAction->trigger();
    m_pixelUnitsAction->trigger();
    m_degreeUnitsAction->trigger();
}

void MainWindow::createActions() {
    ToolMgr& toolMgr = App::instance()->getToolMgr();
    UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();

    // Tool actions

    auto* radioToolGroup = new QActionGroup(this);
    radioToolGroup->setExclusive(true);

    m_cursorToolAction = new QAction(QIcon(":/images/CursorTool.svg"), tr("&Cursor"), radioToolGroup);
    m_cursorToolAction->setCheckable(true);
    m_cursorToolAction->setToolTip("Tracks cursor position");
    connect(m_cursorToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(CursorTool::k_toolName); });

    m_pointToolAction = new QAction(QIcon(":/images/PointTool.svg"), tr("&Point"), radioToolGroup);
    m_pointToolAction->setCheckable(true);
    m_pointToolAction->setToolTip("Measures a point");
    connect(m_pointToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(PointTool::k_toolName); });

    m_lineToolAction = new QAction(QIcon(":/images/LineTool.svg"), tr("&Line"), radioToolGroup);
    m_lineToolAction->setCheckable(true);
    m_lineToolAction->setToolTip("Measures using line");

    m_rectangleToolAction = new QAction(QIcon(":/images/RectangleTool.svg"), tr("&Rectangle"), radioToolGroup);
    m_rectangleToolAction->setCheckable(true);
    m_rectangleToolAction->setToolTip("Measures using rectangle");

    m_circleToolAction = new QAction(QIcon(":/images/CircleTool.svg"), tr("C&ircle"), radioToolGroup);
    m_circleToolAction->setCheckable(true);
    m_circleToolAction->setToolTip("Measures using circle");

    m_windowToolAction = new QAction(QIcon(":/images/WindowTool.svg"), tr("&Window"), radioToolGroup);
    m_windowToolAction->setCheckable(true);
    m_windowToolAction->setToolTip("Measures a window");

    m_rulerToolAction = new QAction(QIcon(":/images/RulerTool.svg"), tr("R&uler"), this);
    m_rulerToolAction->setCheckable(true);
    m_rulerToolAction->setToolTip("Adds screen rulers");

    m_gridToolAction = new QAction(QIcon(":/images/GridTool.svg"), tr("&Screen Grid"), this);
    m_gridToolAction->setCheckable(true);
    m_gridToolAction->setToolTip("Adds screen grid");

    // Units actions

    auto* linearUnitsGroup = new QActionGroup(this);
    linearUnitsGroup->setExclusive(true);

    m_pixelUnitsAction = new QAction(tr("&Pixels"), linearUnitsGroup);
    m_pixelUnitsAction->setCheckable(true);
    connect(m_pixelUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(PixelsId); });

    m_twipUnitsAction = new QAction(tr("&Twips"), linearUnitsGroup);
    m_twipUnitsAction->setCheckable(true);
    connect(m_twipUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(TwipsId); });

    m_pointUnitsAction = new QAction(tr("P&oints"), linearUnitsGroup);
    m_pointUnitsAction->setCheckable(true);
    connect(m_pointUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(PointsId); });

    m_picaUnitsAction = new QAction(tr("Pic&as"), linearUnitsGroup);
    m_picaUnitsAction->setCheckable(true);
    connect(m_picaUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(PicasId); });

    m_inchUnitsAction = new QAction(tr("&Inches"), linearUnitsGroup);
    m_inchUnitsAction->setCheckable(true);
    connect(m_inchUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(InchesId); });

    m_centimeterUnitsAction = new QAction(tr("&Centimeters"), linearUnitsGroup);
    m_centimeterUnitsAction->setCheckable(true);
    connect(m_centimeterUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(CentimetersId); });

    m_millimeterUnitsAction = new QAction(tr("&Millimeters"), linearUnitsGroup);
    m_millimeterUnitsAction->setCheckable(true);
    connect(m_millimeterUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(MillimetersId); });

    m_customUnitsAction = new QAction(tr("[custom]"), linearUnitsGroup);
    m_customUnitsAction->setCheckable(true);

    m_defineCustomUnitsAction = new QAction(tr("Define custom..."), linearUnitsGroup);
    m_defineCustomUnitsAction->setCheckable(true);

    auto* angularUnitsGroup = new QActionGroup(this);
    angularUnitsGroup->setExclusive(true);

    m_degreeUnitsAction = new QAction(tr("&Degrees"), angularUnitsGroup);
    m_degreeUnitsAction->setCheckable(true);
    connect(m_degreeUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setAngularUnits(DegreesId); });

    m_radianUnitsAction = new QAction(tr("&Radians"), angularUnitsGroup);
    m_radianUnitsAction->setCheckable(true);
    connect(m_radianUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setAngularUnits(RadiansId); });
}

void MainWindow::createMenus() {
    // File menu

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addSeparator();

    auto* exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit Meazure"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAct);

    // Tools menu

    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(m_cursorToolAction);
    toolsMenu->addAction(m_pointToolAction);
    toolsMenu->addAction(m_lineToolAction);
    toolsMenu->addAction(m_rectangleToolAction);
    toolsMenu->addAction(m_circleToolAction);
    toolsMenu->addAction(m_windowToolAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_rulerToolAction);
    toolsMenu->addAction(m_gridToolAction);

    // Units menu

    QMenu* unitsMenu = menuBar()->addMenu(tr("&Units"));
    unitsMenu->addAction(m_pixelUnitsAction);
    unitsMenu->addAction(m_twipUnitsAction);
    unitsMenu->addAction(m_pointUnitsAction);
    unitsMenu->addAction(m_picaUnitsAction);
    unitsMenu->addAction(m_inchUnitsAction);
    unitsMenu->addAction(m_centimeterUnitsAction);
    unitsMenu->addAction(m_millimeterUnitsAction);
    unitsMenu->addAction(m_customUnitsAction);
    unitsMenu->addAction(m_defineCustomUnitsAction);
    toolsMenu->addSeparator();
    unitsMenu->addAction(m_degreeUnitsAction);
    unitsMenu->addAction(m_radianUnitsAction);
}

void MainWindow::createToolbar() {
    QToolBar* toolBar = addToolBar("");
    toolBar->addAction(m_cursorToolAction);
    toolBar->addAction(m_pointToolAction);
    toolBar->addAction(m_lineToolAction);
    toolBar->addAction(m_rectangleToolAction);
    toolBar->addAction(m_circleToolAction);
    toolBar->addAction(m_windowToolAction);
    toolBar->addSeparator();
    toolBar->addAction(m_rulerToolAction);
    toolBar->addAction(m_gridToolAction);
}

void MainWindow::createCentralWidget() {
    QWidget* mainView = new MainView();
    setCentralWidget(mainView);
}

void MainWindow::radioToolSelected(RadioTool& tool) {
    statusBar()->showMessage(tool.getInstructions());
}

void MainWindow::linearUnitsChanged(LinearUnitsId) {    // NOLINT(readability-convert-member-functions-to-static)
    App::instance()->getToolMgr().refresh();
}

void MainWindow::angularUnitsChanged(AngularUnitsId) {  // NOLINT(readability-convert-member-functions-to-static)
    App::instance()->getToolMgr().refresh();
}
