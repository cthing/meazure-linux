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

#include "PrecisionPrefsModel.h"
#include <algorithm>


PrecisionPrefsModel::PrecisionPrefsModel(UnitsMgr* unitsMgr, QObject *parent) :
        QObject(parent),
        m_unitsMgr(unitsMgr) {
    for (const LinearUnitsId unitsId : LinearUnitsIdIter()) {
        auto* pref = new Preference(m_unitsMgr->getLinearDefaultPrecisions(unitsId));
        m_linearPrecisions[unitsId] = pref;
        connect(pref, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    }
    for (const AngularUnitsId unitsId : AngularUnitsIdIter()) {
        auto* pref = new Preference(m_unitsMgr->getAngularDefaultPrecisions(unitsId));
        m_angularPrecisions[unitsId] = pref;
        connect(pref, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    }
}

void PrecisionPrefsModel::initialize() {
    for (const auto& entry : m_linearPrecisions) {
        entry.second->initialize(m_unitsMgr->getLinearDisplayPrecisions(entry.first));
    }
    for (const auto& entry : m_angularPrecisions) {
        entry.second->initialize(m_unitsMgr->getAngularDisplayPrecisions(entry.first));
    }
}

void PrecisionPrefsModel::apply() const {
    for (const auto& entry : m_linearPrecisions) {
        if (entry.second->isDirty()) {
            m_unitsMgr->setLinearDisplayPrecisions(entry.first, entry.second->getValue());
        }
    }
    for (const auto& entry : m_angularPrecisions) {
        if (entry.second->isDirty()) {
            m_unitsMgr->setAngularDisplayPrecisions(entry.first, entry.second->getValue());
        }
    }
}

bool PrecisionPrefsModel::isDirty() const {
    return std::any_of(m_linearPrecisions.begin(), m_linearPrecisions.end(), [](const auto& entry) {
        return entry.second->isDirty();
    }) || std::any_of(m_angularPrecisions.begin(), m_angularPrecisions.end(), [](const auto& entry) {
        return entry.second->isDirty();
    });
}

void PrecisionPrefsModel::setPrecision(LinearUnitsId unitsId, LinearMeasurementId measurementId, int precision) {
    Units::DisplayPrecisions precisions = m_linearPrecisions.at(unitsId)->getValue();
    precisions.at(measurementId) = precision;
    m_linearPrecisions.at(unitsId)->setValue(precisions);
}

void PrecisionPrefsModel::setPrecision(AngularUnitsId unitsId, AngularMeasurementId measurementId, int precision) {
    Units::DisplayPrecisions precisions = m_angularPrecisions.at(unitsId)->getValue();
    precisions.at(measurementId) = precision;
    m_angularPrecisions.at(unitsId)->setValue(precisions);
}
