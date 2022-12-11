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
#include <meazure/tools/CircleTool.h>
#include <meazure/tools/CursorTool.h>
#include <meazure/tools/GridTool.h>
#include <meazure/tools/RulerTool.h>
#include <meazure/tools/PointTool.h>
#include <meazure/tools/LineTool.h>
#include <meazure/tools/OriginTool.h>
#include <meazure/tools/RectangleTool.h>
#include <meazure/tools/AngleTool.h>
#include <meazure/tools/WindowTool.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/units/Units.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QActionGroup>
#include <QMenu>
#include <QClipboard>
#include <vector>


MainWindow::MainWindow() {      // NOLINT(cppcoreguidelines-pro-type-member-init)
    createCentralWidget();
    createActions();
    createMenus();
    createToolbar();
    createDialogs();

    ToolMgr& toolMgr = App::instance()->getToolMgr();
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, &MainWindow::radioToolSelected);

    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::angularUnitsChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::supplementalAngleChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::invertYChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::originChanged, &toolMgr, &ToolMgr::refresh);

    m_cursorToolAction->trigger();
    m_pixelUnitsAction->trigger();
    m_degreeUnitsAction->trigger();

    toolMgr.setEnabled(OriginTool::k_toolName, true);

    setWindowFlag(Qt::WindowMaximizeButtonHint, false);
    setMaximumWidth(sizeHint().width());
    setMaximumHeight(sizeHint().height());

    setAlwaysVisible(k_defAlwaysVisible);
}

void MainWindow::createCentralWidget() {
    QWidget* mainView = new MainView();
    setCentralWidget(mainView);
}

