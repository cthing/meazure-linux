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

#include <meazure/units/Units.h>
#include <meazure/utils/MathUtils.h>
#include <QString>


/// Represents custom measurement units in the position log file.
///
class PosLogCustomUnits {

public:
    [[nodiscard]] const QString& getName() const {
        return m_name;
    }

    void setName(const QString& name) {
        m_name = name;
    }

    [[nodiscard]] const QString& getAbbrev() const {
        return m_abbrev;
    }

    void setAbbrev(const QString& abbrev) {
        m_abbrev = abbrev;
    }

    [[nodiscard]] QString getScaleBasisStr() const {
        return m_scaleBasis;
    }

    void setScaleBasisStr(const QString& scaleBasis) {
        m_scaleBasis = scaleBasis;
    }

    [[nodiscard]] double getScaleFactor() const {
        return m_scaleFactor;
    }

    void setScaleFactor(double scaleFactor) {
        m_scaleFactor = scaleFactor;
    }

    [[nodiscard]] const Units::DisplayPrecisions& getDisplayPrecisions() const {
        return m_displayPrecisions;
    }

    void setDisplayPrecisions(const Units::DisplayPrecisions& displayPrecisions) {
        m_displayPrecisions = displayPrecisions;
    }

    void setDisplayPrecision(LinearMeasurementId measurementId, int decimalPlaces) {
        m_displayPrecisions[measurementId] = decimalPlaces;
    }

    bool operator==(const PosLogCustomUnits& rhs) const {
        return m_name == rhs.m_name &&
               m_abbrev == rhs.m_abbrev &&
               m_scaleBasis == rhs.m_scaleBasis &&
               MathUtils::fuzzyEqual(m_scaleFactor, rhs.m_scaleFactor) &&
               m_displayPrecisions == rhs.m_displayPrecisions;
    }

    bool operator!=(const PosLogCustomUnits &rhs) const {
        return !(rhs == *this);
    }

private:
    QString m_name;
    QString m_abbrev;
    QString m_scaleBasis;
    double m_scaleFactor { 0.0 };
    Units::DisplayPrecisions m_displayPrecisions { 0, 0, 0, 0, 0, 0, 0, 0 };
};
