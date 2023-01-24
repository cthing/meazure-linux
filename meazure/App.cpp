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
#include <QStyleFactory>
#include <QPixmap>
#include <QDir>
#include <QtGlobal>
#include <unicode/putil.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


App::App(int &argc, char **argv): QApplication(argc, argv) {     // NOLINT(cppcoreguidelines-pro-type-member-init)
    // Because the ICU library is statically compiled, its data file is not available at runtime. The data file is
    // distributed with the application in the "icu" subdirectory. Point the ICU library at this directory.
    u_setDataDirectory(findAppDataDir(k_icuDir).toUtf8().constData());

    // Set application metadata.
    setApplicationName("meazure");
    setApplicationVersion(appVersion);
    setOrganizationName("C Thing Software");
    setOrganizationDomain("cthing.com");
    setWindowIcon(QPixmap(":/images/Meazure.png"));

    // Parse the command-line.
    QCommandLineParser parser;
    parseCommandLine(parser);

    // Determine if running in development mode.
    const bool devMode = findDevMode(parser);

    // Create the singleton objects.
    m_screenInfo = new ScreenInfo(screens());                                                     // NOLINT(cppcoreguidelines-prefer-member-initializer)
    m_unitsMgr = new UnitsMgr(m_screenInfo);                                                      // NOLINT(cppcoreguidelines-prefer-member-initializer)
    m_toolMgr = new ToolMgr(m_screenInfo, m_unitsMgr);                                            // NOLINT(cppcoreguidelines-prefer-member-initializer)
    m_posLogMgr = new PosLogMgr(m_screenInfo, m_unitsMgr, m_toolMgr);                             // NOLINT(cppcoreguidelines-prefer-member-initializer)
    m_configMgr = new ConfigMgr(devMode);                                                         // NOLINT(cppcoreguidelines-prefer-member-initializer)
    m_mainWindow = new MainWindow(m_screenInfo, m_unitsMgr, m_toolMgr, m_posLogMgr, m_configMgr); // NOLINT(cppcoreguidelines-prefer-member-initializer)

    // Populate the configuration manager with the objects that will participate in saving, restoring
    // and resetting the application configuration.
    populateConfigMgr();

    // Restore the save application state.
    m_configMgr->restoreConfig();

    // Display the application window.
    m_mainWindow->setAttribute(Qt::WA_QuitOnClose, true);
    m_mainWindow->show();

    // Load a position log file, if one was specified on the command-line
    if (parser.isSet("positions")) {
        m_posLogMgr->load(parser.value("positions"));
    }

    // Load a configuration file, if one was specified on the command-line
    if (parser.isSet("config")) {
        m_configMgr->import(parser.value("config"));
    }
}

App::~App() {
    delete m_configMgr;
    delete m_mainWindow;
    delete m_posLogMgr;
    delete m_toolMgr;
    delete m_unitsMgr;
    delete m_screenInfo;
}

void App::parseCommandLine(QCommandLineParser& parser) {
    QCommandLineOption devModeOption(k_devmodeOpt, tr("Enable or disable development mode <bool>."),
                                     tr("bool"), "false");
    devModeOption.setFlags(QCommandLineOption::HiddenFromHelp);

    parser.setApplicationDescription("A tool for easily measuring and capturing portions of the screen.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
        {
            { k_positionsShortOpt, k_positionsLongOpt }, tr("Load the position log file <filename>."), tr("filename")
        }, {
            { k_configShortOpt, k_configLongOpt }, tr("Load the configuration file <filename>."), tr("filename")
        }
    });
    parser.addOption(devModeOption);
    parser.process(*this);
}

bool App::findDevMode(const QCommandLineParser& parser) {
    bool mode = false;

    if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + k_devmodeMarkerFilename).exists()) {
        mode = true;
    }

    if (parser.isSet(k_devmodeOpt)) {
        const QString value = parser.value(k_devmodeOpt).toLower();
        if (value == "true" || value == "t" || value == "enable") {
            mode = true;
        } else if (value == "false" || value == "f" || value == "disable") {
            mode = false;
        }
    }

    return mode;
}

void App::populateConfigMgr() {
    m_configMgr->registerWriter([this](Config& config) { m_mainWindow->writeConfig(config); },
                                [this](Config& config) { m_posLogMgr->writeConfig(config); },
                                [this](Config& config) { m_toolMgr->writeConfig(config); },
                                [this](Config& config) { m_unitsMgr->writeConfig(config); },
                                [this](Config& config) { m_screenInfo->writeConfig(config); },
                                &Colors::writeConfig,
                                &Dimensions::writeConfig);

    m_configMgr->registerReader(&Dimensions::readConfig,
                                &Colors::readConfig,
                                [this](const Config& config) { m_screenInfo->readConfig(config); },
                                [this](const Config& config) { m_unitsMgr->readConfig(config); },
                                [this](const Config& config) { m_toolMgr->readConfig(config); },
                                [this](const Config& config) { m_posLogMgr->readConfig(config); },
                                [this](const Config& config) { m_mainWindow->readConfig(config); });
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
