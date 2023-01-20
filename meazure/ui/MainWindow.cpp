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
#include "GlobalShortcuts.h"
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
#include <meazure/prefs/ui/PrefsPageId.h>
#include <meazure/position-log/PosLogMgr.h>
#include <meazure/profile/ProfileMgr.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QActionGroup>
#include <QMenu>
#include <QClipboard>
#include <QLayout>
#include <QCursor>
#include <QMessageBox>
#include <QPushButton>
#include <vector>


MainWindow::MainWindow() {      // NOLINT(cppcoreguidelines-pro-type-member-init)
    createDialogs();
    createCentralWidget();
    createStatusBar();
    createActions();
    createMenus();
    createToolBar();
    createKeyboardControl();

    ToolMgr& toolMgr = App::instance()->getToolMgr();
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, &MainWindow::radioToolSelected);

    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();
    connect(&unitsMgr, &UnitsMgr::linearUnitsChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::angularUnitsChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::supplementalAngleChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::invertYChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::originChanged, &toolMgr, &ToolMgr::refresh);
    connect(&unitsMgr, &UnitsMgr::calibrationRequired, this, &MainWindow::warnCalibrationRequired);

    const PosLogMgr& posLogMgr = App::instance()->getPosLogMgr();
    connect(&posLogMgr, &PosLogMgr::dirtyChanged, this, &MainWindow::setWindowModified);

    m_cursorToolAction->trigger();
    m_pixelUnitsAction->trigger();
    m_degreeUnitsAction->trigger();

    toolMgr.setEnabled(OriginTool::k_toolName, true);
    toolMgr.setCrosshairsEnabled(true);

    setWindowTitle("[*]" + App::applicationName());
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    layout()->setSizeConstraint(QLayout::SetFixedSize);
    m_mainView->setMinimumWidth(sizeHint().width());

    setAlwaysVisible();
    setAllVisible(true);

    customUnitsChanged();
}

void MainWindow::createCentralWidget() {
    m_mainView = new MainView(m_prefsDialog);
    setCentralWidget(m_mainView);
}

void MainWindow::createStatusBar() {
    statusBar()->setStyleSheet("QStatusBar { border-top: 1px outset grey; }");
}

