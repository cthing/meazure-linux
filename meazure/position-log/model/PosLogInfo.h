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

#include <QString>
#include <QDateTime>


/// Information about a position log file.
///
class PosLogInfo {

public:
    [[nodiscard]] const QString &getTitle() const {
        return m_title;
    }

    void setTitle(const QString &title) {
        m_title = title;
    }

    [[nodiscard]] const QDateTime &getCreated() const {
        return m_created;
    }

    void setCreated(const QDateTime &created) {
        m_created = created;
    }

    [[nodiscard]] const QString &getAppName() const {
        return m_appName;
    }

    void setAppName(const QString &appName) {
        m_appName = appName;
    }

    [[nodiscard]] const QString &getAppVersion() const {
        return m_appVersion;
    }

    void setAppVersion(const QString &appVersion) {
        m_appVersion = appVersion;
    }

    [[nodiscard]] const QString &getAppBuild() const {
        return m_appBuild;
    }

    void setAppBuild(const QString &appBuild) {
        m_appBuild = appBuild;
    }

    [[nodiscard]] const QString &getMachineName() const {
        return m_machineName;
    }

    void setMachineName(const QString &machineName) {
        m_machineName = machineName;
    }

    [[nodiscard]] const QString &getDescription() const {
        return m_description;
    }

    void setDescription(const QString &description) {
        m_description = description;
    }

    bool operator==(const PosLogInfo &rhs) const {
        return m_title == rhs.m_title &&
               m_created == rhs.m_created &&
               m_appName == rhs.m_appName &&
               m_appVersion == rhs.m_appVersion &&
               m_appBuild == rhs.m_appBuild &&
               m_machineName == rhs.m_machineName &&
               m_description == rhs.m_description;
    }

    bool operator!=(const PosLogInfo &rhs) const {
        return !(rhs == *this);
    }

private:
    QString m_title;
    QDateTime m_created;
    QString m_appName;
    QString m_appVersion;
    QString m_appBuild;
    QString m_machineName;
    QString m_description;
};
