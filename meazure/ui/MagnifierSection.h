/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "Magnifier.h"
#include "MagnifierControls.h"
#include "ColorDisplay.h"
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/tools/ToolMgr.h>
#include <meazure/config/Config.h>
#include <QWidget>
#include <QAction>
#include <vector>


/// Provides the screen magnifier, pixel color display and zoom control.
///
class MagnifierSection : public QWidget {

    Q_OBJECT

public:
    MagnifierSection(const ScreenInfoProvider* screenInfo, const ToolMgr* toolMgr);

    void writeConfig(Config& config) const;
    void readConfig(const Config& config);

    void hardReset();

    [[nodiscard]] QAction* getZoomInAction() const {
        return m_zoomInAction;
    }

    [[nodiscard]] QAction* getZoomOutAction() const {
        return m_zoomOutAction;
    }

    [[nodiscard]] std::vector<QAction*> getGridActions() const {
        return m_gridActions;
    }

    [[nodiscard]] QAction* getFreezeAction() const {
        return m_magnifierControls->getFreezeAction();
    }

    [[nodiscard]] std::vector<QAction*> getColorFormatActions() const {
        return m_colorDisplay->getColorFormatActions();
    }

    [[nodiscard]] QAction* getCopyColorAction() const {
        return m_colorDisplay->getCopyColorAction();
    }

private:
    static constexpr Magnifier::GridType k_initialGridType { Magnifier::GridType::Dark };
    static constexpr int k_initialZoomIndex { 1 };
    static constexpr bool k_initialFreeze { false };
    static constexpr ColorFormatId k_initialColorFmt { RGBFmt };

    Magnifier* m_magnifier;
    MagnifierControls* m_magnifierControls;
    ColorDisplay* m_colorDisplay;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
    std::vector<QAction*> m_gridActions;
};
