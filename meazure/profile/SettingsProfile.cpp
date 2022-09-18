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

#include "SettingsProfile.h"


SettingsProfile::SettingsProfile() : m_settings(new QSettings()) {
    m_settings->beginGroup(QString::number(version));
}

SettingsProfile::SettingsProfile(const QString& pathname) :
        m_settings(new QSettings(pathname, QSettings::Format::IniFormat)) {
    m_settings->beginGroup(QString::number(version));
}

SettingsProfile::~SettingsProfile() {
    m_settings->endGroup();
    delete m_settings;
}

void SettingsProfile::writeBool(const QString &key, bool value) {
    m_settings->setValue(key, value);
}

void SettingsProfile::writeInt(const QString& key, int value) {
    m_settings->setValue(key, value);
}

void SettingsProfile::writeUInt(const QString& key, unsigned int value) {
    m_settings->setValue(key, value);
}

void SettingsProfile::writeDbl(const QString& key, double value) {
    m_settings->setValue(key, value);
}

void SettingsProfile::writeStr(const QString& key, const QString& value) {
    m_settings->setValue(key, value);
}

bool SettingsProfile::readBool(const QString& key, bool defaultValue) {
    return m_settings->value(key, defaultValue).toBool();
}

int SettingsProfile::readInt(const QString& key, int defaultValue) {
    bool success = false;
    const int value = m_settings->value(key, defaultValue).toInt(&success);
    return success ? value : defaultValue;
}

unsigned int SettingsProfile::readUInt(const QString& key, unsigned int defaultValue) {
    bool success = false;
    const unsigned int value = m_settings->value(key, defaultValue).toUInt(&success);
    return success ? value : defaultValue;
}

double SettingsProfile::readDbl(const QString& key, double defaultValue) {
    bool success = false;
    const double value = m_settings->value(key, defaultValue).toDouble(&success);
    return success ? value : defaultValue;
}

QString SettingsProfile::readStr(const QString& key, const QString& defaultValue) {
    return m_settings->value(key, defaultValue).toString();
}

bool SettingsProfile::userInitiated() {
    return false;
}

int SettingsProfile::getVersion() {
    return version;
}
