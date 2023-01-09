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
    void recordPosition();

    void savePositions(std::ostream& out);

private:
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

    friend class App;
};
