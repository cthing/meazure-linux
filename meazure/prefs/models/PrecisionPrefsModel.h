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

#include "Preference.h"
#include <meazure/units/Units.h>
#include <meazure/units/UnitsMgr.h>
#include <QObject>
#include <map>
#include <vector>


/// Model for measurement units display precisions. Each measurement unit has associated with it a set of display
/// precisions, one for each measurement type (e.g. width, height, x, y).
///
class PrecisionPrefsModel : public QObject {

    Q_OBJECT

public:
    using LinearPrecisions = std::map<LinearUnitsId, Preference<Units::DisplayPrecisions>*>;
    using AngularPrecisions = std::map<AngularUnitsId, Preference<Units::DisplayPrecisions>*>;

    explicit PrecisionPrefsModel(UnitsMgr* unitsMgr, QObject* parent);

    void initialize();

    void apply() const;

    [[nodiscard]] bool isDirty() const;

    void setPrecision(LinearUnitsId unitsId, LinearMeasurementId measurementId, int precision);
    void setPrecision(AngularUnitsId unitsId, AngularMeasurementId measurementId, int precision);

    LinearPrecisions m_linearPrecisions;
    AngularPrecisions m_angularPrecisions;

signals:
    void dirtyChanged(bool dirty);

private:
    UnitsMgr* m_unitsMgr;
};
