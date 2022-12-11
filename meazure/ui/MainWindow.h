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

#pragma once

#include <meazure/tools/RadioTool.h>
#include <meazure/units/Units.h>
#include "GridDialog.h"
#include <QMainWindow>
#include <QAction>


/// Top level user interface layout manager.
///
class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    MainWindow();

private slots:
    void radioToolSelected(RadioTool& tool);

    static void copyRegion();

    void adjustGrid();

private:
    static constexpr int k_toolbarIconSize { 20 };

    /// Creates actions that are referenced in multiple places in the application.
    ///
    void createActions();

    /// Creates the application's menus.
    ///
    void createMenus();

    /// Creates the application's toolbar.
    ///
    void createToolbar();

    /// Creates application dialogs.
    ///
    void createDialogs();

    /// Creates the application's main widget.
    ///
    void createCentralWidget();

    QAction* m_copyRegionAction;

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

    QAction* m_invertYAction;
    QAction* m_supplementalAngleAction;
    QAction* m_setOriginAction;
    QAction* m_resetOriginAction;

    GridDialog* m_gridDialog;
};
