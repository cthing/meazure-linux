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
#include <meazure/units/Units.h>
#include <meazure/utils/HelpUtils.h>
#include <meazure/prefs/ui/PrefsPageId.h>
#include <meazure/position-log/PosLogMgr.h>
#include <meazure/config/ConfigMgr.h>
#include <QGuiApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QActionGroup>
#include <QMenu>
#include <QClipboard>
#include <QLayout>
#include <QCursor>
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QWhatsThis>
#include <vector>


MainWindow::MainWindow(ScreenInfo* screenInfo, UnitsMgr* unitsMgr, ToolMgr* toolMgr, PosLogMgr* posLogMgr, // NOLINT(cppcoreguidelines-pro-type-member-init)
                       ConfigMgr* configMgr) :
        m_screenInfo(screenInfo),
        m_unitsMgr(unitsMgr),
        m_toolMgr(toolMgr),
        m_posLogMgr(posLogMgr),
        m_configMgr(configMgr) {
    createDialogs();
    createCentralWidget();
    createStatusBar();
    createActions();
    createMenus();
    createToolBar();
    createKeyboardControl();

    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, &MainWindow::radioToolSelected);

    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, m_toolMgr, &ToolMgr::refresh);
    connect(m_unitsMgr, &UnitsMgr::angularUnitsChanged, m_toolMgr, &ToolMgr::refresh);
    connect(m_unitsMgr, &UnitsMgr::supplementalAngleChanged, m_toolMgr, &ToolMgr::refresh);
    connect(m_unitsMgr, &UnitsMgr::invertYChanged, m_toolMgr, &ToolMgr::refresh);
    connect(m_unitsMgr, &UnitsMgr::originChanged, m_toolMgr, &ToolMgr::refresh);
    connect(m_unitsMgr, &UnitsMgr::calibrationRequired, this, &MainWindow::warnCalibrationRequired);

    connect(m_posLogMgr, &PosLogMgr::dirtyChanged, this, &MainWindow::setWindowModified);

    m_cursorToolAction->trigger();
    m_pixelUnitsAction->trigger();
    m_degreeUnitsAction->trigger();

    m_toolMgr->setEnabled(OriginTool::k_toolName, true);
    m_toolMgr->setCrosshairsEnabled(true);

    setWindowTitle("[*]" + QCoreApplication::applicationName());
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    layout()->setSizeConstraint(QLayout::SetFixedSize);
    m_mainView->setMinimumWidth(sizeHint().width());

    setAlwaysVisible();
    setAllVisible(true);

    customUnitsChanged();
}

void MainWindow::createCentralWidget() {
    m_mainView = new MainView(m_screenInfo, m_unitsMgr, m_toolMgr, m_prefsDialog);
    setCentralWidget(m_mainView);
}

void MainWindow::createStatusBar() {
    statusBar()->setStyleSheet("QStatusBar { border-top: 1px outset grey; }");
}

