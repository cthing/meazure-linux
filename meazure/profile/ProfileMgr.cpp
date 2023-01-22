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

#include "ProfileMgr.h"
#include "FileProfile.h"
#include <meazure/App.h>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>


ProfileMgr::ProfileMgr() : m_initialDir(QDir::homePath()) {
}

void ProfileMgr::saveProfile() {
    (m_savePathname.isEmpty() || !QFileInfo(m_savePathname).dir().exists()) ? saveAsProfile() : save(m_savePathname);
}

void ProfileMgr::saveAsProfile() {
    const QString& dir = m_savePathname.isEmpty() ? m_initialDir : m_savePathname;

    QString pathname = QFileDialog::getSaveFileName(nullptr, tr("Save Profile"), dir, k_fileFilter);
    if (pathname.isEmpty()) {
        return;
    }

    if (!pathname.endsWith(k_fileSuffix)) {
        pathname.append(k_fileSuffix);
    }

    m_savePathname = pathname;
    m_initialDir = QFileInfo(m_savePathname).dir().path();

    save(m_savePathname);
}

void ProfileMgr::save(const QString& pathname) {
    try {
        FileProfile profile(pathname, FileProfile::Mode::ProfWrite);
        saveToProfile(profile);
    } catch (const XMLWritingException& ex) {
        const QString msg = tr("There was an error while saving the profile:\n%1\n\nError: %2")
                .arg(pathname).arg(ex.getMessage());
        QMessageBox::warning(nullptr, tr("Profile Save Error"), msg);
    }
}

void ProfileMgr::loadProfile() {
    const QString& dir = m_loadPathname.isEmpty() ? m_initialDir : m_loadPathname;

    const QString pathname = QFileDialog::getOpenFileName(nullptr, tr("Load Profile"), dir, k_fileFilter);
    if (!pathname.isEmpty()) {
        load(pathname);
    }
}

void ProfileMgr::load(const QString& pathname) {
    if (!QFileInfo::exists(pathname)) {
        const QString msg = QObject::tr("Could not find file:\n%1").arg(m_loadPathname);
        QMessageBox::warning(nullptr, QObject::tr("File Not Found\n"), msg);
        return;
    }

    m_loadPathname = pathname;
    m_initialDir = QFileInfo(pathname).dir().path();

    try {
        FileProfile profile(pathname, FileProfile::Mode::ProfRead);
        loadFromProfile(profile);
    } catch (const XMLParsingException& ex) {
        const QString msg =
                QObject::tr("There was an error while loading the profile:\n%1\n\nLine: %2\nCharacter: %3\nError: %4")
                        .arg(pathname).arg(ex.getLine()).arg(ex.getColumn()).arg(ex.getMessage());
        QMessageBox::warning(nullptr, QObject::tr("Profile Load Error"), msg);
    } catch (...) {
        const QString msg = QObject::tr("There was an error while loading the profile:\n%1").arg(pathname);
        QMessageBox::warning(nullptr, QObject::tr("Profile Load Error"), msg);
    }
}

void ProfileMgr::saveSettings() {
}

void ProfileMgr::loadSettings() {
}

void ProfileMgr::saveToProfile(Profile& profile) {
    App::instance()->getMainWindow().saveToProfile(profile);
    App::instance()->getPosLogMgr().saveToProfile(profile);
    App::instance()->getToolMgr().saveToProfile(profile);
    App::instance()->getUnitsMgr().saveToProfile(profile);
    App::instance()->getScreenInfo().saveToProfile(profile);
    Colors::saveToProfile(profile);

    if (!profile.userInitiated()) {
        profile.writeStr("LastProfileDir", m_initialDir);
    }
}

void ProfileMgr::loadFromProfile(Profile& profile) {
    Colors::loadFromProfile(profile);
    App::instance()->getScreenInfo().loadFromProfile(profile);
    App::instance()->getUnitsMgr().loadFromProfile(profile);
    App::instance()->getToolMgr().loadFromProfile(profile);
    App::instance()->getPosLogMgr().loadFromProfile(profile);
    App::instance()->getMainWindow().loadFromProfile(profile);

    if (!profile.userInitiated()) {
        m_initialDir = profile.readStr("LastProfileDir", m_initialDir);
    }
}
