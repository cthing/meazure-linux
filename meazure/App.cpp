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
#include "utils/PlatformUtils.h"
#include <QtPlugin>
#include <QStyleFactory>
#include <QPixmap>
#include <QDir>
#include <QtGlobal>
#include <QRegularExpression>
#include <unicode/putil.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


App::App(int &argc, char **argv): QApplication(argc, argv) {     // NOLINT(cppcoreguidelines-pro-type-member-init)
    // Because the ICU library is statically compiled, its data file is not available at runtime. The data file is
    // distributed with the application in the "icu" subdirectory. Point the ICU library at this directory.
    u_setDataDirectory(PlatformUtils::findAppDataDir(k_icuDir).toUtf8().constData());

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

    if (PlatformUtils::isWayland()) {
        m_waylandAlert = new WaylandAlert();                                                          // NOLINT(cppcoreguidelines-prefer-member-initializer)
        m_waylandAlert->setAttribute(Qt::WA_QuitOnClose, true);
        m_waylandAlert->show();
    } else {
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
        const QStringList positionLogs = parser.positionalArguments().filter(QRegularExpression(".*\\.mpl$"));
        if (!positionLogs.empty()) {
            m_posLogMgr->load(positionLogs.last());
        }

        // Load a configuration file, if one was specified on the command-line
        const QStringList configurations = parser.positionalArguments().filter(QRegularExpression(".*\\.mea$"));
        if (!configurations.empty()) {
            m_configMgr->import(configurations.last());
        }
    }
}

App::~App() {
    delete m_configMgr;
    delete m_mainWindow;
    delete m_waylandAlert;
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
    parser.addOption(devModeOption);
    parser.addPositionalArgument("*.mea", tr("Configuration file"), "[*.mea]");
    parser.addPositionalArgument("*.mpl", tr("Position log file"), "[*.mpl]");
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
    m_configMgr->registerWriters([this](Config& config) { m_mainWindow->writeConfig(config); },
                                 [this](Config& config) { m_posLogMgr->writeConfig(config); },
                                 [this](Config& config) { m_toolMgr->writeConfig(config); },
                                 [this](Config& config) { m_unitsMgr->writeConfig(config); },
                                 [this](Config& config) { m_screenInfo->writeConfig(config); },
                                 &Colors::writeConfig,
                                 &Dimensions::writeConfig);

    m_configMgr->registerReaders(&Dimensions::readConfig,
                                 &Colors::readConfig,
                                 [this](const Config& config) { m_screenInfo->readConfig(config); },
                                 [this](const Config& config) { m_unitsMgr->readConfig(config); },
                                 [this](const Config& config) { m_toolMgr->readConfig(config); },
                                 [this](const Config& config) { m_posLogMgr->readConfig(config); },
                                 [this](const Config& config) { m_mainWindow->readConfig(config); });

    m_configMgr->registerResets(&Dimensions::hardReset,
                                &Colors::hardReset,
                                [this]() { m_screenInfo->hardReset(); },
                                [this]() { m_unitsMgr->hardReset(); },
                                [this]() { m_toolMgr->hardReset(); },
                                [this]() { m_posLogMgr->hardRest(); },
                                [this]() { m_mainWindow->hardReset(); });
}