void MainWindow::createActions() {
    // File actions

    m_loadPositionsAction = new QAction(tr("&Load Positions..."), this);
    m_loadPositionsAction->setShortcut(QKeySequence::Open);
    m_loadPositionsAction->setStatusTip(tr("Load a position log file"));
    m_loadPositionsAction->setWhatsThis(tr("Loads a previously saved log of tool positions so they can be replayed."));
    connect(m_loadPositionsAction, &QAction::triggered, m_posLogMgr, &PosLogMgr::loadPositions);

    m_savePositionsAction = new QAction(tr("&Save Positions"), this);
    m_savePositionsAction->setShortcut(QKeySequence::Save);
    m_savePositionsAction->setStatusTip(tr("Save recorded positions"));
    m_savePositionsAction->setWhatsThis(tr("Saves recorded positions to a position log file."));
    m_savePositionsAction->setEnabled(false);
    connect(m_savePositionsAction, &QAction::triggered, m_posLogMgr, &PosLogMgr::savePositions);
    connect(m_posLogMgr, &PosLogMgr::positionsChanged, this, [this](unsigned int numPositions) {
        m_savePositionsAction->setEnabled(numPositions > 0);
    });

    m_saveAsPositionsAction = new QAction(tr("Save Positions &As..."), this);
    m_saveAsPositionsAction->setShortcut(QKeySequence::SaveAs);
    m_saveAsPositionsAction->setStatusTip(tr("Save recorded positions"));
    m_saveAsPositionsAction->setWhatsThis(tr("Saves recorded positions to a position log file."));
    m_saveAsPositionsAction->setEnabled(false);
    connect(m_saveAsPositionsAction, &QAction::triggered, m_posLogMgr, &PosLogMgr::saveAsPositions);
    connect(m_posLogMgr, &PosLogMgr::positionsChanged, this, [this](unsigned int numPositions) {
        m_saveAsPositionsAction->setEnabled(numPositions > 0);
    });

    m_importConfigAction = new QAction(tr("&Import Configuration..."), this);
    m_importConfigAction->setStatusTip(tr("Restore application settings"));
    m_importConfigAction->setWhatsThis(tr("Restores application settings previously exported configuration file."));
    connect(m_importConfigAction, &QAction::triggered, m_configMgr, &ConfigMgr::importConfig);

    m_exportConfigAction = new QAction(tr("&Export Configuration..."), this);
    m_exportConfigAction->setStatusTip(tr("Saves application settings"));
    m_exportConfigAction->setWhatsThis(tr("Saves application settings to a configuration file."));
    connect(m_exportConfigAction, &QAction::triggered, m_configMgr, &ConfigMgr::exportConfig);

    m_exitAction = new QAction(tr("E&xit"), this);
    m_exitAction->setShortcuts(QKeySequence::Quit);
    m_exitAction->setStatusTip(tr("Exit Meazure"));
    m_exitAction->setWhatsThis(tr("Exists the application."));
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);

    // Edit actions

    m_copyRegionAction = new QAction(tr("Copy &Region"), this);
    m_copyRegionAction->setShortcut(QKeySequence("Ctrl+R"));
    m_copyRegionAction->setStatusTip(tr("Copy tool bounding box to clipboard"));
    m_copyRegionAction->setWhatsThis(tr("Copy the current tool's bounding box to the clipboard."));
    connect(m_copyRegionAction, &QAction::triggered, this, &MainWindow::copyRegion);

    m_findCrosshairsAction = new QAction(tr("&Find Crosshairs"), this);
    m_findCrosshairsAction->setShortcut(QKeySequence("Ctrl+F"));
    m_findCrosshairsAction->setStatusTip(tr("Flash tool crosshairs"));
    m_findCrosshairsAction->setWhatsThis(tr("Flash the current tool's crosshairs."));
    connect(m_findCrosshairsAction, &QAction::triggered, this, [this]() {
        m_toolMgr->flashTool();
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_findCrosshairsAction->setEnabled(tool.hasCrosshairs());
    });

    m_recordPositionAction = new QAction(tr("R&ecord Position"), this);
    m_recordPositionAction->setShortcut(QKeySequence("Ctrl+P"));
    m_recordPositionAction->setStatusTip(tr("Record tool position"));
    m_recordPositionAction->setWhatsThis(tr("Record current tool's cursor or crosshair positions."));
    connect(m_recordPositionAction, &QAction::triggered, m_posLogMgr, &PosLogMgr::addPosition);

    m_managePositionsAction = new QAction(tr("&Manage Positions..."), this);
    m_managePositionsAction->setStatusTip(tr("Replay and update tool positions"));
    m_managePositionsAction->setWhatsThis(tr("Provides a dialog to replay and update tool positions."));
    connect(m_managePositionsAction, &QAction::triggered, this, &MainWindow::managePositions);
    connect(m_posLogMgr, &PosLogMgr::positionsLoaded, this, &MainWindow::managePositions);

    m_deletePositionsAction = new QAction(tr("De&lete Positions"), this);
    m_deletePositionsAction->setStatusTip(tr("Delete all recorded positions"));
    m_deletePositionsAction->setWhatsThis(tr("Delete all previously recorded tool positions."));
    m_deletePositionsAction->setEnabled(false);
    connect(m_deletePositionsAction, &QAction::triggered, m_posLogMgr, &PosLogMgr::deletePositions);
    connect(m_posLogMgr, &PosLogMgr::positionsChanged, this, [this](unsigned int numPositions) {
        m_deletePositionsAction->setEnabled(numPositions > 0);
    });

    m_preferencesAction = new QAction(tr("Pre&ferences..."), this);
    m_preferencesAction->setStatusTip(tr("Set application preferences"));
    m_preferencesAction->setWhatsThis(tr("Provides a dialog to review and set the application preferences."));
    connect(m_preferencesAction, &QAction::triggered, m_prefsDialog, &PrefsDialog::exec);

    // Tool actions

    auto* radioToolGroup = new QActionGroup(this);
    radioToolGroup->setExclusive(true);

    m_cursorToolAction = new QAction(QIcon(Colors::isDarkMode()
            ? ":/images/CursorToolDark.svg" : ":/images/CursorTool.svg"), tr("&Cursor"), radioToolGroup);
    m_cursorToolAction->setCheckable(true);
    m_cursorToolAction->setToolTip(tr("Tracks cursor position"));
    m_cursorToolAction->setStatusTip(tr("Tracks cursor position"));
    m_cursorToolAction->setWhatsThis(tr("Tracks the mouse cursor position."));
    connect(m_cursorToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(CursorTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_cursorToolAction->setChecked(tool.getName() == CursorTool::k_toolName);
    });

    m_pointToolAction = new QAction(QIcon(":/images/PointTool.svg"), tr("&Point"), radioToolGroup);
    m_pointToolAction->setCheckable(true);
    m_pointToolAction->setToolTip(tr("Measures a point"));
    m_pointToolAction->setStatusTip(tr("Measures a point"));
    m_pointToolAction->setWhatsThis(tr("Measures a point. <b>Shift+drag</b> for vertical or horizontal motion. "
                                       "<b>Ctrl+1+arrow</b> moves crosshair one pixel."));
    connect(m_pointToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(PointTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_pointToolAction->setChecked(tool.getName() == PointTool::k_toolName);
    });

    m_lineToolAction = new QAction(QIcon(Colors::isDarkMode()
            ? ":/images/LineToolDark.svg" : ":/images/LineTool.svg"), tr("&Line"), radioToolGroup);
    m_lineToolAction->setCheckable(true);
    m_lineToolAction->setToolTip(tr("Measures using line"));
    m_lineToolAction->setStatusTip(tr("Measures using a line"));
    m_lineToolAction->setWhatsThis(tr("Measures using a line. <b>Shift+drag</b> for vertical or horizontal crosshair "
                                      "motion. <b>Ctrl+drag</b> moves line. <b>Ctrl+1|2+arrow</b> moves a crosshair "
                                      "one pixel."));
    connect(m_lineToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(LineTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_lineToolAction->setChecked(tool.getName() == LineTool::k_toolName);
    });

    m_rectangleToolAction = new QAction(QIcon(Colors::isDarkMode()
            ? ":/images/RectangleToolDark.svg" : ":/images/RectangleTool.svg"), tr("&Rectangle"), radioToolGroup);
    m_rectangleToolAction->setCheckable(true);
    m_rectangleToolAction->setToolTip(tr("Measures using rectangle"));
    m_rectangleToolAction->setStatusTip(tr("Measures using a rectangle"));
    m_rectangleToolAction->setWhatsThis(tr("Measures using a rectangle. <b>Shift+drag</b> for vertical or horizontal "
                                           "crosshair motion. <b>Ctrl+drag</b> moves rectangle. <b>Ctrl+1|2+arrow</b> "
                                           "moves a crosshair one pixel."));
    connect(m_rectangleToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(RectangleTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_rectangleToolAction->setChecked(tool.getName() == RectangleTool::k_toolName);
    });

    m_circleToolAction = new QAction(QIcon(Colors::isDarkMode()
            ? ":/images/CircleToolDark.svg" : ":/images/CircleTool.svg"), tr("C&ircle"), radioToolGroup);
    m_circleToolAction->setCheckable(true);
    m_circleToolAction->setToolTip(tr("Measures using circle"));
    m_circleToolAction->setStatusTip(tr("Measures using a circle"));
    m_circleToolAction->setWhatsThis(tr("Measures using a circle. <b>Shift+drag</b> for vertical or horizontal "
                                        "crosshair motion. <b>Ctrl+drag</b> moves circle. <b>Ctrl+1|3+arrow</b> "
                                        "moves a crosshair one pixel."));
    connect(m_circleToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(CircleTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_circleToolAction->setChecked(tool.getName() == CircleTool::k_toolName);
    });

    m_angleToolAction = new QAction(QIcon(Colors::isDarkMode()
            ? ":/images/AngleToolDark.svg" : ":/images/AngleTool.svg"), tr("&Angle"), radioToolGroup);
    m_angleToolAction->setCheckable(true);
    m_angleToolAction->setToolTip(tr("Measures using protractor"));
    m_angleToolAction->setStatusTip(tr("Measures using protractor"));
    m_angleToolAction->setWhatsThis(tr("Measures using protractor. <b>Shift+drag</b> for vertical or horizontal "
                                       "crosshair motion. <b>Ctrl+drag</b> moves protractor. <b>Ctrl+1|2|3+arrow</b> "
                                       "moves a crosshair one pixel. Positive angles are clockwise from point 1 to 2. "
                                       "Negative angles are counter-clockwise from point 1 to 2."));
    connect(m_angleToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(AngleTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_angleToolAction->setChecked(tool.getName() == AngleTool::k_toolName);
    });

    m_windowToolAction = new QAction(QIcon(":/images/WindowTool.svg"), tr("&Window"), radioToolGroup);
    m_windowToolAction->setCheckable(true);
    m_windowToolAction->setToolTip(tr("Measures a window"));
    m_windowToolAction->setStatusTip(tr("Measures a window"));
    m_windowToolAction->setWhatsThis(tr("Measures the window under the cursor."));
    connect(m_windowToolAction, &QAction::triggered, this, [this] {
        m_toolMgr->selectRadioTool(WindowTool::k_toolName);
    });
    connect(m_toolMgr, &ToolMgr::radioToolSelected, this, [this](RadioTool& tool) {
        m_windowToolAction->setChecked(tool.getName() == WindowTool::k_toolName);
    });

    m_rulerToolAction = new QAction(QIcon(":/images/RulerTool.svg"), tr("R&uler"), this);
    m_rulerToolAction->setCheckable(true);
    m_rulerToolAction->setToolTip(tr("Overlays rulers"));
    m_rulerToolAction->setStatusTip(tr("Overlays rulers"));
    m_rulerToolAction->setWhatsThis(tr("Overlays a ruler square. The ruler size, position and orientation can "
                                       "be adjusted."));
    connect(m_rulerToolAction, &QAction::triggered, this, [this](bool checked) {
        m_toolMgr->setEnabled(RulerTool::k_toolName, checked);
    });
    connect(m_toolMgr, &ToolMgr::toolEnabled, this, [this](Tool& tool, bool enabled) {
        if (tool.getName() == RulerTool::k_toolName) {
            m_rulerToolAction->setChecked(enabled);
        }
    });

    m_gridToolAction = new QAction(QIcon(":/images/GridTool.svg"), tr("&Screen Grid"), this);
    m_gridToolAction->setCheckable(true);
    m_gridToolAction->setToolTip(tr("Overlays a grid"));
    m_gridToolAction->setStatusTip(tr("Overlays a grid"));
    m_gridToolAction->setWhatsThis(tr("Overlays a grid. The grid spacing, origin and angle can be adjusted."));
    connect(m_gridToolAction, &QAction::triggered, this, [this](bool checked) {
        m_toolMgr->setEnabled(GridTool::k_toolName, checked);
    });
    connect(m_toolMgr, &ToolMgr::toolEnabled, this, [this](Tool& tool, bool enabled) {
        if (tool.getName() == GridTool::k_toolName) {
            m_gridToolAction->setChecked(enabled);
        }
    });

    m_gridAdjustAction = new QAction(tr("Screen &Grid Settings..."), this);
    m_gridAdjustAction->setStatusTip(tr("Adjusts grid spacing, origin and angle"));
    m_gridAdjustAction->setWhatsThis(tr("Provides a dialog to adjust the grid spacing, origin and angle."));
    connect(m_gridAdjustAction, &QAction::triggered, this, &MainWindow::adjustGrid);

    // Units actions

    auto* linearUnitsGroup = new QActionGroup(this);
    linearUnitsGroup->setExclusive(true);

    m_pixelUnitsAction = new QAction(tr("&Pixels"), linearUnitsGroup);
    m_pixelUnitsAction->setCheckable(true);
    m_pixelUnitsAction->setStatusTip(tr("Native pixel units"));
    m_pixelUnitsAction->setWhatsThis(tr("Show measurements using pixel units."));
    connect(m_pixelUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(PixelsId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_pixelUnitsAction->setChecked(id == PixelsId);
    });

    m_twipUnitsAction = new QAction(tr("&Twips"), linearUnitsGroup);
    m_twipUnitsAction->setCheckable(true);
    m_twipUnitsAction->setStatusTip(tr("1 twip = 1/1440 inch"));
    m_twipUnitsAction->setWhatsThis(tr("Show measurements using twip units (1440 twips per inch)."));
    connect(m_twipUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(TwipsId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_twipUnitsAction->setChecked(id == TwipsId);
    });

    m_pointUnitsAction = new QAction(tr("P&oints"), linearUnitsGroup);
    m_pointUnitsAction->setCheckable(true);
    m_pointUnitsAction->setStatusTip(tr("1 point = 1/72 inch"));
    m_pointUnitsAction->setWhatsThis(tr("Show measurements using point units (72 points per inch)."));
    connect(m_pointUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(PointsId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_pointUnitsAction->setChecked(id == PointsId);
    });

    m_picaUnitsAction = new QAction(tr("Pic&as"), linearUnitsGroup);
    m_picaUnitsAction->setCheckable(true);
    m_picaUnitsAction->setStatusTip(tr("1 pica = 1/6 inch"));
    m_picaUnitsAction->setWhatsThis(tr("Show measurements using pica units (6 picas per inch)."));
    connect(m_picaUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(PicasId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_picaUnitsAction->setChecked(id == PicasId);
    });

    m_inchUnitsAction = new QAction(tr("&Inches"), linearUnitsGroup);
    m_inchUnitsAction->setCheckable(true);
    m_inchUnitsAction->setStatusTip(tr("Selects inch units"));
    m_inchUnitsAction->setWhatsThis(tr("Show measurements using inch units."));
    connect(m_inchUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(InchesId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_inchUnitsAction->setChecked(id == InchesId);
    });

    m_centimeterUnitsAction = new QAction(tr("&Centimeters"), linearUnitsGroup);
    m_centimeterUnitsAction->setCheckable(true);
    m_centimeterUnitsAction->setStatusTip(tr("2.54 centimeters = 1 inch"));
    m_centimeterUnitsAction->setWhatsThis(tr("Show measurements using centimeter units (2.54 centimeters per inch)."));
    connect(m_centimeterUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(CentimetersId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_centimeterUnitsAction->setChecked(id == CentimetersId);
    });

    m_millimeterUnitsAction = new QAction(tr("&Millimeters"), linearUnitsGroup);
    m_millimeterUnitsAction->setCheckable(true);
    m_millimeterUnitsAction->setStatusTip(tr("25.4 millimeters = 1 inch"));
    m_millimeterUnitsAction->setWhatsThis(tr("Show measurements using millimeter units (25.4 millimeters per inch)."));
    connect(m_millimeterUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setLinearUnits(MillimetersId);
    });
    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, [this](LinearUnitsId id) {
        m_millimeterUnitsAction->setChecked(id == MillimetersId);
    });

    m_customUnitsAction = new QAction("", linearUnitsGroup);
    m_customUnitsAction->setCheckable(true);
    m_customUnitsAction->setStatusTip(tr("Selects custom units"));
    m_customUnitsAction->setWhatsThis(tr("Show measurements using custom units."));
    connect(m_unitsMgr, &UnitsMgr::customUnitsChanged, this, &MainWindow::customUnitsChanged);

    m_defineCustomUnitsAction = new QAction(tr("Define C&ustom..."), this);
    m_defineCustomUnitsAction->setStatusTip(tr("Defines custom units"));
    m_defineCustomUnitsAction->setWhatsThis(tr("Provides a dialog to define custom units."));
    connect(m_defineCustomUnitsAction, &QAction::triggered, this, [this]() {
        m_prefsDialog->execPage(PrefsPageId::UnitsPage);
    });

    auto* angularUnitsGroup = new QActionGroup(this);
    angularUnitsGroup->setExclusive(true);

    m_degreeUnitsAction = new QAction(tr("&Degrees"), angularUnitsGroup);
    m_degreeUnitsAction->setCheckable(true);
    m_degreeUnitsAction->setStatusTip(tr("360 degrees = 2\u03C0 radians"));
    m_degreeUnitsAction->setWhatsThis(tr("Shows angular measurements using degrees."));
    connect(m_degreeUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setAngularUnits(DegreesId);
    });
    connect(m_unitsMgr, &UnitsMgr::angularUnitsChanged, this, [this](AngularUnitsId id) {
        m_degreeUnitsAction->setChecked(id == DegreesId);
    });

    m_radianUnitsAction = new QAction(tr("&Radians"), angularUnitsGroup);
    m_radianUnitsAction->setCheckable(true);
    m_radianUnitsAction->setStatusTip(tr("2\u03C0 radians = 360 degrees"));
    m_radianUnitsAction->setWhatsThis(tr("Shows angular measurements using radians."));
    connect(m_radianUnitsAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setAngularUnits(RadiansId);
    });
    connect(m_unitsMgr, &UnitsMgr::angularUnitsChanged, this, [this](AngularUnitsId id) {
        m_radianUnitsAction->setChecked(id == RadiansId);
    });

    // View actions

    m_collapseAction = new QAction(this);
    m_collapseAction->setShortcut(QKeySequence("Ctrl+H"));
    m_collapseAction->setStatusTip(tr("Toggle visibility of all sections"));
    m_collapseAction->setWhatsThis(tr("Toggle the visibility of all user interface sections."));
    connect(m_collapseAction, &QAction::triggered, this, [this]() {
        setAllVisible(!m_sectionVisibility.canCollapse());
    });
    connect(this, &MainWindow::visibilityChanged, this, [this]() {
        m_collapseAction->setText(m_sectionVisibility.canCollapse() ? tr("Collapse") : tr("Expand"));
    });

    m_toolBarVisibleAction = new QAction(tr("&Tool Bar"), this);
    m_toolBarVisibleAction->setCheckable(true);
    m_toolBarVisibleAction->setStatusTip(tr("Toggle toolbar visibility"));
    m_toolBarVisibleAction->setWhatsThis(tr("Toggles the visibility of the toolbar."));
    connect(m_toolBarVisibleAction, &QAction::triggered, this, &MainWindow::setToolBarVisible);
    connect(this, &MainWindow::toolBarVisibilityChanged, m_toolBarVisibleAction, &QAction::setChecked);

    m_toolDataSectionVisibleAction = new QAction(tr("Tool &Info"), this);
    m_toolDataSectionVisibleAction->setCheckable(true);
    m_toolDataSectionVisibleAction->setStatusTip(tr("Toggle info section visibility"));
    m_toolDataSectionVisibleAction->setWhatsThis(tr("Toggles the visibility of the measurement information section."));
    connect(m_toolDataSectionVisibleAction, &QAction::triggered, this, &MainWindow::setToolDataSectionVisible);
    connect(this, &MainWindow::toolDataSectionVisibilityChanged, m_toolDataSectionVisibleAction, &QAction::setChecked);

    m_screenDataSectionVisibleAction = new QAction(tr("&Screen Info"), this);
    m_screenDataSectionVisibleAction->setCheckable(true);
    m_screenDataSectionVisibleAction->setStatusTip(tr("Toggle screen info section visibility"));
    m_screenDataSectionVisibleAction->setWhatsThis(tr("Toggles the visibility of the screen information section."));
    connect(m_screenDataSectionVisibleAction, &QAction::triggered, this, &MainWindow::setScreenDataSectionVisible);
    connect(this, &MainWindow::screenDataSectionVisibilityChanged, m_screenDataSectionVisibleAction, &QAction::setChecked);

    m_magnifierSectionVisibleAction = new QAction(tr("&Magnifier"), this);
    m_magnifierSectionVisibleAction->setCheckable(true);
    m_magnifierSectionVisibleAction->setStatusTip(tr("Toggle magnifier visibility"));
    m_magnifierSectionVisibleAction->setWhatsThis(tr("Toggles the visibility of the magnifier."));
    connect(m_magnifierSectionVisibleAction, &QAction::triggered, this, &MainWindow::setMagnifierSectionVisible);
    connect(this, &MainWindow::magnifierSectionVisibilityChanged, m_magnifierSectionVisibleAction, &QAction::setChecked);

    m_statusBarVisibleAction = new QAction(tr("Status &Bar"), this);
    m_statusBarVisibleAction->setCheckable(true);
    m_statusBarVisibleAction->setStatusTip(tr("Toggle status bar visibility"));
    m_statusBarVisibleAction->setWhatsThis(tr("Toggles the visibility of the status bar."));
    connect(m_statusBarVisibleAction, &QAction::triggered, this, &MainWindow::setStatusBarVisible);
    connect(this, &MainWindow::statusBarVisibilityChanged, m_statusBarVisibleAction, &QAction::setChecked);

    m_hideCrosshairsAction = new QAction(tr("&Hide Crosshairs"), this);
    m_hideCrosshairsAction->setShortcut(QKeySequence("Ctrl+B"));
    m_hideCrosshairsAction->setCheckable(true);
    m_hideCrosshairsAction->setStatusTip(tr("Toggle crosshair visibility"));
    m_hideCrosshairsAction->setWhatsThis(tr("Toggles the visibility of the tool crosshairs."));
    connect(m_hideCrosshairsAction, &QAction::triggered, this, [this](bool hide) {
        m_toolMgr->setCrosshairsEnabled(!hide);
    });
    connect(m_toolMgr, &ToolMgr::crosshairsEnabled, this, [this](bool enable) {
        m_hideCrosshairsAction->setChecked(!enable);
    });

    m_hideDataWindowsAction = new QAction(tr("Hide &Popup Data Windows"), this);
    m_hideDataWindowsAction->setCheckable(true);
    m_hideDataWindowsAction->setStatusTip(tr("Toggle data window visibility"));
    m_hideDataWindowsAction->setWhatsThis(tr("Toggles the visibility of the tool popup data windows."));
    connect(m_hideDataWindowsAction, &QAction::triggered, this, [this](bool hide) {
        m_toolMgr->setDataWinEnabled(!hide);
    });
    connect(m_toolMgr, &ToolMgr::dataWinEnabled, this, [this](bool enable) {
        m_hideDataWindowsAction->setChecked(!enable);
    });

    m_hideOriginToolAction = new QAction(tr("Hide &Origin Marker"), this);
    m_hideOriginToolAction->setCheckable(true);
    m_hideOriginToolAction->setStatusTip(tr("Toggle origin marker visibility"));
    m_hideOriginToolAction->setWhatsThis(tr("Toggles the visibility of the origin marker."));
    connect(m_hideOriginToolAction, &QAction::triggered, this, [this](bool hide) {
        m_toolMgr->setEnabled(OriginTool::k_toolName, !hide);
    });
    connect(m_toolMgr, &ToolMgr::toolEnabled, this, [this](Tool& tool, bool enabled) {
        if (tool.getName() == OriginTool::k_toolName) {
            m_hideOriginToolAction->setChecked(!enabled);
        }
    });

    m_invertYAction = new QAction(tr("Invert &Y"), this);
    m_invertYAction->setCheckable(true);
    m_invertYAction->setStatusTip(tr("Toggle direction of the y-axis"));
    m_invertYAction->setWhatsThis(tr("Toggles the origin and direction of the y-axis. Default origin is at the top "
                                     "of the screen with positive y pointing downward."));
    connect(m_invertYAction, &QAction::triggered, m_unitsMgr, &UnitsMgr::setInvertY);
    connect(m_unitsMgr, &UnitsMgr::invertYChanged, m_invertYAction, &QAction::setChecked);

    m_supplementalAngleAction = new QAction(tr("Supplemental &Angle"));
    m_supplementalAngleAction->setCheckable(true);
    m_supplementalAngleAction->setStatusTip(tr("Toggle supplemental angle"));
    m_supplementalAngleAction->setWhatsThis(tr("Toggles the angle display from included angle to supplemental "
                                               "angle (180 - included angle)."));
    connect(m_supplementalAngleAction, &QAction::triggered, m_unitsMgr, &UnitsMgr::setSupplementalAngle);
    connect(m_unitsMgr, &UnitsMgr::supplementalAngleChanged, m_supplementalAngleAction, &QAction::setChecked);

    m_setOriginAction = new QAction(tr("Set &Origin"), this);
    m_setOriginAction->setShortcut(QKeySequence("Ctrl+N"));
    m_setOriginAction->setStatusTip(tr("Set the origin location"));
    m_setOriginAction->setWhatsThis(tr("Sets the origin to the current tool's active position"));
    connect(m_setOriginAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setOrigin(m_toolMgr->getActivePosition());
    });

    m_resetOriginAction = new QAction(tr("&Reset Origin"), this);
    m_resetOriginAction->setStatusTip(tr("Resets origin location"));
    m_resetOriginAction->setWhatsThis(tr("Resets the origin to the default position (top, left or bottom, left if "
                                         "y-axis is inverted)."));
    connect(m_resetOriginAction, &QAction::triggered, this, [this] {
        m_unitsMgr->setOrigin(QPoint(0, 0));
    });

    m_alwaysVisibleAction = new QAction(tr("Always &Visible"), this);
    m_alwaysVisibleAction->setCheckable(true);
    m_alwaysVisibleAction->setStatusTip(tr("Ensures application always visible"));
    m_alwaysVisibleAction->setWhatsThis(tr("Toggles whether the application can be hidden by other windows"));
    connect(m_alwaysVisibleAction, &QAction::triggered, this, &MainWindow::setAlwaysVisible);
    connect(this, &MainWindow::alwaysVisibleChanged, m_alwaysVisibleAction, &QAction::setChecked);

    // Help actions

    m_helpAction = new QAction(tr("Help"), this);
    m_helpAction->setIcon(QIcon::fromTheme("help-browser"));
    m_helpAction->setShortcut(QKeySequence::HelpContents);
    m_helpAction->setStatusTip(tr("Opens the user manual"));
    m_helpAction->setWhatsThis(tr("Opens the user manual in a web browser"));
    m_helpAction->setEnabled(HelpUtils::isHelpPresent());
    connect(m_helpAction, &QAction::triggered, this, []() {
        HelpUtils::openHelp();
    });

    m_whatsThisAction = QWhatsThis::createAction(this);
    m_whatsThisAction->setIcon(QIcon::fromTheme("dialog-question"));
    m_whatsThisAction->setStatusTip(tr("Provides context-sensitive help"));
    m_whatsThisAction->setWhatsThis(tr("Provides context-sensitive help by clicking on user interface items"));

    m_releasesAction = new QAction(tr("&Releases"), this);
    m_releasesAction->setStatusTip(tr("Browse Meazure releases"));
    m_releasesAction->setWhatsThis(tr("Opens web page showing Meazure releases."));
    connect(m_releasesAction, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/cthing/meazure-linux/releases"));
    });

    m_reportIssueAction = new QAction(tr("Report an &Issue"), this);
    m_reportIssueAction->setStatusTip(tr("Submit an an issue"));
    m_reportIssueAction->setWhatsThis(tr("Opens web page to submit a bug report or feature request."));
    connect(m_reportIssueAction, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/cthing/meazure-linux/issues"));
    });

    m_aboutAction = new QAction(tr("&About"), this);
    m_aboutAction->setStatusTip(tr("Show application information"));
    m_aboutAction->setWhatsThis(tr("Shows application version and other information."));
    connect(m_aboutAction, &QAction::triggered, m_aboutDialog, &AboutDialog::exec);
}

