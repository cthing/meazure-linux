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

#include "UnitsPrefsModel.h"


UnitsPrefsModel::UnitsPrefsModel(UnitsMgr& unitsMgr, QObject *parent) :
        QObject(parent),
        m_customUnits(unitsMgr.getCustomUnits()) {
    m_name = new Preference<QString>(m_customUnits->getName());
    m_abbrev = new Preference<QString>(m_customUnits->getAbbrev());
    m_scaleBasis = new Preference<CustomUnits::ScaleBasis>(m_customUnits->getScaleBasis());
    m_scaleFactor = new Preference<double>(m_customUnits->getScaleFactor());

    connect(m_name, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_abbrev, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_scaleBasis, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_scaleFactor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
}

void UnitsPrefsModel::initialize() {     // NOLINT(readability-make-member-function-const)
    m_name->initialize(m_customUnits->getName());
    m_abbrev->initialize(m_customUnits->getAbbrev());
    m_scaleBasis->initialize(m_customUnits->getScaleBasis());
    m_scaleFactor->initialize(m_customUnits->getScaleFactor());
}

void UnitsPrefsModel::apply() const {
    if (m_name->isDirty()) {
        m_customUnits->setName(*m_name);
        m_name->clearDirty();
    }
    if (m_abbrev->isDirty()) {
        m_customUnits->setAbbrev(*m_abbrev);
        m_abbrev->clearDirty();
    }
    if (m_scaleBasis->isDirty()) {
        m_customUnits->setScaleBasis(*m_scaleBasis);
        m_scaleBasis->clearDirty();
    }
    if (m_scaleFactor->isDirty()) {
        m_customUnits->setScaleFactor(*m_scaleFactor);
        m_scaleFactor->clearDirty();
    }
}

bool UnitsPrefsModel::isDirty() const {
    return m_name->isDirty() || m_abbrev->isDirty() || m_scaleBasis->isDirty() || m_scaleFactor->isDirty();
}
