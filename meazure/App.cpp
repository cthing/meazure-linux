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

#include "App.h"
#include "AppVersion.h"
#include <QtPlugin>
#include <QCommandLineParser>
#include <QStyleFactory>
#include <QPixmap>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


App::App(int &argc, char **argv):
        QApplication(argc, argv),
        m_screenInfo(screens()),
        m_unitsMgr(m_screenInfo),
        m_toolMgr(m_screenInfo, m_unitsMgr),
        m_posLogMgr(m_toolMgr, m_screenInfo, m_unitsMgr) {
    setApplicationName("meazure");
    setApplicationDisplayName("Meazure");
    setApplicationVersion(appVersion);

    setOrganizationName("cthing");
    setOrganizationDomain("cthing.com");

    setWindowIcon(QPixmap(":/images/Meazure.png"));

    QCommandLineParser parser;
    parser.setApplicationDescription("A tool for easily measuring and capturing portions of the screen.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(*this);

    m_mainWindow.setAttribute(Qt::WA_QuitOnClose, true);
    m_mainWindow.show();
}
