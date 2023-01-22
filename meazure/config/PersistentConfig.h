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
#include <QString>
#include <QSettings>


/// Persists the application state to platform-specific settings storage. On application startup this configuration is
/// read to restore the application state from the last time it was run. On exit, this configuration is written so that
/// the application can be restored to the same state when it is next run.
///
class PersistentConfig : public Config {

public:
    /// Creates a persistent configuration using the default settings storage location.
    ///
    PersistentConfig();

    /// @internal
    /// Creates a persistent configuration using the specified pathname. If the pathname does not exist,
    /// it will be created. This method is used for testing purposes.
    ///
    /// @param[in] pathname Pathname of the settings file
    ///
    explicit PersistentConfig(const QString& pathname);

    ~PersistentConfig() override;

    PersistentConfig(const PersistentConfig&) = delete;
    PersistentConfig(PersistentConfig&&) = delete;
    PersistentConfig& operator=(const PersistentConfig&) = delete;

    void writeBool(const QString& key, bool value) override;
    void writeInt(const QString& key, int value) override;
    void writeUInt(const QString& key, unsigned int value) override;
    void writeDbl(const QString& key, double value) override;
    void writeStr(const QString& key, const QString& value) override;

    [[nodiscard]] bool readBool(const QString& key, bool defaultValue) const override;
    [[nodiscard]] int readInt(const QString& key, int defaultValue) const override;
    [[nodiscard]] unsigned int readUInt(const QString& key, unsigned int defaultValue) const override;
    [[nodiscard]] double readDbl(const QString& key, double defaultValue) const override;
    [[nodiscard]] QString readStr(const QString& key, const QString& defaultValue) const override;

    [[nodiscard]] bool isPersistent() const override;

    [[nodiscard]] int getVersion() const override;

private:
#ifdef Q_OS_LINUX
    static constexpr const char* k_orgKey { "CThingSoftware" };
#elif defined(Q_OS_WIN)
    static constexpr const char* k_orgKey { "C Thing Software" };
#elif defined(Q_OS_MACOS)
    static constexpr const char* k_orgKey { "CThingSoftware" };
#endif
    static constexpr const char* k_appKey { "Meazure" };
    static constexpr int k_version { 3 };

    QSettings* m_settings;
};
