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

#include "PosLogMgr.h"
#include "model/PosLogArchive.h"
#include "model/PosLogInfo.h"
#include "io/PosLogReader.h"
#include "io/PosLogWriter.h"
#include "AppVersion.h"
#include <QPoint>
#include <QSizeF>
#include <QRect>
#include <QDateTime>
#include <QHostInfo>
#include <QGuiApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <fstream>


PosLogMgr::PosLogMgr(const ToolMgr& toolMgr, const ScreenInfoProvider& screenInfo, const UnitsProvider& unitsMgr) :
        m_toolMgr(toolMgr),
        m_screenInfo(screenInfo),
        m_units(unitsMgr) {
    m_title = QString("%1 Position Log File").arg(QGuiApplication::applicationDisplayName());

    connect(&m_toolMgr, &ToolMgr::xy1PositionChanged, this, [this](const QPointF& coord) {
        m_currentToolData.setPoint1(coord);
    });
    connect(&m_toolMgr, &ToolMgr::xy2PositionChanged, this, [this](const QPointF& coord) {
        m_currentToolData.setPoint2(coord);
    });
    connect(&m_toolMgr, &ToolMgr::xyvPositionChanged, this, [this](const QPointF& coord) {
        m_currentToolData.setPointV(coord);
    });
    connect(&m_toolMgr, &ToolMgr::widthHeightChanged, this, [this](const QSizeF& widthHeight) {
        m_currentToolData.setWidthHeight(widthHeight);
    });
    connect(&m_toolMgr, &ToolMgr::distanceChanged, this, [this](double distance) {
        m_currentToolData.setDistance(distance);
    });
    connect(&m_toolMgr, &ToolMgr::angleChanged, this, [this](double angle) {
        m_currentToolData.setAngle(angle);
    });
    connect(&m_toolMgr, &ToolMgr::areaChanged, this, [this](double area) {
        m_currentToolData.setArea(area);
    });
}

void PosLogMgr::recordPosition() {
    PosLogPosition position;
    position.setToolName(m_toolMgr.getCurentRadioTool()->getName());
    position.setToolTraits(m_toolMgr.getCurentRadioTool()->getTraits());
    position.setToolData(m_currentToolData);
    position.setRecorded(QDateTime::currentDateTime().toUTC());

    PosLogDesktopSharedPtr desktop = createDesktop();       // NOLINT(misc-const-correctness)
    position.setDesktop(desktop);

    m_positions.push_back(position);
    m_dirty = true;

    m_toolMgr.strobeTool();

    emit positionsChanged(m_positions.size());
}

void PosLogMgr::deletePositions() {
    m_positions.clear();
    m_desktopCache.clear();

    m_dirty = false;

    emit positionsChanged(m_positions.size());
}

void PosLogMgr::savePositions() {
    if (m_pathname.isEmpty()) {
        saveAsPositions();
    } else {
        save(m_pathname);
    }
}

void PosLogMgr::saveAsPositions() {
    QString pathname = QFileDialog::getSaveFileName(nullptr, tr("Save Positions"), m_pathname, k_fileFilter);
    if (!pathname.isEmpty()) {
        if (!pathname.endsWith(k_fileSuffix)) {
            pathname.append(k_fileSuffix);
        }
        m_pathname = pathname;
        save(m_pathname);
    }
}

void PosLogMgr::save(const QString& pathname) {
    PosLogArchive archive;
    archive.setVersion(k_archiveMajorVersion);

    PosLogInfo info;
    info.setTitle(m_title);
    info.setDescription(m_description);
    info.setCreated(QDateTime::currentDateTime().toUTC());
    info.setAppName(QGuiApplication::applicationDisplayName());
    info.setAppVersion(appVersion);
    info.setAppBuild(appBuild);
    info.setMachineName(QHostInfo::localHostName());
    archive.setInfo(info);

    for (const PosLogDesktopWeakPtr& desktopWeakPtr : m_desktopCache) {
        if (!desktopWeakPtr.expired()) {
            archive.addDesktop(desktopWeakPtr.lock());
        }
    }

    archive.setPositions(m_positions);

    std::ofstream archiveStream;
    PosLogWriter logWriter(m_units);
    try {
        archiveStream.open(pathname.toUtf8().constData(), std::ios::out | std::ios::trunc);
        if (archiveStream.fail()) {
            const QString msg = QObject::tr("Could not open the position log file:\n%1\n\nError: %2")
                    .arg(pathname).arg(std::strerror(errno));       // NOLINT(concurrency-mt-unsafe)
            QMessageBox::warning(nullptr, tr("Position Log Save Error"), msg);
        }

        logWriter.write(archiveStream, archive);
        archiveStream.close();
        m_dirty = false;
    } catch (const XMLWritingException& ex) {
        archiveStream.close();
        const QString msg = QObject::tr("There was an error while saving the position log file:\n%1\n\nError: %2")
                .arg(pathname).arg(ex.getMessage());
        QMessageBox::warning(nullptr, tr("Position Log Save Error"), msg);
    }
}

