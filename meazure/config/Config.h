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

#include <QString>


/// Base class for persistent and exported application state. A configuration contains all aspects of the application
/// state (e.g. preference settings, tool locations, the currently selected tools, calibration information).
/// The configuration is persisted between each run of the application and can be exported by the user.
///
struct Config {

    Config() = default;
    virtual ~Config() = default;

    Config(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(const Config&) = delete;

    /// Writes a boolean value to the specified key.
    ///
    /// @param[in] key Configuration key to write
    /// @param[in] value Boolean value for the key
    ///
    virtual void writeBool(const QString& key, bool value) = 0;

    /// Writes an integer value to the specified key.
    ///
    /// @param[in] key Configuration key to write
    /// @param[in] value Integer value for the key
    ///
    virtual void writeInt(const QString& key, int value) = 0;

    /// Writes an unsigned integer value to the specified key.
    ///
    /// @param[in] key Configuration key to write
    /// @param[in] value Integer value for the key
    ///
    virtual void writeUInt(const QString& key, unsigned int value) = 0;

    /// Writes a double value to the specified key.
    ///
    /// @param[in] key Configuration key to write
    /// @param[in] value Double value for the key
    ///
    virtual void writeDbl(const QString& key, double value) = 0;

    /// Writes a string value to the specified key.
    ///
    /// @param[in] key Configuration key to write
    /// @param[in] value String value for the key
    ///
    virtual void writeStr(const QString& key, const QString& value) = 0;

    /// Reads a boolean value from the specified key.
    ///
    /// @param[in] key Configuration key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the configuration.
    /// @return Boolean value for the key or the default value if the key is not found.
    ///
    [[nodiscard]] virtual bool readBool(const QString& key, bool defaultValue) const = 0;

    /// Reads an integer value from the specified key.
    ///
    /// @param[in] key Configuration key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the configuration.
    /// @return Integer value for the key or the default value if the key is not found.
    ///
    [[nodiscard]] virtual int readInt(const QString& key, int defaultValue) const = 0;

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param[in] key Configuration key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the configuration.
    /// @return Integer value for the key or the default value if the key is not found.
    ///
    [[nodiscard]] virtual unsigned int readUInt(const QString& key, unsigned int defaultValue) const = 0;

    /// Reads a double value from the specified key.
    ///
    /// @param[in] key Configuration key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the configuration.
    /// @return Double value for the key or the default value if the key is not found.
    ///
    [[nodiscard]] virtual double readDbl(const QString& key, double defaultValue) const = 0;

    /// Reads a string value from the specified key.
    ///
    /// @param[in] key Configuration key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the configuration.
    /// @return String value for the key or the default value if the key is not found.
    ///
    [[nodiscard]] virtual QString readStr(const QString& key, const QString& defaultValue) const = 0;

    /// Indicates whether the configuration is being persisted.
    ///
    /// @return true if the configuration is persistent. false if the configuration is exported.
    ///
    [[nodiscard]] virtual bool isPersistent() const = 0;

    /// Returns the configuration format version number.
    ///
    /// @return Configuration format version number.
    ///
    [[nodiscard]] virtual int getVersion() const = 0;

protected:
    // Element names
    static constexpr const char* k_createdElem { "created" };
    static constexpr const char* k_dataElem { "data" };
    static constexpr const char* k_generatorElem { "generator" };
    static constexpr const char* k_infoElem { "info" };
    static constexpr const char* k_machineElem { "machine" };
    static constexpr const char* k_profileElem { "profile" };
    static constexpr const char* k_titleElem { "title" };

    // Attribute names
    static constexpr const char* k_buildAttr { "build" };
    static constexpr const char* k_dateAttr { "date" };
    static constexpr const char* k_nameAttr { "name" };
    static constexpr const char* k_valueAttr { "value" };
    static constexpr const char* k_versionAttr { "version" };
};
