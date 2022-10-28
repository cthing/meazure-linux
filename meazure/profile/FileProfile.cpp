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

#include "FileProfile.h"
#include "AppVersion.h"
#include <QDateTime>
#include <QHostInfo>


FileProfile::FileProfile(const QString& pathname, Mode mode) :
        m_pathname(pathname),
        m_mode(mode),
        m_title("Meazure Profile File") {

    if (m_mode == ProfWrite) {
        m_writeStream.exceptions(std::ios::failbit | std::ios::badbit);
        m_writeStream.open(m_pathname.toStdString(), std::ios::out | std::ios::trunc);
        m_writer = std::make_unique<XMLWriter>(m_writeStream);

        writeFileStart();
    } else {
        parseFile(pathname);
    }
}

FileProfile::~FileProfile() {
    if (m_mode == ProfWrite) {
        try {
            writeFileEnd();
        } catch (...) {
            assert(false);
        }
    }
}

void FileProfile::writeBool(const QString& key, bool value) {
    m_writer->startElement(key)
            .addAttribute("value", (value ? "true" : "false"))
            .endElement();
}

void FileProfile::writeInt(const QString& key, int value) {
    m_writer->startElement(key)
            .addAttribute("value", value)
            .endElement();
}

void FileProfile::writeUInt(const QString& key, unsigned int value) {
    m_writer->startElement(key)
            .addAttribute("value", value)
            .endElement();
}

void FileProfile::writeDbl(const QString& key, double value) {
    m_writer->startElement(key)
            .addAttribute("value", value)
            .endElement();
}

void FileProfile::writeStr(const QString& key, const QString& value) {
    m_writer->startElement(key)
            .addAttribute("value", value)
            .endElement();
}

bool FileProfile::readBool(const QString& key, bool defaultValue) {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    const QString val = (*iter).second.toLower();
    return ((val == "true") || (val == "1") || (val == "yes"));
}

int FileProfile::readInt(const QString& key, int defaultValue) {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    bool success = false;
    const int value = (*iter).second.toInt(&success);
    return success ? value : defaultValue;
}

unsigned int FileProfile::readUInt(const QString& key, unsigned int defaultValue) {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    bool success = false;
    const unsigned int value = (*iter).second.toUInt(&success);
    return success ? value : defaultValue;
}

double FileProfile::readDbl(const QString& key, double defaultValue) {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    bool success = false;
    const double value = (*iter).second.toDouble(&success);
    return success ? value : defaultValue;
}

QString FileProfile::readStr(const QString& key, const QString& defaultValue) {
    auto iter = m_valueMap.find(key);
    return (iter == m_valueMap.end()) ? defaultValue : (*iter).second;
}

bool FileProfile::userInitiated() {
    return true;
}

int FileProfile::getVersion() {
    return m_readVersion;
}

void FileProfile::writeFileStart() {
    m_writer->startDocument();

    m_writer->startElement("profile")
            .addAttribute("version", k_version);

    m_writer->startElement("info");

    m_writer->startElement("title")
            .characters(m_title)
            .endElement();

    m_writer->startElement("created")
            .addAttribute("date", QDateTime::currentDateTime().toString(Qt::ISODate))
            .endElement();

    m_writer->startElement("generator")
            .addAttribute("name", "Meazure")
            .addAttribute("version", appVersion)
            .addAttribute("build", appBuild)
            .endElement();

    m_writer->startElement("machine")
            .addAttribute("name", QHostInfo::localHostName())
            .endElement();

    m_writer->endElement();         // info

    m_writer->startElement("data");
}

void FileProfile::writeFileEnd() {
    m_writer->endElement();         // data
    m_writer->endElement();         // profile
    m_writer->endDocument();
}

void FileProfile::parseFile(const QString& pathname) {
    XMLParser parser(this);
    parser.parseFile(pathname);
}

void FileProfile::startElement(const QString& container, const QString& elementName, const XMLAttributes& attrs) {
    if (elementName == "profile") {
        int value = 0;
        attrs.getValueInt("version", value);
        m_readVersion = value;
    } else if ((container == "data") || (m_readVersion == 1)) {
        QString value;
        attrs.getValueStr("value", value);
        m_valueMap[elementName] = value;
    }
}

void FileProfile::characterData(const QString& container, const QString& data) {
    if (container == "title") {
        m_title += data;
    }
}

QString FileProfile::getFilePathname() {
    return m_pathname;
}
