/*
 * Copyright 2023 C Thing Software
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

#include "Config.h"
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/tools/ToolMgr.h>
#include <meazure/position-log/PosLogMgr.h>
#include <QObject>
#include <QString>


class MainWindow;


/// Supervises the persistence and exporting of the application configuration. The configuration represents the state
/// of the application (e.g. currently selected tools, color preferences, etc.) so that it can be saved and restored.
///
class ConfigMgr : public QObject {

    Q_OBJECT

public:
    ConfigMgr(ScreenInfo* screenInfo, UnitsMgr* unitsMgr, ToolMgr* toolMgr, PosLogMgr* posLogMgr);

    void setMainWindow(MainWindow* mainWindow);

public slots:
    void exportConfig();
    void importConfig();
    void import(const QString& pathname);

    void saveConfig();
    void restoreConfig();

private:
    static constexpr const char* k_fileFilter { "Meazure Configuration Files (*.mea);;All Files (*.*)" };
    static constexpr const char* k_fileSuffix { ".mea" };

    void writeConfig(Config& config) const;
    void readConfig(const Config& config);

    ScreenInfo* m_screenInfo;
    UnitsMgr* m_unitsMgr;
    ToolMgr* m_toolMgr;
    PosLogMgr* m_posLogMgr;
    MainWindow* m_mainWindow { nullptr };
    QString m_exportPathname;
    QString m_importPathname;
    QString m_initialDir;
};
