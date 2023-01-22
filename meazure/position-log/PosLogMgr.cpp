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
#include <meazure/tools/CursorTool.h>
#include <meazure/tools/WindowTool.h>
#include <meazure/tools/PointTool.h>
#include <meazure/tools/RectangleTool.h>
#include <QPoint>
#include <QSizeF>
#include <QRect>
#include <QHostInfo>
#include <QGuiApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <fstream>


PosLogMgr::PosLogMgr(ToolMgr& toolMgr, const ScreenInfoProvider& screenInfo, UnitsMgr& unitsMgr) :
        m_toolMgr(toolMgr),
        m_screenInfo(screenInfo),
        m_units(unitsMgr),
        m_initialDir(QDir::homePath()) {
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

void PosLogMgr::changeTitle(const QString& title) {
    m_title = title;
    markDirty();
}

void PosLogMgr::changeDescription(const QString& description) {
    m_description = description;
    markDirty();
}

void PosLogMgr::addPosition() {
    insertPosition(m_positions.size());
}

void PosLogMgr::insertPosition(unsigned int positionIndex) {
    PosLogPosition position;
    position.setToolName(m_toolMgr.getCurentRadioTool()->getName());
    position.setToolTraits(m_toolMgr.getCurentRadioTool()->getTraits());
    position.setToolData(m_currentToolData);
    position.setRecorded(QDateTime::currentDateTime().toUTC());

    PosLogDesktopSharedPtr desktop = createDesktop();       // NOLINT(misc-const-correctness)
    position.setDesktop(desktop);

    auto iter = (positionIndex >= m_positions.size()) ? m_positions.end() : (m_positions.begin() + positionIndex);
    m_positions.insert(iter, position);
    markDirty();

    m_toolMgr.strobeTool();

    emit positionsChanged(m_positions.size());
    emit positionAdded(positionIndex);
}

void PosLogMgr::deletePosition(unsigned int positionIndex) {
    if (positionIndex >= m_positions.size()) {
        return;
    }

    m_positions.erase(m_positions.begin() + positionIndex);

    if (m_positions.empty()) {
        clearDirty();
    } else {
        markDirty();
    }

    emit positionsChanged(m_positions.size());
}

void PosLogMgr::deletePositions() {
    m_positions.clear();
    m_desktopCache.clear();

    clearDirty();

    emit positionsChanged(m_positions.size());
}

bool PosLogMgr::savePositions() {
    return (m_savePathname.isEmpty() || !QFileInfo(m_savePathname).dir().exists())
            ? saveAsPositions()
            : save(m_savePathname);
}

bool PosLogMgr::saveAsPositions() {
    bool success = false;

    const QString& dir = m_savePathname.isEmpty() ? m_initialDir : m_savePathname;

    QString pathname = QFileDialog::getSaveFileName(nullptr, tr("Save Positions"), dir, k_fileFilter);
    if (pathname.isEmpty()) {
        success = false;
    } else {
        if (!pathname.endsWith(k_fileSuffix)) {
            pathname.append(k_fileSuffix);
        }
        m_savePathname = pathname;
        m_initialDir = QFileInfo(m_savePathname).dir().path();
        success = save(m_savePathname);
    }

    return success;
}

bool PosLogMgr::save(const QString& pathname) {
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

    bool success = false;
    std::ofstream archiveStream;
    PosLogWriter logWriter(m_units);
    try {
        archiveStream.open(pathname.toUtf8().constData(), std::ios::out | std::ios::trunc);
        if (archiveStream.fail()) {
            const QString msg = tr("Could not open the position log file:\n%1\n\nError: %2")
                    .arg(pathname).arg(std::strerror(errno));       // NOLINT(concurrency-mt-unsafe)
            QMessageBox::warning(nullptr, tr("Position Log Save Error"), msg);
        }

        logWriter.write(archiveStream, archive);
        archiveStream.close();
        clearDirty();
        success = true;
    } catch (const XMLWritingException& ex) {
        archiveStream.close();
        const QString msg = tr("There was an error while saving the position log file:\n%1\n\nError: %2")
                .arg(pathname).arg(ex.getMessage());
        QMessageBox::warning(nullptr, tr("Position Log Save Error"), msg);
        success = false;
    }

    return success;
}

void PosLogMgr::loadPositions() {
    if (m_dirty) {
        const QMessageBox::StandardButton response =
                QMessageBox::question(nullptr, tr("Save Positions"),
                                      tr("Positions have been recorded but have not yet been saved.\n\n"
                                         "Save these positions?"),
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                      QMessageBox::Yes);
        if (response == QMessageBox::Yes) {
            if (!savePositions()) {
                return;
            }
        } else if (response == QMessageBox::Cancel) {
            return;
        }
    }

    const QString& dir = m_loadPathname.isEmpty() ? m_initialDir : m_loadPathname;

    const QString pathname = QFileDialog::getOpenFileName(nullptr, tr("Load Positions"), dir, k_fileFilter);
    if (!pathname.isEmpty()) {
        load(pathname);
    }
}

void PosLogMgr::load(const QString& pathname) {
    if (!QFileInfo::exists(pathname)) {
        const QString msg = tr("Could not find file:\n%1").arg(pathname);
        QMessageBox::warning(nullptr, tr("File Not Found\n"), msg);
        return;
    }

    m_loadPathname = pathname;
    m_initialDir = QFileInfo(m_loadPathname).dir().path();

    PosLogReader logReader(m_units);
    bool success = false;

    PosLogArchiveSharedPtr archive;
    try {
        archive = logReader.readFile(pathname);
        success = true;
    } catch (const XMLParsingException& ex) {
        const QString msg =
                tr("There was an error while loading the position log file:\n"
                   "%1\n\nLine: %2\nCharacter: %3\nError: %4")
                .arg(ex.getPathname()).arg(ex.getLine()).arg(ex.getColumn()).arg(ex.getMessage());
        QMessageBox::warning(nullptr, tr("Position Log Load Error"), msg);
    } catch (...) {
        const QString msg = tr("There was an error while loading the position log file:\n%1").arg(pathname);
        QMessageBox::warning(nullptr, tr("Position Log Load Error"), msg);
    }

    if (!success) {
        return;
    }

    deletePositions();

    m_title = archive->getInfo().getTitle();
    m_description = archive->getInfo().getDescription();

    for (PosLogDesktopSharedPtr desktopSharePtr : archive->getDesktops()) {     // NOLINT(misc-const-correctness,performance-for-range-copy)
        const PosLogDesktopWeakPtr desktopWeakPtr(desktopSharePtr);
        m_desktopCache.push_back(desktopWeakPtr);
    }

    for (const PosLogPosition& position : archive->getPositions()) {
        m_positions.push_back(position);
    }

    if (!m_positions.empty()) {
        emit positionsChanged(m_positions.size());
    }

    emit positionsLoaded();
}

void PosLogMgr::writeConfig(Config& config) const {
    if (config.isPersistent()) {
        config.writeStr("LastLogDir", m_initialDir);
    }

}

void PosLogMgr::readConfig(const Config& config) {
    if (config.isPersistent()) {
        m_initialDir = config.readStr("LastLogDir", m_initialDir);
    }
}

void PosLogMgr::showPosition(unsigned int positionIndex) {
    if (positionIndex >= m_positions.size()) {
        return;
    }

    const PosLogPosition& position = m_positions[positionIndex];
    const PosLogDesktopSharedPtr desktop = position.getDesktop();

    // Change the units if needed. If these are custom units perform additional configuration.

    const LinearUnitsId linearUnitsId = desktop->getLinearUnitsId();
    if (linearUnitsId != m_units.getLinearUnitsId()) {
        if (linearUnitsId == CustomId) {
            const PosLogCustomUnits& logCustomUnits = desktop->getCustomUnits();
            CustomUnits* customUnits = m_units.getCustomUnits();

            customUnits->setName(logCustomUnits.getName());
            customUnits->setAbbrev(logCustomUnits.getAbbrev());
            customUnits->setScaleBasis(logCustomUnits.getScaleBasisStr());
            customUnits->setScaleFactor(logCustomUnits.getScaleFactor());
            customUnits->setDisplayPrecisions(logCustomUnits.getDisplayPrecisions());
        }

        m_units.setLinearUnits(desktop->getLinearUnitsId());
    }

    const AngularUnitsId angularUnitsId = desktop->getAngularUnitsId();
    if (angularUnitsId != m_units.getAngularUnitsId()) {
        m_units.setAngularUnits(desktop->getAngularUnitsId());
    }

    // Set the origin and y-axis orientation.

    const bool invertY = desktop->isInvertY();
    if (invertY != m_units.isInvertY()) {
        m_units.setInvertY(invertY);
    }

    const QPoint origin = m_units.unconvertPos(desktop->getOrigin());
    if (origin != m_units.getOrigin()) {
        m_units.setOrigin(origin);
    }

    // Change the radio tool, if needed. If the position used the cursor tool, it is displayed using the
    // point tool so that the cursor is not pulled out from under the user. If the position used the window
    // tool, it is displayed using the rectangle tool.

    QString toolName = position.getToolName();
    if (toolName == CursorTool::k_toolName) {
        toolName = PointTool::k_toolName;
    } else if (toolName == WindowTool::k_toolName) {
        toolName = RectangleTool::k_toolName;
    }

    if (toolName != m_toolMgr.getCurentRadioTool()->getName()) {
        m_toolMgr.selectRadioTool(toolName.toUtf8().constData());
    }

    // Show the position.

    const RadioToolTraits traits = position.getToolTraits();
    const PosLogToolData& toolData = position.getToolData();

    if ((traits & RadioToolTrait::XY1Available) != 0) {
        m_toolMgr.setXY1Position(toolData.getPoint1());
    }
    if ((traits & RadioToolTrait::XY2Available) != 0) {
        m_toolMgr.setXY2Position(toolData.getPoint2());
    }
    if ((traits & RadioToolTrait::XYVAvailable) != 0) {
        m_toolMgr.setXYVPosition(toolData.getPointV());
    }
}

PosLogDesktopSharedPtr PosLogMgr::createDesktop() {
    PosLogDesktopSharedPtr desktop = std::make_shared<PosLogDesktop>();
    desktop->setOrigin(m_units.convertPos(m_units.getOrigin()));
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
