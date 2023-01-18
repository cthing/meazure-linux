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
#include <QDir>
#include <QtGlobal>
#include <unicode/putil.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


App::App(int &argc, char **argv):
        QApplication(argc, argv),
        m_screenInfo(screens()),
        m_unitsMgr(m_screenInfo),
        m_toolMgr(m_screenInfo, m_unitsMgr),
        m_posLogMgr(m_toolMgr, m_screenInfo, m_unitsMgr) {
    // Because the ICU library is statically compiled, its data file is not available at runtime. The data file is
    // distributed with the application in the "icu" subdirectory. Point the ICU library at this directory.
    u_setDataDirectory(findAppDataDir(k_icuDir).toUtf8().constData());

    setApplicationName("meazure");
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

QString App::findAppDataDir(const QString& subdir) {
    const QString s(subdir.isNull() ? "" : ("/" + subdir));
#ifdef Q_OS_LINUX
    const QDir appDir(applicationDirPath() + s);
    if (appDir.exists()) {
        return appDir.filesystemPath().c_str();
    }

    const QDir shareDir("/usr/share/meazure" + s);
    return shareDir.filesystemPath().c_str();
#elif defined(Q_OS_WIN)
    const QDir appDir(applicationDirPath() + s);
    return appDir.filesystemPath().c_str();
#elif defined(Q_OS_MACOS)
    const QDir appDir(applicationDirPath() + s);
    if (appDir.exists()) {
        return appDir.filesystemPath().c_str();
    }

    const QString contentsDirStr(QDir(applicationDirPath()).filesystemPath().parent_path().c_str());
    const QDir resourcesDir(contentsDirStr + "/Resources" + s);
    return resourcesDir.filesystemPath().c_str();
#else
    return nullptr;
#endif
}

QString App::findAppDataFile(const QString& filename) {
    return (filename + QDir::separator() + filename).toUtf8().constData();
}
