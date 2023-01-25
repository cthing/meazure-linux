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
#include <vector>
#include <functional>


class MainWindow;


/// Supervises the persistence and exporting of the application configuration. The configuration represents the state
/// of the application (e.g. currently selected tools, color preferences, etc.) so that it can be saved and restored.
///
class ConfigMgr : public QObject {

    Q_OBJECT

public:
    using WriteConfig = std::function<void(Config&)>;
    using ReadConfig = std::function<void(const Config&)>;
    using HardReset = std::function<void(void)>;

    explicit ConfigMgr(bool devMode);

    template <class ...WRITERS>
    void registerWriters(const WRITERS&... writers) {
        m_writers = { writers... };
    };

    template <class ...READERS>
    void registerReaders(const READERS&... readers) {
        m_readers = { readers... };
    }

    template <class ...RESETS>
    void registerResets(const RESETS&... resets) {
        m_resets = { resets... };
    }

public slots:
    void exportConfig();
    void importConfig();
    void import(const QString& pathname);

    void saveConfig();
    void restoreConfig();

    void hardReset();

private:
    static constexpr const char* k_fileFilter { "Meazure Configuration Files (*.mea);;All Files (*.*)" };
    static constexpr const char* k_fileSuffix { ".mea" };
    static constexpr const char* k_devSettingsFilename { "MeaDevSettings.conf" };

    void writeConfig(Config& config) const;
    void readConfig(const Config& config);

    [[nodiscard]] static QString getDevSettingsPathname();

    QString m_exportPathname;
    QString m_importPathname;
    QString m_initialDir;
    bool m_devMode;
    std::vector<WriteConfig> m_writers;
    std::vector<ReadConfig> m_readers;
    std::vector<HardReset> m_resets;
};
