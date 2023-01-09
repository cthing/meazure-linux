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

#include "PosLogInfo.h"
#include "PosLogDesktop.h"
#include "PosLogPosition.h"
#include <vector>


/// Represents the complete position log file for serialization and deserialization.
///
class PosLogArchive {

public:
    [[nodiscard]] const PosLogInfo& getInfo() const {
        return m_info;
    }

    void setInfo(const PosLogInfo& info) {
        m_info = info;
    }

    [[nodiscard]] const PosLogDesktopSharedPtrVector& getDesktops() const {
        return m_desktops;
    }

    [[nodiscard]] PosLogDesktopSharedPtr getOrCreateDesktop(const QString& id) {
        for (PosLogDesktopSharedPtr desktop : m_desktops) {
            if (id == desktop->getId()) {
                return desktop;
            }
        }

        return std::make_shared<PosLogDesktop>(id);
    }

    void addDesktop(PosLogDesktopSharedPtr desktop) {   // NOLINT(performance-unnecessary-value-param)
        m_desktops.push_back(desktop);
    }

    [[nodiscard]] const PosLogPositionVector& getPositions() const {
        return m_positions;
    }

    void setPositions(const PosLogPositionVector& positions) {
        m_positions = positions;
    }

private:
    PosLogInfo m_info;
    PosLogDesktopSharedPtrVector m_desktops;
    PosLogPositionVector m_positions;
};
