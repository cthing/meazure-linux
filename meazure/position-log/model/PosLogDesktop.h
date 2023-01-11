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

#include "PosLogCustomUnits.h"
#include "PosLogScreen.h"
#include <meazure/units/Units.h>
#include <QUuid>
#include <QPointF>
#include <QSizeF>
#include <vector>
#include <memory>


/// Represents information about the system at the time a position is recorded. This includes the list of all display
/// screens attached to the system. The desktop information is saved in the position log file. To minimize repetition,
/// positions reference the desktop information in effect when they were recorded. This means that one desktop
/// information section can serve many positions.
///
class PosLogDesktop {

public:

    PosLogDesktop() : PosLogDesktop(QUuid::createUuid()) {
    }

    explicit PosLogDesktop(const QString& id) : PosLogDesktop(QUuid::fromString(id)) {
    }

    explicit PosLogDesktop(const QUuid& id) : m_id(id) {
    }

    [[nodiscard]] QString getId() const {
        return m_id.toString(QUuid::WithoutBraces);
    }

    [[nodiscard]] const QPointF& getOrigin() const {
        return m_origin;
    }

    void setOrigin(const QPointF& origin) {
        m_origin = origin;
    }

    [[nodiscard]] bool isInvertY() const {
        return m_invertY;
    }

    void setInvertY(bool invertY) {
        m_invertY = invertY;
    }

    [[nodiscard]] const QSizeF& getSize() const {
        return m_size;
    }

    void setSize(const QSizeF& size) {
        m_size = size;
    }

    [[nodiscard]] LinearUnitsId getLinearUnitsId() const {
        return m_linearUnitsId;
    }

    void setLinearUnitsId(LinearUnitsId unitsId) {
        m_linearUnitsId = unitsId;
    }

    [[nodiscard]] AngularUnitsId getAngularUnitsId() const {
        return m_angularUnitsId;
    }

    void setAngularUnitsId(AngularUnitsId unitsId) {
        m_angularUnitsId = unitsId;
    }

    [[nodiscard]] const PosLogScreenVector& getScreens() const {
        return m_screens;
    }

    void addScreen(const PosLogScreen& screen) {
        m_screens.push_back(screen);
    }

    [[nodiscard]] const PosLogCustomUnits& getCustomUnits() const {
        return m_customUnits;
    }

    void setCustomUnits(const PosLogCustomUnits& customUnits) {
        m_customUnits = customUnits;
    }

    [[nodiscard]] bool isSame(const PosLogDesktop &rhs) const {
        return m_origin == rhs.m_origin &&
               m_invertY == rhs.m_invertY &&
               m_size == rhs.m_size &&
               m_linearUnitsId == rhs.m_linearUnitsId &&
               m_angularUnitsId == rhs.m_angularUnitsId &&
               m_screens == rhs.m_screens &&
               m_customUnits == rhs.m_customUnits;
    }

    bool operator==(const PosLogDesktop &rhs) const {
        return m_id == rhs.m_id && isSame(rhs);
    }

    bool operator!=(const PosLogDesktop &rhs) const {
        return !(rhs == *this);
    }

private:
    QUuid m_id;                                     ///< ID for use by a position object to reference this object.
    QPointF m_origin;                               ///< Origin in the units in effect when this object was created.
    bool m_invertY { false };                       ///< Is origin inverted.
    QSizeF m_size;                                  ///< Desktop size in units in effect when desktop object created.
    LinearUnitsId m_linearUnitsId { PixelsId };     ///< Current linear units.
    AngularUnitsId m_angularUnitsId { DegreesId };  ///< Current angle units.
    PosLogScreenVector m_screens;                   ///< List of display screens.
    PosLogCustomUnits m_customUnits;
};


using PosLogDesktopSharedPtr = std::shared_ptr<PosLogDesktop>;
using PosLogDesktopWeakPtr = std::weak_ptr<PosLogDesktop>;
using PosLogDesktopSharedPtrVector = std::vector<PosLogDesktopSharedPtr>;
using PosLogDesktopWeakPtrVector = std::vector<PosLogDesktopWeakPtr>;
