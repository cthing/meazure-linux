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
#include <QObject>
#include <QString>


/// Supervises the persistence and exporting of the application configuration. The configuration represents the state
/// of the application (e.g. currently selected tools, color preferences, etc.) so that it can be saved and restored.
///
class ConfigMgr : public QObject {

    Q_OBJECT

public:
    ConfigMgr();

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

    QString m_exportPathname;
    QString m_importPathname;
    QString m_initialDir;
};