void MainWindow::createMenus() {
    // File menu

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_loadPositionsAction);
    fileMenu->addAction(m_savePositionsAction);
    fileMenu->addAction(m_saveAsPositionsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_importConfigAction);
    fileMenu->addAction(m_exportConfigAction);
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
    viewMenu->addAction(m_mainView->getMagnifierFreezeAction());

    QMenu* magGridMenu = viewMenu->addMenu("Magnifier &Grid");
    for (QAction* gridType : m_mainView->getMagnifierGridActions()) {
        magGridMenu->addAction(gridType);
    }

    QMenu* colorMenu = viewMenu->addMenu("&Color Format");
    bool inColorMatching = false;
    for (QAction* colorFormat : m_mainView->getColorFormatActions()) {
        const bool matching = colorFormat->data().toBool();
        if (inColorMatching != matching) {
            inColorMatching = matching;
            colorMenu->addSeparator();
        }
        colorMenu->addAction(colorFormat);
    }

    viewMenu->addSeparator();
    viewMenu->addAction(m_invertYAction);
    viewMenu->addAction(m_supplementalAngleAction);
    viewMenu->addAction(m_setOriginAction);
    viewMenu->addAction(m_resetOriginAction);
    viewMenu->addSeparator();
    viewMenu->addAction(m_alwaysVisibleAction);

    // Help menu

    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(m_helpAction);
    helpMenu->addAction(m_whatsThisAction);
    helpMenu->addSeparator();
    helpMenu->addAction(m_releasesAction);
    helpMenu->addAction(m_reportIssueAction);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::createToolBar() {
    m_toolBar = addToolBar("");
    m_toolBar->setFloatable(false);
    m_toolBar->setMovable(false);
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
    auto* gridTool = dynamic_cast<GridTool*>(m_toolMgr->getTool(GridTool::k_toolName));
    m_gridDialog = new GridDialog(gridTool, m_screenInfo, m_unitsMgr, this);

    m_prefsDialog = new PrefsDialog(m_screenInfo, m_unitsMgr, m_configMgr, this);

    m_positionDialog = new PosLogManageDlg(m_posLogMgr, this);

    m_aboutDialog = new AboutDialog(m_screenInfo, this);
}

