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

#include "model/PosLogToolData.h"
#include "model/PosLogPosition.h"
#include "model/PosLogDesktop.h"
#include <meazure/tools/ToolMgr.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsMgr.h>
#include <QObject>
#include <QString>
#include <QDateTime>


/// Manages the recording, saving and loading of measurement tool positions. The positions are saved to an XML
/// format file.
///
class PosLogMgr : public QObject {

    Q_OBJECT

public:
    [[nodiscard]] const QString& getTitle() const {
        return m_title;
    }

    void setTitle(const QString& title) {
        m_title = title;
    }

    [[nodiscard]] const QString& getDescription() const {
        return m_description;
    }

    void setDescription(const QString& description) {
        m_description = description;
    }

    [[nodiscard]] unsigned int getNumPositions() const {
        return m_positions.size();
    }

    [[nodiscard]] QDateTime getPositionRecorded(unsigned int positionIndex) const {
        return (positionIndex < m_positions.size()) ? m_positions[positionIndex].getRecorded() : QDateTime();
    }

    [[nodiscard]] QString getPositionDescription(unsigned int positionIndex) const {
        return (positionIndex < m_positions.size()) ? m_positions[positionIndex].getDescription() : QString();
    }

    void changePositionDescription(unsigned int positionIndex, const QString& description) {
        if (positionIndex < m_positions.size()) {
            m_positions[positionIndex].setDescription(description);
            markDirty();
        }
    }

    [[nodiscard]] bool isDirty() const {
        return m_dirty;
    }

    void refresh() {
        emit positionsChanged(m_positions.size());
    }

signals:
    void positionsLoaded();
    void positionsChanged(unsigned int numPositions);
    void positionAdded(unsigned int positionIndex);
    void dirtyChanged(bool dirty);

public slots:
    void changeTitle(const QString& title);
    void changeDescription(const QString& description);

    void addPosition();

    void insertPosition(unsigned int positionIndex);

    void deletePosition(unsigned int positionIndex);

    void deletePositions();

    bool savePositions();

    bool saveAsPositions();

    void loadPositions();

    void showPosition(unsigned int positionIndex);

private:
    static constexpr int k_archiveMajorVersion { 1 };
    static constexpr const char* k_fileFilter { "Meazure Position Log Files (*.mpl);;All Files (*.*)" };
    static constexpr const char* k_fileSuffix { ".mpl" };

    explicit PosLogMgr(ToolMgr& toolMgr, const ScreenInfoProvider& screenInfo, UnitsMgr& unitsMgr);

    bool save(const QString& pathname);

    void load(const QString& pathname);

    [[nodiscard]] PosLogDesktopSharedPtr createDesktop();

    void clearDirty() {
        m_dirty = false;
        emit dirtyChanged(m_dirty);
    }

    void markDirty() {
        m_dirty = true;
        emit dirtyChanged(m_dirty);
    }

    ToolMgr& m_toolMgr;
    const ScreenInfoProvider& m_screenInfo;
    UnitsMgr& m_units;
    PosLogToolData m_currentToolData;
    PosLogDesktopWeakPtrVector m_desktopCache;
    PosLogPositionVector m_positions;
    QString m_title;
    QString m_description;
    QString m_pathname;
    bool m_dirty { false };

    friend class App;
};