void MainWindow::createActions() {
    ToolMgr& toolMgr = App::instance()->getToolMgr();
    UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();

    // Edit actions

    m_copyRegionAction = new QAction(tr("Copy Region"), this);
    m_copyRegionAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(m_copyRegionAction, &QAction::triggered, this, &MainWindow::copyRegion);

    // Tool actions

    auto* radioToolGroup = new QActionGroup(this);
    radioToolGroup->setExclusive(true);

    m_cursorToolAction = new QAction(QIcon(":/images/CursorTool.svg"), tr("&Cursor"), radioToolGroup);
    m_cursorToolAction->setCheckable(true);
    m_cursorToolAction->setToolTip("Tracks cursor position");
    connect(m_cursorToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(CursorTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_cursorToolAction->setChecked(tool.getName() == CursorTool::k_toolName);
    });

    m_pointToolAction = new QAction(QIcon(":/images/PointTool.svg"), tr("&Point"), radioToolGroup);
    m_pointToolAction->setCheckable(true);
    m_pointToolAction->setToolTip("Measures a point");
    connect(m_pointToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(PointTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_pointToolAction->setChecked(tool.getName() == PointTool::k_toolName);
    });

    m_lineToolAction = new QAction(QIcon(":/images/LineTool.svg"), tr("&Line"), radioToolGroup);
    m_lineToolAction->setCheckable(true);
    m_lineToolAction->setToolTip("Measures using line");
    connect(m_lineToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(LineTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_lineToolAction->setChecked(tool.getName() == LineTool::k_toolName);
    });

    m_rectangleToolAction = new QAction(QIcon(":/images/RectangleTool.svg"), tr("&Rectangle"), radioToolGroup);
    m_rectangleToolAction->setCheckable(true);
    m_rectangleToolAction->setToolTip("Measures using rectangle");
    connect(m_rectangleToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(RectangleTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_rectangleToolAction->setChecked(tool.getName() == RectangleTool::k_toolName);
    });

    m_circleToolAction = new QAction(QIcon(":/images/CircleTool.svg"), tr("C&ircle"), radioToolGroup);
    m_circleToolAction->setCheckable(true);
    m_circleToolAction->setToolTip("Measures using circle");
    connect(m_circleToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(CircleTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_circleToolAction->setChecked(tool.getName() == CircleTool::k_toolName);
    });

    m_angleToolAction = new QAction(QIcon(":/images/AngleTool.svg"), tr("&Angle"), radioToolGroup);
    m_angleToolAction->setCheckable(true);
    m_angleToolAction->setToolTip("Measures using protractor");
    connect(m_angleToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(AngleTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_angleToolAction->setChecked(tool.getName() == AngleTool::k_toolName);
    });

    m_windowToolAction = new QAction(QIcon(":/images/WindowTool.svg"), tr("&Window"), radioToolGroup);
    m_windowToolAction->setCheckable(true);
    m_windowToolAction->setToolTip("Measures a window");
    connect(m_windowToolAction, &QAction::triggered, this,
            [&toolMgr] { toolMgr.selectRadioTool(WindowTool::k_toolName); });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_windowToolAction->setChecked(tool.getName() == WindowTool::k_toolName);
    });

    m_rulerToolAction = new QAction(QIcon(":/images/RulerTool.svg"), tr("R&uler"), this);
    m_rulerToolAction->setCheckable(true);
    m_rulerToolAction->setToolTip("Adds screen rulers");
    connect(m_rulerToolAction, &QAction::triggered, this,
            [&toolMgr](bool checked) { toolMgr.setEnabled(RulerTool::k_toolName, checked); });
    connect(&toolMgr, &ToolMgr::toolEnabled, this, [this](Tool& tool, bool enabled) {
        if (tool.getName() == RulerTool::k_toolName) {
            m_rulerToolAction->setChecked(enabled);
        }
    });

    m_gridToolAction = new QAction(QIcon(":/images/GridTool.svg"), tr("&Screen Grid"), this);
    m_gridToolAction->setCheckable(true);
    m_gridToolAction->setToolTip("Adds screen grid");
    connect(m_gridToolAction, &QAction::triggered, this,
            [&toolMgr](bool checked) { toolMgr.setEnabled(GridTool::k_toolName, checked); });
    connect(&toolMgr, &ToolMgr::toolEnabled, this, [this](Tool& tool, bool enabled) {
        if (tool.getName() == GridTool::k_toolName) {
            m_gridToolAction->setChecked(enabled);
        }
    });

    m_gridAdjustAction = new QAction(tr("Screen &Grid Spacing..."), this);
    connect(m_gridAdjustAction, &QAction::triggered, this, &MainWindow::adjustGrid);

    // Units actions

    auto* linearUnitsGroup = new QActionGroup(this);
    linearUnitsGroup->setExclusive(true);

    m_pixelUnitsAction = new QAction(tr("&Pixels"), linearUnitsGroup);
    m_pixelUnitsAction->setCheckable(true);
    connect(m_pixelUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(PixelsId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_pixelUnitsAction->setChecked(id == PixelsId);
    });

    m_twipUnitsAction = new QAction(tr("&Twips"), linearUnitsGroup);
    m_twipUnitsAction->setCheckable(true);
    connect(m_twipUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(TwipsId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_twipUnitsAction->setChecked(id == TwipsId);
    });

    m_pointUnitsAction = new QAction(tr("P&oints"), linearUnitsGroup);
    m_pointUnitsAction->setCheckable(true);
    connect(m_pointUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(PointsId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_pointUnitsAction->setChecked(id == PointsId);
    });

    m_picaUnitsAction = new QAction(tr("Pic&as"), linearUnitsGroup);
    m_picaUnitsAction->setCheckable(true);
    connect(m_picaUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(PicasId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_picaUnitsAction->setChecked(id == PicasId);
    });

    m_inchUnitsAction = new QAction(tr("&Inches"), linearUnitsGroup);
    m_inchUnitsAction->setCheckable(true);
    connect(m_inchUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(InchesId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_inchUnitsAction->setChecked(id == InchesId);
    });

    m_centimeterUnitsAction = new QAction(tr("&Centimeters"), linearUnitsGroup);
    m_centimeterUnitsAction->setCheckable(true);
    connect(m_centimeterUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(CentimetersId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_centimeterUnitsAction->setChecked(id == CentimetersId);
    });

    m_millimeterUnitsAction = new QAction(tr("&Millimeters"), linearUnitsGroup);
    m_millimeterUnitsAction->setCheckable(true);
    connect(m_millimeterUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setLinearUnits(MillimetersId); });
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_millimeterUnitsAction->setChecked(id == MillimetersId);
    });

    m_customUnitsAction = new QAction(tr("[custom]"), linearUnitsGroup);
    m_customUnitsAction->setCheckable(true);

    m_defineCustomUnitsAction = new QAction(tr("Define custom..."), linearUnitsGroup);
    m_defineCustomUnitsAction->setCheckable(true);

    auto* angularUnitsGroup = new QActionGroup(this);
    angularUnitsGroup->setExclusive(true);

    m_degreeUnitsAction = new QAction(tr("&Degrees"), angularUnitsGroup);
    m_degreeUnitsAction->setCheckable(true);
    connect(m_degreeUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setAngularUnits(DegreesId); });
    connect(&unitsMgr, &UnitsMgr::angularUnitsChanged, this, [this](AngularUnitsId id) {
        m_degreeUnitsAction->setChecked(id == DegreesId);
    });

    m_radianUnitsAction = new QAction(tr("&Radians"), angularUnitsGroup);
    m_radianUnitsAction->setCheckable(true);
    connect(m_radianUnitsAction, &QAction::triggered, this, [&unitsMgr] { unitsMgr.setAngularUnits(RadiansId); });
    connect(&unitsMgr, &UnitsMgr::angularUnitsChanged, this, [this](AngularUnitsId id) {
        m_radianUnitsAction->setChecked(id == RadiansId);
    });

    // View actions

    m_invertYAction = new QAction(tr("Invert &Y"), this);
    m_invertYAction->setCheckable(true);
    connect(m_invertYAction, &QAction::triggered, &unitsMgr, &UnitsMgr::setInvertY);
    connect(&unitsMgr, &UnitsMgr::invertYChanged, m_invertYAction, &QAction::setChecked);

    m_supplementalAngleAction = new QAction(tr("Supplemental &Angle"));
    m_supplementalAngleAction->setCheckable(true);
    connect(m_supplementalAngleAction, &QAction::triggered, &unitsMgr, &UnitsMgr::setSupplementalAngle);
    connect(&unitsMgr, &UnitsMgr::supplementalAngleChanged, m_supplementalAngleAction, &QAction::setChecked);

    m_setOriginAction = new QAction(tr("Set Origin"), this);
    m_setOriginAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(m_setOriginAction, &QAction::triggered, this, [&toolMgr, &unitsMgr] {
        unitsMgr.setOrigin(toolMgr.getActivePosition());
    });

    m_resetOriginAction = new QAction(tr("Reset Origin"), this);
    connect(m_resetOriginAction, &QAction::triggered, this, [&unitsMgr] {
        unitsMgr.setOrigin(QPoint(0, 0));
    });

    m_alwaysVisibleAction = new QAction(tr("Always &Visible"), this);
    m_alwaysVisibleAction->setCheckable(true);
    connect(m_alwaysVisibleAction, &QAction::triggered, this, &MainWindow::setAlwaysVisible);
    connect(this, &MainWindow::alwaysVisibleChanged, m_alwaysVisibleAction, &QAction::setChecked);
}