void MainWindow::createActions() {
    ToolMgr& toolMgr = App::instance()->getToolMgr();
    UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();
    const PosLogMgr& posLogMgr = App::instance()->getPosLogMgr();
    const ProfileMgr& profileMgr = App::instance()->getProfileMgr();

    // File actions

    m_loadPositionsAction = new QAction(tr("&Load Positions..."), this);
    m_loadPositionsAction->setShortcut(QKeySequence::Open);
    connect(m_loadPositionsAction, &QAction::triggered, &posLogMgr, &PosLogMgr::loadPositions);

    m_savePositionsAction = new QAction(tr("&Save Positions"), this);
    m_savePositionsAction->setShortcut(QKeySequence::Save);
    m_savePositionsAction->setEnabled(false);
    connect(m_savePositionsAction, &QAction::triggered, &posLogMgr, &PosLogMgr::savePositions);
    connect(&posLogMgr, &PosLogMgr::positionsChanged, this, [this](unsigned int numPositions) {
        m_savePositionsAction->setEnabled(numPositions > 0);
    });

    m_saveAsPositionsAction = new QAction(tr("Save Positions &As..."), this);
    m_saveAsPositionsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsPositionsAction->setEnabled(false);
    connect(m_saveAsPositionsAction, &QAction::triggered, &posLogMgr, &PosLogMgr::saveAsPositions);
    connect(&posLogMgr, &PosLogMgr::positionsChanged, this, [this](unsigned int numPositions) {
        m_saveAsPositionsAction->setEnabled(numPositions > 0);
    });

    m_loadProfileAction = new QAction(tr("Loa&d Profile..."), this);
    connect(m_loadProfileAction, &QAction::triggered, &profileMgr, &ProfileMgr::loadProfile);

    m_saveProfileAction = new QAction(tr("Save &Profile..."), this);
    connect(m_saveProfileAction, &QAction::triggered, &profileMgr, &ProfileMgr::saveProfile);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Exit Meazure"));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    // Edit actions

    m_copyRegionAction = new QAction(tr("Copy &Region"), this);
    m_copyRegionAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(m_copyRegionAction, &QAction::triggered, this, &MainWindow::copyRegion);

    m_findCrosshairsAction = new QAction(tr("&Find Crosshairs"), this);
    m_findCrosshairsAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(m_findCrosshairsAction, &QAction::triggered, this, [&toolMgr]() {
        toolMgr.flashTool();
    });
    connect(&toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_findCrosshairsAction->setEnabled(tool.hasCrosshairs());
    });

    m_recordPositionAction = new QAction(tr("R&ecord Position"), this);
    m_recordPositionAction->setShortcut(QKeySequence("Ctrl+P"));
    connect(m_recordPositionAction, &QAction::triggered, &posLogMgr, &PosLogMgr::addPosition);

    m_managePositionsAction = new QAction(tr("&Manage Positions..."), this);
    connect(m_managePositionsAction, &QAction::triggered, this, &MainWindow::managePositions);

    m_deletePositionsAction = new QAction(tr("De&lete Positions"), this);
    m_deletePositionsAction->setEnabled(false);
    connect(m_deletePositionsAction, &QAction::triggered, &posLogMgr, &PosLogMgr::deletePositions);
    connect(&posLogMgr, &PosLogMgr::positionsChanged, this, [this](unsigned int numPositions) {
        m_deletePositionsAction->setEnabled(numPositions > 0);
    });

    m_preferencesAction = new QAction(tr("Pre&ferences..."), this);
    connect(m_preferencesAction, &QAction::triggered, m_prefsDialog, &PrefsDialog::exec);

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

    m_customUnitsAction = new QAction("", linearUnitsGroup);
    m_customUnitsAction->setCheckable(true);
    connect(&unitsMgr, &UnitsMgr::customUnitsChanged, this, &MainWindow::customUnitsChanged);

    m_defineCustomUnitsAction = new QAction(tr("Define C&ustom..."), this);
    connect(m_defineCustomUnitsAction, &QAction::triggered, this, [this]() {
        m_prefsDialog->execPage(PrefsPageId::UnitsPage);
    });

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

    m_collapseAction = new QAction(this);
    m_collapseAction->setShortcut(QKeySequence("Ctrl+H"));
    connect(m_collapseAction, &QAction::triggered, this, [this]() {
        setAllVisible(!m_sectionVisibility.canCollapse());
    });
    connect(this, &MainWindow::visibilityChanged, this, [this]() {
        m_collapseAction->setText(m_sectionVisibility.canCollapse() ? tr("Collapse") : tr("Expand"));
    });

    m_toolBarVisibleAction = new QAction(tr("&Tool Bar"), this);
    m_toolBarVisibleAction->setCheckable(true);
    connect(m_toolBarVisibleAction, &QAction::triggered, this, &MainWindow::setToolBarVisible);
    connect(this, &MainWindow::toolBarVisibilityChanged, m_toolBarVisibleAction, &QAction::setChecked);

    m_toolDataSectionVisibleAction = new QAction(tr("Tool &Info"), this);
    m_toolDataSectionVisibleAction->setCheckable(true);
    connect(m_toolDataSectionVisibleAction, &QAction::triggered, this, &MainWindow::setToolDataSectionVisible);
    connect(this, &MainWindow::toolDataSectionVisibilityChanged, m_toolDataSectionVisibleAction, &QAction::setChecked);

    m_screenDataSectionVisibleAction = new QAction(tr("&Screen Info"), this);
    m_screenDataSectionVisibleAction->setCheckable(true);
    connect(m_screenDataSectionVisibleAction, &QAction::triggered, this, &MainWindow::setScreenDataSectionVisible);
    connect(this, &MainWindow::screenDataSectionVisibilityChanged, m_screenDataSectionVisibleAction, &QAction::setChecked);

    m_magnifierSectionVisibleAction = new QAction(tr("&Magnifier"), this);
    m_magnifierSectionVisibleAction->setCheckable(true);
    connect(m_magnifierSectionVisibleAction, &QAction::triggered, this, &MainWindow::setMagnifierSectionVisible);
    connect(this, &MainWindow::magnifierSectionVisibilityChanged, m_magnifierSectionVisibleAction, &QAction::setChecked);

    m_statusBarVisibleAction = new QAction(tr("Status &Bar"), this);
    m_statusBarVisibleAction->setCheckable(true);
    connect(m_statusBarVisibleAction, &QAction::triggered, this, &MainWindow::setStatusBarVisible);
    connect(this, &MainWindow::statusBarVisibilityChanged, m_statusBarVisibleAction, &QAction::setChecked);

    m_hideCrosshairsAction = new QAction(tr("&Hide Crosshairs"), this);
    m_hideCrosshairsAction->setShortcut(QKeySequence("Ctrl+B"));
    m_hideCrosshairsAction->setCheckable(true);
    connect(m_hideCrosshairsAction, &QAction::triggered, this, [&toolMgr](bool hide) {
        toolMgr.setCrosshairsEnabled(!hide);
    });
    connect(&toolMgr, &ToolMgr::crosshairsEnabled, this, [this](bool enable) {
        m_hideCrosshairsAction->setChecked(!enable);
    });

    m_hideDataWindowsAction = new QAction(tr("Hide &Popup Data Windows"), this);
    m_hideDataWindowsAction->setCheckable(true);
    connect(m_hideDataWindowsAction, &QAction::triggered, this, [&toolMgr](bool hide) {
        toolMgr.setDataWinEnabled(!hide);
    });
    connect(&toolMgr, &ToolMgr::dataWinEnabled, this, [this](bool enable) {
        m_hideDataWindowsAction->setChecked(!enable);
    });

    m_hideOriginToolAction = new QAction(tr("Hide &Origin Marker"), this);
    m_hideOriginToolAction->setCheckable(true);
    connect(m_hideOriginToolAction, &QAction::triggered, this, [&toolMgr](bool hide) {
        toolMgr.setEnabled(OriginTool::k_toolName, !hide);
    });
    connect(&toolMgr, &ToolMgr::toolEnabled, this, [this](Tool& tool, bool enabled) {
        if (tool.getName() == OriginTool::k_toolName) {
            m_hideOriginToolAction->setChecked(!enabled);
        }
    });

    m_invertYAction = new QAction(tr("Invert &Y"), this);
    m_invertYAction->setCheckable(true);
    connect(m_invertYAction, &QAction::triggered, &unitsMgr, &UnitsMgr::setInvertY);
    connect(&unitsMgr, &UnitsMgr::invertYChanged, m_invertYAction, &QAction::setChecked);

    m_supplementalAngleAction = new QAction(tr("Supplemental &Angle"));
    m_supplementalAngleAction->setCheckable(true);
    connect(m_supplementalAngleAction, &QAction::triggered, &unitsMgr, &UnitsMgr::setSupplementalAngle);
    connect(&unitsMgr, &UnitsMgr::supplementalAngleChanged, m_supplementalAngleAction, &QAction::setChecked);

    m_setOriginAction = new QAction(tr("Set &Origin"), this);
    m_setOriginAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(m_setOriginAction, &QAction::triggered, this, [&toolMgr, &unitsMgr] {
        unitsMgr.setOrigin(toolMgr.getActivePosition());
    });

    m_resetOriginAction = new QAction(tr("&Reset Origin"), this);
    connect(m_resetOriginAction, &QAction::triggered, this, [&unitsMgr] {
        unitsMgr.setOrigin(QPoint(0, 0));
    });

    m_alwaysVisibleAction = new QAction(tr("Always &Visible"), this);
    m_alwaysVisibleAction->setCheckable(true);
    connect(m_alwaysVisibleAction, &QAction::triggered, this, &MainWindow::setAlwaysVisible);
    connect(this, &MainWindow::alwaysVisibleChanged, m_alwaysVisibleAction, &QAction::setChecked);
}

