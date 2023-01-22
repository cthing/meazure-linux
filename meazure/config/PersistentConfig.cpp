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

#include "PersistentConfig.h"


PersistentConfig::PersistentConfig() : m_settings(new QSettings(k_orgKey, k_appKey)) {
    m_settings->beginGroup(QString::number(k_version));
}

PersistentConfig::PersistentConfig(const QString& pathname) :
        m_settings(new QSettings(pathname, QSettings::Format::IniFormat))  {
    m_settings->beginGroup(QString::number(k_version));
}

PersistentConfig::~PersistentConfig() {
    m_settings->endGroup();
    delete m_settings;
}

void PersistentConfig::writeBool(const QString &key, bool value) {
    m_settings->setValue(key, value);
}

void PersistentConfig::writeInt(const QString& key, int value) {
    m_settings->setValue(key, value);
}

void PersistentConfig::writeUInt(const QString& key, unsigned int value) {
    m_settings->setValue(key, value);
}

void PersistentConfig::writeDbl(const QString& key, double value) {
    m_settings->setValue(key, value);
}

void PersistentConfig::writeStr(const QString& key, const QString& value) {
    m_settings->setValue(key, value);
}

bool PersistentConfig::readBool(const QString& key, bool defaultValue) const {
    return m_settings->value(key, defaultValue).toBool();
}

int PersistentConfig::readInt(const QString& key, int defaultValue) const {
    bool success = false;
    const int value = m_settings->value(key, defaultValue).toInt(&success);
    return success ? value : defaultValue;
}

unsigned int PersistentConfig::readUInt(const QString& key, unsigned int defaultValue) const {
    bool success = false;
    const unsigned int value = m_settings->value(key, defaultValue).toUInt(&success);
    return success ? value : defaultValue;
}

double PersistentConfig::readDbl(const QString& key, double defaultValue) const {
    bool success = false;
    const double value = m_settings->value(key, defaultValue).toDouble(&success);
    return success ? value : defaultValue;
}

QString PersistentConfig::readStr(const QString& key, const QString& defaultValue) const {
    return m_settings->value(key, defaultValue).toString();
}

bool PersistentConfig::isPersistent() const {
    return true;
}

int PersistentConfig::getVersion() const {
    return k_version;
}
