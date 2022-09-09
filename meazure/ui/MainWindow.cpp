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
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QActionGroup>


MainWindow::MainWindow() {          // NOLINT(cppcoreguidelines-pro-type-member-init)
    createCentralWidget();
    createActions();
    createMenus();
    createToolbar();

    statusBar()->showMessage(tr("Hello"));
}

void MainWindow::createActions() {
    auto* radioToolGroup = new QActionGroup(this);
    radioToolGroup->setExclusive(true);

    m_cursorToolAction = new QAction(QIcon(":/images/CursorTool.svg"), tr("&Cursor"), radioToolGroup);
    m_cursorToolAction->setCheckable(true);
    m_cursorToolAction->setToolTip("Tracks cursor position");

    m_pointToolAction = new QAction(QIcon(":/images/PointTool.svg"), tr("&Point"), radioToolGroup);
    m_pointToolAction->setCheckable(true);
    m_pointToolAction->setToolTip("Measures a point");

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
    m_windowToolAction->setToolTip("Meazures a window");

    m_rulerToolAction = new QAction(QIcon(":/images/RulerTool.svg"), tr("R&uler"), this);
    m_rulerToolAction->setCheckable(true);
    m_rulerToolAction->setToolTip("Adds screen rulers");

    m_gridToolAction = new QAction(QIcon(":/images/GridTool.svg"), tr("&Screen Grid"), this);
    m_gridToolAction->setCheckable(true);
    m_gridToolAction->setToolTip("Adds screen grid");
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
