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

#include "Config.h"
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include <QString>
#include <map>
#include <memory>
#include <fstream>


/// Represents an exported application configuration file. An exported configuration is an XML file containing
/// most aspects of the application state (e.g. preference settings, tool locations, the currently selected tools,
/// calibration information). To keep the exported configuration platform-independent, certain items are not
/// exported (e.g. window position, file dialog directories).
///
class ExportedConfig : public Config, public XMLParserHandler {

public:
    /// Access mode.
    enum Mode {
        Read,        ///< Open the configuration file for reading.
        Write        ///< Open the configuration file for writing.
    };


    /// Creates an instance of a configuration object. The specified configuration file is opened for either reading
    /// or writing.
    ///
    /// @param[in] pathname Pathname for the configuration file. If the file is opened for writing, it is created if
    ///                     it does not exist.
    /// @param[in] mode Opening mode for the configuration file (i.e. read or write).
    ///
    ExportedConfig(QString pathname, Mode mode);

    /// Closes the configuration file and destroys the object instance.
    ///
    ~ExportedConfig() override;

    ExportedConfig(const ExportedConfig&) = delete;
    ExportedConfig(ExportedConfig&&) = delete;
    ExportedConfig& operator=(const ExportedConfig&) = delete;

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

    /// Called when the XML parser encounters the start of an element.
    ///
    /// @param[in] container Name of the element containing this element.
    /// @param[in] elementName Name of the element.
    /// @param[in] attrs Attributes for the element.
    ///
    void startElement(const QString& container, const QString& elementName, const XMLAttributes& attrs) override;

    /// Called when the XML parser encounters character data.
    ///
    /// @param[in] container Name of the element containing the character data.
    /// @param[in] data Character data.
    ///
    void characterData(const QString& container, const QString& data) override;

    /// Obtains the pathname of the configuration file represented by an instance of this class.
    ///
    /// @return Pathname of the configuration file.
    ///
    QString getFilePathname() override;

private:
    /// Writes the XML boilerplate at the start of the XML configuration file.
    ///
    void writeFileStart();

    /// Write the XML boilerplate at the end of the XML configuration file.
    ///
    void writeFileEnd();

    /// Supervises the parsing of the XML configuration file.
    ///
    void parseFile();


    using XMLWriterPtr = std::unique_ptr<XMLWriter>;

    static constexpr int k_version { 2 };

    QString m_pathname;                         ///< Pathname of the file.
    std::ofstream m_writeStream;                ///< Output stream for the configuration file.
    XMLWriterPtr m_writer;                      ///< Writer for the configuration file.
    Mode m_mode;                                ///< Opening mode for the configuration file.
    int m_readVersion { k_version };            ///< Configuration format version number read from the file.
    QString m_title;                            ///< Title for the configuration file.
    std::map<QString, QString> m_valueMap;      ///< Maps configuration keys to values.
};
