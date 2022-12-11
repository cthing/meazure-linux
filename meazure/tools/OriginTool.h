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

#include "Tool.h"
#include <meazure/graphics/OriginMarker.h>


/// Origin marker tool. Two thin lines are arranged to form axes indicators on the screen at the location of the
/// origin of the coordinate system. The intersection of the lines indicates the origin and the direction of the
/// lines from the origin indicates the positive direction along the axes.
///
class OriginTool : public Tool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "OriginTool" };

    OriginTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
               QObject* parent = nullptr);

    ~OriginTool() override;

    OriginTool(const OriginTool&) = delete;
    OriginTool(OriginTool&&) = delete;
    OriginTool& operator=(const OriginTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    void setEnabled(bool enable) override;

    void saveProfile(Profile& profile) const override;
    void loadProfile(Profile& profile) override;

    void refresh() override;

private:
    void setPosition();

    OriginMarker* m_marker;
};