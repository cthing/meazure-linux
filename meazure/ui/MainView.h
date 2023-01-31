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

#pragma once

#include "ToolDataSection.h"
#include "ScreenDataSection.h"
#include "MagnifierSection.h"
#include <meazure/prefs/ui/PrefsDialog.h>
#include <meazure/config/Config.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/tools/ToolMgr.h>
#include <QWidget>
#include <QAction>
#include <vector>


/// Contains the tool data, screen data and magnifier sections of the application.
///
class MainView : public QWidget {

    Q_OBJECT

public:
    explicit MainView(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr, const ToolMgr* toolMgr,
                      PrefsDialog* prefsDialog);

    [[nodiscard]] QAction* getMagnifierZoomInAction() const;
    [[nodiscard]] QAction* getMagnifierZoomOutAction() const;
    [[nodiscard]] std::vector<QAction*> getMagnifierGridActions() const;
    [[nodiscard]] QAction* getMagnifierFreezeAction() const;
    [[nodiscard]] std::vector<QAction*> getColorFormatActions() const;
    [[nodiscard]] QAction* getCopyColorAction() const;

    [[nodiscard]] ToolDataSection* getToolDataSection() const;
    [[nodiscard]] ScreenDataSection* getScreenDataSection() const;
    [[nodiscard]] MagnifierSection* getMagnifierSection() const;

    void writeConfig(Config& config) const;
    void readConfig(const Config& config);

    void hardReset();

private:
    ToolDataSection* m_toolDataSection;
    ScreenDataSection* m_screenDataSection;
    MagnifierSection* m_magnifierSection;
};
