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

#include "ui/MainWindow.h"
#include "ui/WaylandAlert.h"
#include "environment/ScreenInfo.h"
#include "units/UnitsMgr.h"
#include "tools/ToolMgr.h"
#include "position-log/PosLogMgr.h"
#include "config/ConfigMgr.h"
#include <QApplication>
#include <QString>
#include <QCommandLineParser>


/// Represents the application.
///
class App : public QApplication {

public:
    /// Constructs the application.
    ///
    /// @param[in] argc Number of command line arguments
    /// @param[in] argv Command line arguments
    ///
    App(int& argc, char** argv);

    ~App() override;

    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;

private:
    // Command-line options
    static constexpr const char* k_devmodeOpt { "devmode" };
    static constexpr const char* k_resetOpt { "reset" };

    static constexpr const char* k_devmodeMarkerFilename { "meadevmode" };
    static constexpr const char* k_icuDir { "icu" };

    void parseCommandLine(QCommandLineParser& parser);

    static bool findDevMode(const QCommandLineParser& parser);

    void populateConfigMgr();

    ScreenInfo* m_screenInfo;
    UnitsMgr* m_unitsMgr;
    ToolMgr* m_toolMgr;
    PosLogMgr* m_posLogMgr;
    ConfigMgr* m_configMgr;
    MainWindow* m_mainWindow { nullptr };
    WaylandAlert* m_waylandAlert { nullptr };
};
