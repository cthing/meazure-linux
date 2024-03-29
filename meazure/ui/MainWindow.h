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

#pragma once

#include "MainView.h"
#include "GridDialog.h"
#include "AboutDialog.h"
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/tools/ToolMgr.h>
#include <meazure/tools/RadioTool.h>
#include <meazure/units/Units.h>
#include <meazure/units/CustomUnits.h>
#include <meazure/config/Config.h>
#include <meazure/config/ConfigMgr.h>
#include <meazure/prefs/ui/PrefsDialog.h>
#include <meazure/position-log/PosLogMgr.h>
#include <meazure/position-log/PosLogManageDlg.h>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>


/// Top level user interface layout manager.
///
class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow(ScreenInfo* screenInfo, UnitsMgr* unitsMgr, ToolMgr* toolMgr, PosLogMgr* posLogMgr,
               ConfigMgr* configMgr);

    void writeConfig(Config& config) const;
    void readConfig(const Config& config);

    void hardReset();

signals:
    void alwaysVisibleChanged(bool alwaysVisible);
    void toolBarVisibilityChanged(bool visible);
    void toolDataSectionVisibilityChanged(bool visible);
    void screenDataSectionVisibilityChanged(bool visible);
    void magnifierSectionVisibilityChanged(bool visible);
    void statusBarVisibilityChanged(bool visible);
    void visibilityChanged();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void radioToolSelected(RadioTool& tool);

    void copyRegion();

private:
    void adjustGrid();

    void managePositions();

    void setAlwaysVisible(bool alwaysVisible = k_defAlwaysVisible);

    void setToolBarVisible(bool visible = k_defToolBarVisible);
    void setToolDataSectionVisible(bool visible = k_defToolDataSectionVisible);
    void setScreenDataSectionVisible(bool visible = k_defScreenDataSectionVisible);
    void setMagnifierSectionVisible(bool visible = k_defMagnifierSectionVisible);
    void setStatusBarVisible(bool visible = k_defStatusBarVisible);
    void setAllVisible(bool visible);

    void customUnitsChanged();

    void warnCalibrationRequired();

private:
    struct SectionVisibility {
        [[nodiscard]] bool canCollapse() const {
            return m_toolBarVisible || m_toolDataSectionVisible || m_screenDataSectionVisible
                    || m_magnifierSectionVisible || m_statusBarVisible;
        }

        bool m_toolBarVisible { k_defToolBarVisible };
        bool m_toolDataSectionVisible { k_defToolDataSectionVisible };
        bool m_screenDataSectionVisible { k_defScreenDataSectionVisible };
        bool m_magnifierSectionVisible { k_defMagnifierSectionVisible };
        bool m_statusBarVisible { k_defStatusBarVisible };
    };


    static constexpr bool k_defAlwaysVisible { true };
    static constexpr bool k_defToolBarVisible { true };
    static constexpr bool k_defToolDataSectionVisible { true };
    static constexpr bool k_defScreenDataSectionVisible { true };
    static constexpr bool k_defMagnifierSectionVisible { true };
    static constexpr bool k_defStatusBarVisible { true };

    /// Creates actions that are referenced in multiple places in the application.
    ///
    void createActions();

    /// Creates the application's menus.
    ///
    void createMenus();

    /// Creates the application's toolbar.
    ///
    void createToolBar();

    /// Configures the application's status bar.
    ///
    void createStatusBar();

    /// Creates application dialogs.
    ///
    void createDialogs();

    /// Creates the application's main widget.
    ///
    void createCentralWidget();

    /// Creates the keyboard crosshair control.
    ///
    void createKeyboardControl();

    [[nodiscard]] bool isAlwaysVisible() const;

    ScreenInfo* m_screenInfo;
    UnitsMgr* m_unitsMgr;
    ToolMgr* m_toolMgr;
    PosLogMgr* m_posLogMgr;
    ConfigMgr* m_configMgr;

    MainView* m_mainView;

    QToolBar* m_toolBar;

    QAction* m_loadPositionsAction;
    QAction* m_savePositionsAction;
    QAction* m_saveAsPositionsAction;
    QAction* m_importConfigAction;
    QAction* m_exportConfigAction;
    QAction* m_exitAction;

    QAction* m_copyRegionAction;
    QAction* m_findCrosshairsAction;
    QAction* m_recordPositionAction;
    QAction* m_managePositionsAction;
    QAction* m_deletePositionsAction;
    QAction* m_preferencesAction;

    QAction* m_cursorToolAction;
    QAction* m_pointToolAction;
    QAction* m_lineToolAction;
    QAction* m_rectangleToolAction;
    QAction* m_circleToolAction;
    QAction* m_angleToolAction;
    QAction* m_windowToolAction;
    QAction* m_rulerToolAction;
    QAction* m_gridToolAction;
    QAction* m_gridAdjustAction;

    QAction* m_pixelUnitsAction;
    QAction* m_twipUnitsAction;
    QAction* m_pointUnitsAction;
    QAction* m_picaUnitsAction;
    QAction* m_inchUnitsAction;
    QAction* m_centimeterUnitsAction;
    QAction* m_millimeterUnitsAction;
    QAction* m_customUnitsAction;
    QAction* m_defineCustomUnitsAction;
    QAction* m_degreeUnitsAction;
    QAction* m_radianUnitsAction;

    QAction* m_collapseAction;
    QAction* m_toolBarVisibleAction;
    QAction* m_toolDataSectionVisibleAction;
    QAction* m_screenDataSectionVisibleAction;
    QAction* m_magnifierSectionVisibleAction;
    QAction* m_statusBarVisibleAction;
    QAction* m_hideCrosshairsAction;
    QAction* m_hideDataWindowsAction;
    QAction* m_hideOriginToolAction;
    QAction* m_invertYAction;
    QAction* m_supplementalAngleAction;
    QAction* m_setOriginAction;
    QAction* m_resetOriginAction;
    QAction* m_alwaysVisibleAction;

    QAction* m_helpAction;
    QAction* m_whatsThisAction;
    QAction* m_releasesAction;
    QAction* m_reportIssueAction;
    QAction* m_aboutAction;

    GridDialog* m_gridDialog;
    PrefsDialog* m_prefsDialog;
    PosLogManageDlg* m_positionDialog;
    AboutDialog* m_aboutDialog;

    SectionVisibility m_sectionVisibility;
};