void MainWindow::createKeyboardControl() {
    auto* globalShortcuts = new GlobalShortcuts(this);

    connect(globalShortcuts, &GlobalShortcuts::ctrl1Up, this, [this]() { m_toolMgr->stepY1Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl1Down, this, [this]() { m_toolMgr->stepY1Position(1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl1Left, this, [this]() { m_toolMgr->stepX1Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl1Right, this, [this]() { m_toolMgr->stepX1Position(1); });

    connect(globalShortcuts, &GlobalShortcuts::ctrl2Up, this, [this]() { m_toolMgr->stepY2Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl2Down, this, [this]() { m_toolMgr->stepY2Position(1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl2Left, this, [this]() { m_toolMgr->stepX2Position(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl2Right, this, [this]() { m_toolMgr->stepX2Position(1); });

    connect(globalShortcuts, &GlobalShortcuts::ctrl3Up, this, [this]() { m_toolMgr->stepYVPosition(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl3Down, this, [this]() { m_toolMgr->stepYVPosition(1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl3Left, this, [this]() { m_toolMgr->stepXVPosition(-1); });
    connect(globalShortcuts, &GlobalShortcuts::ctrl3Right, this, [this]() { m_toolMgr->stepXVPosition(1); });

    connect(globalShortcuts, &GlobalShortcuts::ctrlShift1, this, [this]() {
        const QPointF coord = m_unitsMgr->convertCoord(QCursor::pos());
        m_toolMgr->setX1Position(coord.x());
        m_toolMgr->setY1Position(coord.y());
    });
    connect(globalShortcuts, &GlobalShortcuts::ctrlShift2, this, [this]() {
        const QPointF coord = m_unitsMgr->convertCoord(QCursor::pos());
        m_toolMgr->setX2Position(coord.x());
        m_toolMgr->setY2Position(coord.y());
    });
    connect(globalShortcuts, &GlobalShortcuts::ctrlShift3, this, [this]() {
        const QPointF coord = m_unitsMgr->convertCoord(QCursor::pos());
        m_toolMgr->setXVPosition(coord.x());
        m_toolMgr->setYVPosition(coord.y());
    });

    QCoreApplication::instance()->installEventFilter(globalShortcuts);
}

void MainWindow::writeConfig(Config& config) const {
    if (config.isPersistent()) {
        const QRect geom = geometry();
        config.writeInt("WindowLeft", geom.x());
        config.writeInt("WindowTop", geom.y());
    }

    config.writeBool("AlwaysVisible", isAlwaysVisible());
    config.writeBool("ExpandToolbar", m_sectionVisibility.m_toolBarVisible);
    config.writeBool("ExpandToolInfo", m_sectionVisibility.m_toolDataSectionVisible);
    config.writeBool("ExpandScreenInfo", m_sectionVisibility.m_screenDataSectionVisible);
    config.writeBool("ExpandMagnifier", m_sectionVisibility.m_magnifierSectionVisible);
    config.writeBool("ExpandStatusbar", m_sectionVisibility.m_statusBarVisible);

    m_mainView->writeConfig(config);
}

void MainWindow::readConfig(const Config& config) {
    m_mainView->readConfig(config);

    setAlwaysVisible(config.readBool("AlwaysVisible", isAlwaysVisible()));
    setToolBarVisible(config.readBool("ExpandToolbar", m_sectionVisibility.m_toolBarVisible));
    setToolDataSectionVisible(config.readBool("ExpandToolInfo", m_sectionVisibility.m_toolDataSectionVisible));
    setScreenDataSectionVisible(config.readBool("ExpandScreenInfo", m_sectionVisibility.m_screenDataSectionVisible));
    setMagnifierSectionVisible(config.readBool("ExpandMagnifier", m_sectionVisibility.m_magnifierSectionVisible));
    setStatusBarVisible(config.readBool("ExpandStatusbar", m_sectionVisibility.m_statusBarVisible));

    if (config.isPersistent()) {
        QRect geom = geometry();
        geom.setX(config.readInt("WindowLeft", geom.x()));
        geom.setY(config.readInt("WindowTop", geom.y()));
        setGeometry(geom);
    }
}

void MainWindow::hardReset() {
    m_mainView->hardReset();

    setAlwaysVisible();
    setAllVisible(true);
}

void MainWindow::radioToolSelected(RadioTool& tool) {
    m_copyRegionAction->setEnabled(tool.canGrabRegion());

    statusBar()->showMessage(tool.getInstructions());
}

void MainWindow::copyRegion() {
    RadioTool* radioTool = m_toolMgr->getCurentRadioTool();
    if (radioTool->canGrabRegion()) {
        const QImage image = radioTool->grabRegion();
        if (!image.isNull()) {
            QGuiApplication::clipboard()->setImage(image);
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
    const CustomUnits* customUnits = m_unitsMgr->getCustomUnits();
    const bool defined = customUnits->haveCustomUnits();
    m_customUnitsAction->setEnabled(defined);
    m_customUnitsAction->setText(defined ? customUnits->getName() : tr("[custom]"));

    if (!defined && m_customUnitsAction->isChecked()) {
        m_unitsMgr->setLinearUnits(PixelsId);
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
            accept = m_posLogMgr->savePositions();
        } else {
            accept = (response == QMessageBox::No);
        }
    }

    if (accept) {
        m_configMgr->saveConfig();
    }

    event->setAccepted(accept);
}