void PosLogMgr::loadPositions() {
    const QString pathname = QFileDialog::getOpenFileName(nullptr, tr("Load Positions"), m_pathname, k_fileFilter);
    if (!pathname.isEmpty()) {
        m_pathname = pathname;
        load(m_pathname);
    }
}

void PosLogMgr::load(const QString& pathname) {
    PosLogReader logReader(m_units);
    bool success = false;

    PosLogArchiveSharedPtr archive;
    try {
        archive = logReader.readFile(pathname);
        success = true;
    } catch (const XMLParsingException& ex) {
        const QString msg =
                QObject::tr("There was an error while loading the position log file:\n"
                            "%1\n\nLine: %2\nCharacter: %3\nError: %4")
                .arg(ex.getPathname()).arg(ex.getLine()).arg(ex.getColumn()).arg(ex.getMessage());
        QMessageBox::warning(nullptr, tr("Position Log Load Error"), msg);
    } catch (...) {
        const QString msg = QObject::tr("There was an error while loading the position log file:\n%1").arg(pathname);
        QMessageBox::warning(nullptr, tr("Position Log Load Error"), msg);
    }

    if (!success) {
        return;
    }

    deletePositions();

    for (PosLogDesktopSharedPtr desktopSharePtr : archive->getDesktops()) {     // NOLINT(misc-const-correctness,performance-for-range-copy)
        const PosLogDesktopWeakPtr desktopWeakPtr(desktopSharePtr);
        m_desktopCache.push_back(desktopWeakPtr);
    }

    for (const PosLogPosition& position : archive->getPositions()) {
        m_positions.push_back(position);
    }

    if (!m_positions.empty()) {
        m_dirty = true;

        emit positionsChanged(m_positions.size());
    }
}

void PosLogMgr::managePositions() {

}

PosLogDesktopSharedPtr PosLogMgr::createDesktop() {
    PosLogDesktopSharedPtr desktop = std::make_shared<PosLogDesktop>();
    desktop->setOrigin(m_units.getOrigin());
    desktop->setInvertY(m_units.isInvertY());
    desktop->setLinearUnitsId(m_units.getLinearUnitsId());
    desktop->setAngularUnitsId(m_units.getAngularUnitsId());

    const QRect screenVirtualRect = m_screenInfo.getVirtualRect();
    const QSizeF size = m_units.getWidthHeight(screenVirtualRect.topLeft(), screenVirtualRect.bottomRight());
    desktop->setSize(size);

    for (int screenIndex = 0; screenIndex < m_screenInfo.getNumScreens(); screenIndex++) {
        PosLogScreen screen;
        screen.setPrimary(m_screenInfo.isPrimary(screenIndex));
        screen.setManualRes(m_screenInfo.isManualRes(screenIndex));
        screen.setRes(m_screenInfo.getScreenRes(screenIndex));
        screen.setDescription(m_screenInfo.getScreenName(screenIndex));

        const QRect& screenRect = m_screenInfo.getScreenRect(screenIndex);
        const QPointF p1 = m_units.convertCoord(screenRect.topLeft());
        const QPointF p2 = m_units.convertCoord(screenRect.bottomRight());
        screen.setRect(QRectF(p1, p2));

        desktop->addScreen(screen);
    }

    if (m_units.getLinearUnitsId() == CustomId) {
        const CustomUnits* customUnits = m_units.getCustomUnits();
        PosLogCustomUnits posLogCustomUnits;
        posLogCustomUnits.setName(customUnits->getName());
        posLogCustomUnits.setAbbrev(customUnits->getAbbrev());
        posLogCustomUnits.setScaleBasisStr(customUnits->getScaleBasisStr());
        posLogCustomUnits.setScaleFactor(customUnits->getScaleFactor());
        posLogCustomUnits.setDisplayPrecisions(customUnits->getDisplayPrecisions());
        desktop->setCustomUnits(posLogCustomUnits);
    }

    for (const PosLogDesktopWeakPtr& desktopWeakPtr : m_desktopCache) {
        if (!desktopWeakPtr.expired()) {
            PosLogDesktopSharedPtr cachedDesktop = desktopWeakPtr.lock();
            if (cachedDesktop->isSame(*desktop)) {
                return cachedDesktop;
            }
        }
    }

    const PosLogDesktopWeakPtr desktopWeakPtr(desktop);
    m_desktopCache.push_back(desktopWeakPtr);

    return desktop;
}
