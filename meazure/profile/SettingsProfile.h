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

#include "Profile.h"
#include <QString>
#include <QSettings>


/// Persists the application state to platform-specific settings storage. On application startup this profile is
/// read to restore the application state from the last time it was run. On exit, this profile is written so that
/// the application can be restored to the same state when it is next run.
///
class SettingsProfile : public Profile {

public:
    /// Creates an instance of the settings profile using the default settings storage location.
    ///
    SettingsProfile();

    /// @internal
    /// Creates an instance of the settings profile using the specified pathname. If the pathname does not exist,
    /// it will be created. This method is used for testing purposes.
    ///
    /// @param[in] pathname Pathname of the settings file
    ///
    SettingsProfile(const QString& pathname);

    virtual ~SettingsProfile();

    /// Writes a boolean value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Boolean value for the key
    ///
    void writeBool(const QString& key, bool value) override;

    /// Writes an integer value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Integer value for the key
    ///
    void writeInt(const QString& key, int value) override;

    /// Writes an unsigned integer value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Unsigned integer value for the key
    ///
    void writeUInt(const QString& key, unsigned int value) override;

    /// Writes a double value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Double value for the key
    ///
    void writeDbl(const QString& key, double value) override;

    /// Writes a string value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value String value for the key
    ///
    void writeStr(const QString& key, const QString& value) override;

    /// Reads a boolean value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    ///
    bool readBool(const QString& key, bool defaultValue) override;

    /// Reads an integer value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    ///
    int readInt(const QString& key, int defaultValue) override;

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    ///
    unsigned int readUInt(const QString& key, unsigned int defaultValue) override;

    /// Reads a double value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    double readDbl(const QString& key, double defaultValue) override;

    /// Reads a string value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    QString readStr(const QString& key, const QString& defaultValue) override;

    /// Indicates whether the profile is being written at the user's request (i.e. a file profile).
    ///
    /// @return Always false because a registry profile is not written at the user's request.
    ///
    bool userInitiated() override;

    /// Returns the profile format version number.
    ///
    /// @return Profile format version number.
    ///
    int getVersion() override;

private:
    static constexpr int k_version = 1;

    QSettings* m_settings;
};
