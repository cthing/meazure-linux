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

#include "ConfigMgr.h"
#include "ExportedConfig.h"
#include "PersistentConfig.h"
#include <meazure/App.h>
#include <meazure/graphics/Colors.h>
#include <meazure/graphics/Dimensions.h>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>


ConfigMgr::ConfigMgr() : m_initialDir(QDir::homePath()) {
}

void ConfigMgr::exportConfig() {
    const QString& dir = m_exportPathname.isEmpty() ? m_initialDir : m_exportPathname;

    QString pathname = QFileDialog::getSaveFileName(nullptr, tr("Export Configuration"), dir, k_fileFilter);
    if (pathname.isEmpty()) {
        return;
    }

    if (!pathname.endsWith(k_fileSuffix)) {
        pathname.append(k_fileSuffix);
    }

    m_exportPathname = pathname;
    m_initialDir = QFileInfo(m_exportPathname).dir().path();

    try {
        ExportedConfig config(m_exportPathname, ExportedConfig::Mode::Write);
        writeConfig(config);
    } catch (const XMLWritingException& ex) {
        const QString msg = tr("There was an error while saving the configuration:\n%1\n\nError: %2")
                .arg(m_exportPathname).arg(ex.getMessage());
        QMessageBox::warning(nullptr, tr("Configuration Export Error"), msg);
    }
}

void ConfigMgr::importConfig() {
    const QString& dir = m_importPathname.isEmpty() ? m_initialDir : m_importPathname;

    const QString pathname = QFileDialog::getOpenFileName(nullptr, tr("Import Configuration"), dir, k_fileFilter);
    if (!pathname.isEmpty()) {
        import(pathname);
    }
}

void ConfigMgr::import(const QString& pathname) {
    if (!QFileInfo::exists(pathname)) {
        const QString msg = QObject::tr("Could not find file:\n%1").arg(m_importPathname);
        QMessageBox::warning(nullptr, QObject::tr("File Not Found\n"), msg);
        return;
    }

    m_importPathname = pathname;
    m_initialDir = QFileInfo(pathname).dir().path();

    try {
        const ExportedConfig config(m_importPathname, ExportedConfig::Mode::Read);
        readConfig(config);
    } catch (const XMLParsingException& ex) {
        const QString msg =
                QObject::tr("There was an error while importing the configuration:\n%1\n\nLine: %2\n"
                            "Character: %3\nError: %4")
                        .arg(pathname).arg(ex.getLine()).arg(ex.getColumn()).arg(ex.getMessage());
        QMessageBox::warning(nullptr, QObject::tr("Configuration Import Error"), msg);
    } catch (...) {
        const QString msg = QObject::tr("There was an error while importing the configuration:\n%1").arg(pathname);
        QMessageBox::warning(nullptr, QObject::tr("Configuration Import Error"), msg);
    }
}

void ConfigMgr::saveConfig() {
    PersistentConfig config;
    writeConfig(config);
}

void ConfigMgr::restoreConfig() {
    const PersistentConfig config;
    readConfig(config);
}

void ConfigMgr::writeConfig(Config& config) const {
    App::instance()->getMainWindow().writeConfig(config);
    App::instance()->getPosLogMgr().writeConfig(config);
    App::instance()->getToolMgr().writeConfig(config);
    App::instance()->getUnitsMgr().writeConfig(config);
    App::instance()->getScreenInfo().writeConfig(config);
    Colors::writeConfig(config);
    Dimensions::writeConfig(config);

    if (config.isPersistent()) {
        config.writeStr("LastConfigDir", m_initialDir);
    }
}

void ConfigMgr::readConfig(const Config& config) {
    Dimensions::readConfig(config);
    Colors::readConfig(config);
    App::instance()->getScreenInfo().readConfig(config);
    App::instance()->getUnitsMgr().readConfig(config);
    App::instance()->getToolMgr().readConfig(config);
    App::instance()->getPosLogMgr().readConfig(config);
    App::instance()->getMainWindow().readConfig(config);

    if (config.isPersistent()) {
        m_initialDir = config.readStr("LastConfigDir", m_initialDir);
    }
}