void MainWindow::createMenus() {
    const MainView* mainView = dynamic_cast<MainView*>(centralWidget());

    // File menu

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addSeparator();

    auto* exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit Meazure"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAct);

    // Edit menu

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_copyRegionAction);
    editMenu->addAction(mainView->getCopyColorAction());

    // Tools menu

    QMenu* toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(m_cursorToolAction);
    toolsMenu->addAction(m_pointToolAction);
    toolsMenu->addAction(m_lineToolAction);
    toolsMenu->addAction(m_rectangleToolAction);
    toolsMenu->addAction(m_circleToolAction);
    toolsMenu->addAction(m_angleToolAction);
    toolsMenu->addAction(m_windowToolAction);
    toolsMenu->addSeparator();
    toolsMenu->addAction(m_rulerToolAction);
    toolsMenu->addAction(m_gridToolAction);
    toolsMenu->addAction(m_gridAdjustAction);

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
    unitsMenu->addSeparator();
    unitsMenu->addAction(m_degreeUnitsAction);
    unitsMenu->addAction(m_radianUnitsAction);

    // View menu

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(mainView->getMagnifierZoomInAction());
    viewMenu->addAction(mainView->getMagnifierZoomOutAction());
    viewMenu->addAction(mainView->getMagnifierGridAction());
    viewMenu->addAction(mainView->getMagnifierFreezeAction());

    QMenu* colorMenu = viewMenu->addMenu("&Color Format");
    for (QAction* colorFormat : mainView->getColorFormatActions()) {
        colorMenu->addAction(colorFormat);
    }

    viewMenu->addSeparator();

    viewMenu->addAction(m_invertYAction);
    viewMenu->addAction(m_supplementalAngleAction);
    viewMenu->addAction(m_setOriginAction);
    viewMenu->addAction(m_resetOriginAction);

    viewMenu->addSeparator();

    viewMenu->addAction(m_alwaysVisibleAction);
}

void MainWindow::createToolbar() {
    QToolBar* toolBar = addToolBar("");
    toolBar->setIconSize(QSize(k_toolbarIconSize, k_toolbarIconSize));
    toolBar->addAction(m_cursorToolAction);
    toolBar->addAction(m_pointToolAction);
    toolBar->addAction(m_lineToolAction);
    toolBar->addAction(m_rectangleToolAction);
    toolBar->addAction(m_circleToolAction);
    toolBar->addAction(m_angleToolAction);
    toolBar->addAction(m_windowToolAction);
    toolBar->addSeparator();
    toolBar->addAction(m_rulerToolAction);
    toolBar->addAction(m_gridToolAction);
}

void MainWindow::createDialogs() {
    App* app = App::instance();
    auto* gridTool = dynamic_cast<GridTool*>(app->getToolMgr().getTool(GridTool::k_toolName));
    m_gridDialog = new GridDialog(gridTool, app->getScreenInfo(), app->getUnitsMgr(), this);
}

void MainWindow::radioToolSelected(RadioTool& tool) {
    m_copyRegionAction->setEnabled(tool.canGrabRegion());

    statusBar()->showMessage(tool.getInstructions());
}

void MainWindow::copyRegion() {
    RadioTool* radioTool = App::instance()->getToolMgr().getCurentRadioTool();
    if (radioTool->canGrabRegion()) {
        const QImage image = radioTool->grabRegion();
        if (!image.isNull()) {
            App::clipboard()->setImage(image);
        }
        radioTool->flash();
    }
}

void MainWindow::adjustGrid() {
    if (!m_gridToolAction->isChecked()) {
        m_gridToolAction->trigger();
    }
    m_gridDialog->exec();
}

void MainWindow::setAlwaysVisible(bool alwaysVisible) {
    setWindowFlag(Qt::WindowStaysOnTopHint, alwaysVisible);
    show();

    emit alwaysVisibleChanged(alwaysVisible);
}
