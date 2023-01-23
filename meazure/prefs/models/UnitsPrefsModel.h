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

#include "Preference.h"
#include <meazure/units/CustomUnits.h>
#include <meazure/units/UnitsMgr.h>
#include <QObject>


/// Custom measurement units definition model.
///
class UnitsPrefsModel : public QObject {

    Q_OBJECT

public:
    explicit UnitsPrefsModel(UnitsMgr* unitsMgr, QObject* parent);

    void initialize();

    void apply() const;

    [[nodiscard]] bool isDirty() const;

    Preference<QString>* m_name;
    Preference<QString>* m_abbrev;
    Preference<CustomUnits::ScaleBasis>* m_scaleBasis;
    Preference<double>* m_scaleFactor;

signals:
    void dirtyChanged(bool dirty);

private:
    CustomUnits* m_customUnits;
};
