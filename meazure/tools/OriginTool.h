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

#include "Tool.h"
#include <meazure/graphics/OriginMarker.h>
#include <meazure/config/Config.h>


/// Origin marker tool. Two thin lines are arranged to form axes indicators on the screen at the location of the
/// origin of the coordinate system. The intersection of the lines indicates the origin and the direction of the
/// lines from the origin indicates the positive direction along the axes.
///
class OriginTool : public Tool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "OriginTool" };

    OriginTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~OriginTool() override;

    OriginTool(const OriginTool&) = delete;
    OriginTool(OriginTool&&) = delete;
    OriginTool& operator=(const OriginTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    void setEnabled(bool enable) override;

    void writeConfig(Config& config) const override;
    void readConfig(const Config& config) override;

    void hardReset() override;

    void refresh() override;

private:
    void setPosition();

    OriginMarker* m_marker;
};
