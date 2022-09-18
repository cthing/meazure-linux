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

#include <QString>


/// Base class for the persisting the application state to either QSettings or a file.
///
class Profile {

public:
    Profile() = default;
    virtual ~Profile() = default;

    Profile(const Profile&) = delete;
    Profile(Profile&&) = delete;
    Profile& operator=(const Profile&) = delete;

    /// Writes a boolean value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Boolean value for the key
    ///
    virtual void writeBool(const QString& key, bool value) = 0;

    /// Writes an integer value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Integer value for the key
    ///
    virtual void writeInt(const QString& key, int value) = 0;

    /// Writes an unsigned integer value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Integer value for the key
    ///
    virtual void writeUInt(const QString& key, unsigned int value) = 0;

    /// Writes a double value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value Double value for the key
    ///
    virtual void writeDbl(const QString& key, double value) = 0;

    /// Writes a string value to the specified key.
    ///
    /// @param[in] key Profile key to write
    /// @param[in] value String value for the key
    ///
    virtual void writeStr(const QString& key, const QString& value) = 0;

    /// Reads a boolean value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Boolean value for the key or the default value if the key is not found.
    ///
    virtual bool readBool(const QString& key, bool defaultValue) = 0;

    /// Reads an integer value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Integer value for the key or the default value if the key is not found.
    ///
    virtual int readInt(const QString& key, int defaultValue) = 0;

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Integer value for the key or the default value if the key is not found.
    ///
    virtual unsigned int readUInt(const QString& key, unsigned int defaultValue) = 0;

    /// Reads a double value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Double value for the key or the default value if the key is not found.
    ///
    virtual double readDbl(const QString& key, double defaultValue) = 0;

    /// Reads a string value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return String value for the key or the default value if the key is not found.
    ///
    virtual QString readStr(const QString& key, const QString& defaultValue) = 0;

    /// Indicates whether the profile is being written at the user's request (i.e. a file profile).
    ///
    /// @return true if the user initiated the profile writing.
    ///
    virtual bool userInitiated() = 0;

    /// Returns the profile format version number.
    ///
    /// @return Profile format version number.
    ///
    virtual int getVersion() = 0;
};