void MainWindow::createMenus() {
    // File menu

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_loadPositionsAction);
    fileMenu->addAction(m_savePositionsAction);
    fileMenu->addAction(m_saveAsPositionsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_loadProfileAction);
    fileMenu->addAction(m_saveProfileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);

    // Edit menu

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(m_copyRegionAction);
    editMenu->addAction(m_mainView->getCopyColorAction());
    editMenu->addSeparator();
    editMenu->addAction(m_findCrosshairsAction);
    editMenu->addSeparator();
    editMenu->addAction(m_recordPositionAction);
    editMenu->addAction(m_managePositionsAction);
    editMenu->addAction(m_deletePositionsAction);
    editMenu->addSeparator();
    editMenu->addAction(m_preferencesAction);

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
    viewMenu->addAction(m_collapseAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_toolBarVisibleAction);
    viewMenu->addAction(m_toolDataSectionVisibleAction);
    viewMenu->addAction(m_screenDataSectionVisibleAction);
    viewMenu->addAction(m_magnifierSectionVisibleAction);
    viewMenu->addAction(m_statusBarVisibleAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_hideCrosshairsAction);
    viewMenu->addAction(m_hideDataWindowsAction);
    viewMenu->addAction(m_hideOriginToolAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_mainView->getMagnifierZoomInAction());
    viewMenu->addAction(m_mainView->getMagnifierZoomOutAction());
    viewMenu->addAction(m_mainView->getMagnifierGridAction());
    viewMenu->addAction(m_mainView->getMagnifierFreezeAction());

    QMenu* colorMenu = viewMenu->addMenu("&Color Format");
    for (QAction* colorFormat : m_mainView->getColorFormatActions()) {
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

void MainWindow::createToolBar() {
    m_toolBar = addToolBar("");
    m_toolBar->setIconSize(QSize(k_toolBarIconSize, k_toolBarIconSize));
    m_toolBar->addAction(m_cursorToolAction);
    m_toolBar->addAction(m_pointToolAction);
    m_toolBar->addAction(m_lineToolAction);
    m_toolBar->addAction(m_rectangleToolAction);
    m_toolBar->addAction(m_circleToolAction);
    m_toolBar->addAction(m_angleToolAction);
    m_toolBar->addAction(m_windowToolAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_rulerToolAction);
    m_toolBar->addAction(m_gridToolAction);
}

void MainWindow::createDialogs() {
    App* app = App::instance();

    auto* gridTool = dynamic_cast<GridTool*>(app->getToolMgr().getTool(GridTool::k_toolName));
    m_gridDialog = new GridDialog(gridTool, app->getScreenInfo(), app->getUnitsMgr(), this);

    m_prefsDialog = new PrefsDialog(this);

    m_positionDialog = new PosLogManageDlg(app->getPosLogMgr(), this);
}

void MainWindow::createKeyboardControl() {
    ToolMgr& toolMgr = App::instance()->getToolMgr();
    UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();

    auto* globalShortcuts = new GlobalShortcuts(this);

    connect(globalShortcuts, &GlobalShortcuts::ctrl1Up, this, [&toolMgr]() { toolMgr.stepY1Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl1Down, this, [&toolMgr]() { toolMgr.stepY1Position(1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl1Left, this, [&toolMgr]() { toolMgr.stepX1Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl1Right, this, [&toolMgr]() { toolMgr.stepX1Position(1); });

    connect(globalShortcuts, &GlobalShortcuts::ctrl2Up, this, [&toolMgr]() { toolMgr.stepY2Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl2Down, this, [&toolMgr]() { toolMgr.stepY2Position(1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl2Left, this, [&toolMgr]() { toolMgr.stepX2Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl2Right, this, [&toolMgr]() { toolMgr.stepX2Position(1); });

    connect(globalShortcuts, &GlobalShortcuts::ctrl3Up, this, [&toolMgr]() { toolMgr.stepYVPosition(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl3Down, this, [&toolMgr]() { toolMgr.stepYVPosition(1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl3Left, this, [&toolMgr]() { toolMgr.stepXVPosition(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl3Right, this, [&toolMgr]() { toolMgr.stepXVPosition(1); });

    connect(globalShortcuts, &GlobalShortcuts::ctrlShift1, this, [&toolMgr, &unitsMgr]() {
        const QPointF coord = unitsMgr.convertCoord(QCursor::pos());
        toolMgr.setX1Position(coord.x());
        toolMgr.setY1Position(coord.y());
    });
    connect(globalShortcuts, &GlobalShortcuts::ctrlShift2, this, [&toolMgr, &unitsMgr]() {
        const QPointF coord = unitsMgr.convertCoord(QCursor::pos());
        toolMgr.setX2Position(coord.x());
        toolMgr.setY2Position(coord.y());
    });
    connect(globalShortcuts, &GlobalShortcuts::ctrlShift3, this, [&toolMgr, &unitsMgr]() {
        const QPointF coord = unitsMgr.convertCoord(QCursor::pos());
        toolMgr.setXVPosition(coord.x());
        toolMgr.setYVPosition(coord.y());
    });

    App::instance()->installEventFilter(globalShortcuts);
}

void MainWindow::saveToProfile(Profile& profile) const {
    if (!profile.userInitiated()) {
        profile.writeBool("AlwaysVisible", isAlwaysVisible());
        profile.writeBool("ExpandToolbar", m_sectionVisibility.m_toolBarVisible);
        profile.writeBool("ExpandToolInfo", m_sectionVisibility.m_toolDataSectionVisible);
        profile.writeBool("ExpandScreenInfo", m_sectionVisibility.m_screenDataSectionVisible);
        profile.writeBool("ExpandMagnifier", m_sectionVisibility.m_magnifierSectionVisible);
        profile.writeBool("ExpandStatusbar", m_sectionVisibility.m_statusBarVisible);
    }
}

void MainWindow::loadFromProfile(Profile& profile) {
    if (!profile.userInitiated()) {
        setAlwaysVisible(profile.readBool("AlwaysVisible", isAlwaysVisible()));
        setToolBarVisible(profile.readBool("ExpandToolbar", m_sectionVisibility.m_toolBarVisible));
        setToolDataSectionVisible(profile.readBool("ExpandToolInfo", m_sectionVisibility.m_toolDataSectionVisible));
        setScreenDataSectionVisible(profile.readBool("ExpandScreenInfo", m_sectionVisibility.m_screenDataSectionVisible));
        setMagnifierSectionVisible(profile.readBool("ExpandMagnifier", m_sectionVisibility.m_magnifierSectionVisible));
        setStatusBarVisible(profile.readBool("ExpandStatusbar", m_sectionVisibility.m_statusBarVisible));
    }
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

void MainWindow::managePositions() {
    m_positionDialog->show();
}

void MainWindow::setAlwaysVisible(bool alwaysVisible) {
    setWindowFlag(Qt::WindowStaysOnTopHint, alwaysVisible);
    show();

    emit alwaysVisibleChanged(alwaysVisible);
}

bool MainWindow::isAlwaysVisible() const {
    return (windowFlags() & Qt::WindowStaysOnTopHint) != 0;
}

void MainWindow::setToolBarVisible(bool visible) {
    m_sectionVisibility.m_toolBarVisible = visible;
    m_toolBar->setVisible(visible);

    emit toolBarVisibilityChanged(visible);
    emit visibilityChanged();
}

void MainWindow::setToolDataSectionVisible(bool visible) {
    m_sectionVisibility.m_toolDataSectionVisible = visible;
    m_mainView->getToolDataSection()->setVisible(visible);

    emit toolDataSectionVisibilityChanged(visible);
    emit visibilityChanged();
}

void MainWindow::setScreenDataSectionVisible(bool visible) {
    m_sectionVisibility.m_screenDataSectionVisible = visible;
    m_mainView->getScreenDataSection()->setVisible(visible);

    emit screenDataSectionVisibilityChanged(visible);
    emit visibilityChanged();
}

void MainWindow::setMagnifierSectionVisible(bool visible) {
    m_sectionVisibility.m_magnifierSectionVisible = visible;
    m_mainView->getMagnifierSection()->setVisible(visible);

    emit magnifierSectionVisibilityChanged(visible);
    emit visibilityChanged();
}

void MainWindow::setStatusBarVisible(bool visible) {
    m_sectionVisibility.m_statusBarVisible = visible;
    statusBar()->setVisible(visible);

    emit statusBarVisibilityChanged(visible);
    emit visibilityChanged();
}

void MainWindow::setAllVisible(bool visible) {
    if (visible) {
        setToolBarVisible();
        setToolDataSectionVisible();
        setScreenDataSectionVisible();
        setMagnifierSectionVisible();
        setStatusBarVisible();
    } else {
        setToolBarVisible(false);
        setToolDataSectionVisible(false);
        setScreenDataSectionVisible(false);
        setMagnifierSectionVisible(false);
        setStatusBarVisible(false);
    }
}

void MainWindow::customUnitsChanged() {
    const CustomUnits* customUnits = App::instance()->getUnitsMgr().getCustomUnits();
    const bool defined = customUnits->haveCustomUnits();
    m_customUnitsAction->setEnabled(defined);
    m_customUnitsAction->setText(defined ? customUnits->getName() : tr("[custom]"));

    if (!defined && m_customUnitsAction->isChecked()) {
        App::instance()->getUnitsMgr().setLinearUnits(PixelsId);
    }
}

void MainWindow::warnCalibrationRequired() {
    QMessageBox calDlg;
    QPushButton *calibrateButton = calDlg.addButton(tr("Calibrate"), QMessageBox::YesRole);
    calDlg.addButton(tr("Continue"), QMessageBox::NoRole);
    calDlg.setDefaultButton(calibrateButton);
    calDlg.setWindowTitle(tr("Warning - Screen Resolution Not Calibrated"));
    calDlg.setText(QObject::tr("Screen resolution calibration is recommended."));
    calDlg.setInformativeText(tr("To accurately display the requested units, Meazure requires the correct screen "
                                 "resolution (pixels/inch).\n\n"
                                 "Using the operating system default screen resolution is not accurate. "
                                 "It is highly recommended that you manually set the resolution using "
                                 "the Calibration preference panel.\n\n"
                                 "If the pixel dimensions of the screen have changed since the last time the screen "
                                 "resolution was set (e.g. new monitor), please recalibrate the screen resolution "
                                 "using the Calibration preference panel."));
    calDlg.setIcon(QMessageBox::Warning);
    calDlg.exec();
    if (calDlg.clickedButton() == calibrateButton) {
        m_prefsDialog->execPage(PrefsPageId::CalibrationPage);
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    bool accept = true;

    if (isWindowModified()) {
        const QMessageBox::StandardButton response =
                QMessageBox::question(this, tr("Save Positions"),
                                      tr("Positions have been recorded but have not yet been saved.\n\n"
                                         "Save these positions?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                      QMessageBox::Yes);
        if (response == QMessageBox::Yes) {
            accept = App::instance()->getPosLogMgr().savePositions();
        } else {
            accept = (response == QMessageBox::No);
        }
    }

    event->setAccepted(accept);
}
