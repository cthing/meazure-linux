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

#include "ExportedConfig.h"
#include "AppVersion.h"
#include <QDateTime>
#include <QHostInfo>
#include <utility>


ExportedConfig::ExportedConfig(QString pathname, Mode mode) :
        m_pathname(std::move(pathname)),
        m_mode(mode),
        m_title("Meazure Configuration File") {

    if (m_mode == Write) {
        m_writeStream.open(m_pathname.toStdString(), std::ios::out | std::ios::trunc);
        m_writer = std::make_unique<XMLWriter>(m_writeStream);

        writeFileStart();
    } else {
        parseFile();
    }
}

ExportedConfig::~ExportedConfig() {
    if (m_mode == Write) {
        try {
            writeFileEnd();
        } catch (...) {
            assert(false);
        }
    }
}

void ExportedConfig::writeBool(const QString& key, bool value) {
    m_writer->startElement(key)
            .addAttribute(k_valueAttr, (value ? "true" : "false"))
            .endElement();
}

void ExportedConfig::writeInt(const QString& key, int value) {
    m_writer->startElement(key)
            .addAttribute(k_valueAttr, value)
            .endElement();
}

void ExportedConfig::writeUInt(const QString& key, unsigned int value) {
    m_writer->startElement(key)
            .addAttribute(k_valueAttr, value)
            .endElement();
}

void ExportedConfig::writeDbl(const QString& key, double value) {
    m_writer->startElement(key)
            .addAttribute(k_valueAttr, value)
            .endElement();
}

void ExportedConfig::writeStr(const QString& key, const QString& value) {
    m_writer->startElement(key)
            .addAttribute(k_valueAttr, value)
            .endElement();
}

bool ExportedConfig::readBool(const QString& key, bool defaultValue) const {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    const QString val = (*iter).second.toLower();
    return ((val == "true") || (val == "1") || (val == "yes"));
}

int ExportedConfig::readInt(const QString& key, int defaultValue) const {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    bool success = false;
    const int value = (*iter).second.toInt(&success);
    return success ? value : defaultValue;
}

unsigned int ExportedConfig::readUInt(const QString& key, unsigned int defaultValue) const {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    bool success = false;
    const unsigned int value = (*iter).second.toUInt(&success);
    return success ? value : defaultValue;
}

double ExportedConfig::readDbl(const QString& key, double defaultValue) const {
    auto iter = m_valueMap.find(key);
    if (iter == m_valueMap.end()) {
        return defaultValue;
    }

    bool success = false;
    const double value = (*iter).second.toDouble(&success);
    return success ? value : defaultValue;
}

QString ExportedConfig::readStr(const QString& key, const QString& defaultValue) const {
    auto iter = m_valueMap.find(key);
    return (iter == m_valueMap.end()) ? defaultValue : (*iter).second;
}

bool ExportedConfig::isPersistent() const {
    return false;
}

int ExportedConfig::getVersion() const {
    return m_readVersion;
}

void ExportedConfig::writeFileStart() {
    m_writer->startDocument();

    // The root element is named "profile" rather than "config" for backward compatibility. In older versions of
    // Meazure, exported configuration files were called profiles.
    m_writer->startElement(k_profileElem)
            .addAttribute(k_versionAttr, k_version);

    m_writer->startElement(k_infoElem);

    m_writer->startElement(k_titleElem)
            .characters(m_title)
            .endElement();

    m_writer->startElement(k_createdElem)
            .addAttribute(k_dateAttr, QDateTime::currentDateTime().toString(Qt::ISODate))
            .endElement();

    m_writer->startElement(k_generatorElem)
            .addAttribute(k_nameAttr, "Meazure")
            .addAttribute(k_versionAttr, appVersion)
            .addAttribute(k_buildAttr, appBuild)
            .endElement();

    m_writer->startElement(k_machineElem)
            .addAttribute(k_nameAttr, QHostInfo::localHostName())
            .endElement();

    m_writer->endElement();         // info

    m_writer->startElement(k_dataElem);
}

void ExportedConfig::writeFileEnd() {
    m_writer->endElement();         // data
    m_writer->endElement();         // profile
    m_writer->endDocument();
}

void ExportedConfig::parseFile() {
    XMLParser parser(this);
    parser.parseFile(m_pathname);
}

void ExportedConfig::startElement(const QString& container, const QString& elementName, const XMLAttributes& attrs) {
    if (elementName == k_profileElem) {
        int value = 0;
        attrs.getValueInt(k_versionAttr, value);
        m_readVersion = value;
    } else if ((container == k_dataElem) || (m_readVersion == 1)) {
        QString value;
        attrs.getValueStr(k_valueAttr, value);
        m_valueMap[elementName] = value;
    }
}

void ExportedConfig::characterData(const QString& container, const QString& data) {
    if (container == k_titleElem) {
        m_title += data;
    }
}

QString ExportedConfig::getFilePathname() {
    return m_pathname;
}
