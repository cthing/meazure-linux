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

#include "ui/MainWindow.h"
#include "environment/ScreenInfo.h"
#include "units/UnitsMgr.h"
#include "tools/ToolMgr.h"
#include "position-log/PosLogMgr.h"
#include "profile/ProfileMgr.h"
#include <QApplication>
#include <QString>


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

    ~App() override = default;

    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;

    static inline App* instance() {
        return dynamic_cast<App*>(QApplication::instance());
    }

    [[nodiscard]] const ScreenInfo& getScreenInfo() const { return m_screenInfo; }
    [[nodiscard]] ScreenInfo& getScreenInfo() { return m_screenInfo; }
    [[nodiscard]] UnitsMgr& getUnitsMgr() { return m_unitsMgr; }
    [[nodiscard]] ToolMgr& getToolMgr() { return m_toolMgr; }
    [[nodiscard]] PosLogMgr& getPosLogMgr() { return m_posLogMgr; }
    [[nodiscard]] ProfileMgr& getProfileMgr() { return m_profileMgr; }
    [[nodiscard]] MainWindow& getMainWindow() { return m_mainWindow; }

    static QString findAppDataDir(const QString& subdir = QString());
    static QString findAppDataFile(const QString& filename);

private:
    static constexpr const char* k_icuDir { "icu" };

    ScreenInfo m_screenInfo;
    UnitsMgr m_unitsMgr;
    ToolMgr m_toolMgr;
    PosLogMgr m_posLogMgr;
    ProfileMgr m_profileMgr;
    MainWindow m_mainWindow;
};
