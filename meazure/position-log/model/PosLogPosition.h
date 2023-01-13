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

#include "PosLogToolData.h"
#include "PosLogDesktop.h"
#include <meazure/tools/RadioToolTraits.h>
#include <QString>
#include <QDateTime>
#include <vector>


/// Represents a tool position. A position object references the desktop information that provides its environment.
/// Since desktop environments do not tend to change often, many position objects typically reference the same
/// desktop information object. The position object is ultimately persisted to the position log file along with
/// the desktop information object.
///
/// A position consists of one or more (x,y) points. The number of points required for a position depends on the
/// measurement tool whose position is being recorded. For example, the Point tool requires only one point to
/// completely describe its position, whereas the Line tool requires two points, one per endpoint of the line.
///
class PosLogPosition {

public:
    [[nodiscard]] const QString& getToolName() const {
        return m_toolName;
    }

    void setToolName(const QString& toolName) {
        m_toolName = toolName;
    }

    [[nodiscard]] RadioToolTraits getToolTraits() const {
        return m_toolTraits;
    }

    void setToolTraits(RadioToolTraits toolTraits) {
        m_toolTraits = toolTraits;
    }

    void addToolTrait(RadioToolTrait toolTrait) {
        m_toolTraits |= toolTrait;
    }

    [[nodiscard]] const PosLogToolData& getToolData() const {
        return m_toolData;
    }

    void setToolData(const PosLogToolData& toolData) {
        m_toolData = toolData;
    }

    [[nodiscard]] const QString& getDescription() const {
        return m_description;
    }

    void setDescription(const QString& description) {
        m_description = description;
    }

    [[nodiscard]] const QDateTime& getRecorded() const {
        return m_recorded;
    }

    void setRecorded(const QDateTime& recorded) {
        m_recorded = recorded;
    }

    [[nodiscard]] PosLogDesktopSharedPtr getDesktop() const {
        return m_desktop;
    }

    void setDesktop(PosLogDesktopSharedPtr desktop) {
        m_desktop = desktop;        // NOLINT(performance-unnecessary-value-param)
    }

    bool operator==(const PosLogPosition &rhs) const {
        const bool desktopsEqual = (m_desktop == rhs.m_desktop) ||
                                   (m_desktop && rhs.m_desktop && *m_desktop == *rhs.m_desktop);

        return m_toolName == rhs.m_toolName &&
               m_toolTraits == rhs.m_toolTraits &&
               m_toolData == rhs.m_toolData &&
               m_description == rhs.m_description &&
               m_recorded == rhs.m_recorded &&
               desktopsEqual;
    }

    bool operator!=(const PosLogPosition &rhs) const {
        return !(rhs == *this);
    }

private:
    QString m_toolName;
    RadioToolTraits m_toolTraits;
    PosLogToolData m_toolData;
    QString m_description;
    QDateTime m_recorded;
    PosLogDesktopSharedPtr m_desktop;
};


using PosLogPositionVector = std::vector<PosLogPosition>;
