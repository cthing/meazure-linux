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
#include <meazure/units/UnitsProvider.h>
#include <QObject>
#include <QString>
#include <iostream>


/// Manages the recording, saving and loading of measurement tool positions. The positions are saved to an XML
/// format file.
///
class PosLogMgr : public QObject {

    Q_OBJECT

public:
    void savePositions(std::ostream& out);

    void loadPositions(const QString& pathname);

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

    [[nodiscard]] bool isDirty() const {
        return m_dirty;
    }

signals:
    void positionsChanged(unsigned int numPositions);

public slots:
    void recordPosition();

    void deletePositions();

private:
    static constexpr int k_archiveMajorVersion { 1 };

    explicit PosLogMgr(const ToolMgr& toolMgr, const ScreenInfoProvider& screenInfo, const UnitsProvider& unitsMgr);

    [[nodiscard]] PosLogDesktopSharedPtr createDesktop();

    const ToolMgr& m_toolMgr;
    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_units;
    PosLogToolData m_currentToolData;
    PosLogDesktopWeakPtrVector m_desktopCache;
    PosLogPositionVector m_positions;
    QString m_title;
    QString m_description;
    bool m_dirty { false };

    friend class App;
};
