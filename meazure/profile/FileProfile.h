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
#include <meazure/xml/XMLParser.h>
#include <meazure/xml/XMLWriter.h>
#include <QString>
#include <map>
#include <memory>
#include <fstream>


/// Persists the application state to an XML file.
///
class FileProfile : public Profile, public XMLParserHandler {

public:
    /// File access mode.
    enum Mode {
        ProfRead,       ///< Open profile file for reading.
        ProfWrite       ///< Open profile file for writing.
    };


    /// Creates an instance of a profile file object. The specified profile file is opened for either reading
    /// or writing.
    ///
    /// @param[in] pathname Pathname for the profile file. If the file is opened for writing, it is created if
    ///                     it does not exist.
    /// @param[in] mode Opening mode for the profile file (i.e. read or write).
    ///
    FileProfile(const QString& pathname, Mode mode);

    /// Closes the profile file and destroys the object instance.
    ///
    virtual ~FileProfile();

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
    /// @return Boolean value for the key or the default value if the key is not found.
    ///
    bool readBool(const QString& key, bool defaultValue) override;

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Integer value for the key or the default value if the key is not found.
    ///
    int readInt(const QString& key, int defaultValue) override;

    /// Reads an unsigned integer value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Unsigned integer value for the key or the default value if the key is not found.
    ///
    unsigned int readUInt(const QString& key, unsigned int defaultValue) override;

    /// Reads a double value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return Double value for the key or the default value if the key is not found.
    ///
    double readDbl(const QString& key, double defaultValue) override;

    /// Reads a string value from the specified key.
    ///
    /// @param[in] key Profile key to read
    /// @param[in] defaultValue Default value to use if the key is not found in the profile.
    /// @return String value for the key or the default value if the key is not found.
    ///
    QString readStr(const QString& key, const QString& defaultValue) override;

    /// Indicates whether the profile is being written at the user's request (i.e. a file profile).
    ///
    /// @return Always true because a file profile is only written at the user's request.
    ///
    bool userInitiated() override;

    /// Returns the profile format version number.
    ///
    /// @return Profile format version number.
    ///
    int getVersion() override;

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

    /// Obtains the pathname of the profile file represented by an instance of this class.
    ///
    /// @return Pathname of the profile file.
    ///
    QString getFilePathname() override;

private:
    /// Writes the XML boilerplate at the start of the XML profile file.
    ///
    void writeFileStart();

    /// Write the XML boilerplate at the end of the XML profile file.
    ///
    void writeFileEnd();

    /// Supervises the parsing of the XML profile file.
    ///
    /// @param[in] pathname Profile file to parse
    ///
    void parseFile(const QString& pathname);


    typedef std::unique_ptr<XMLWriter> XMLWriterPtr;


    static constexpr int k_version = 2;

    QString m_pathname;                         ///< Pathname of the file.
    std::ofstream m_writeStream;                ///< Output stream for the profile.
    XMLWriterPtr m_writer;                      ///< Writer for the profile.
    Mode m_mode;                                ///< Opening mode for the profile file.
    int m_readVersion;                          ///< Profile format version number read from the profile file.
    QString m_title;                            ///< Title for the profile file.
    std::map<QString, QString> m_valueMap;      ///< Maps profile keys to values.
};
