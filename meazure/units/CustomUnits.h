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

#include "Units.h"
#include <meazure/config/Config.h>
#include <QString>
#include <QSizeF>
#include <QObject>


/// Provides linear measurements in user defined units.
///
class CustomUnits : public QObject, public LinearUnits {

    Q_OBJECT

public:
    /// User defined units are based on a conversion factor. This factor converts from a basis set of units to the
    /// custom units. The basis for the conversion is identified by this enumeration.
    ///
    enum ScaleBasis {
        PixelBasis = 0,         ///< Custom units = factor * pixels.
        InchBasis = 1,          ///< Custom units = factor * inches.
        CentimeterBasis = 2     ///< Custom units = factor * centimeters.
    };


    static constexpr double k_defScaleFactor { 1.0 };             ///< Default scale factor.
    static constexpr ScaleBasis k_defScaleBasis { PixelBasis };   ///< Default basis for the custom units.


    /// Constructs custom units.
    ///
    /// @param[in] screenInfoProvider Screen information provider
    ///
    explicit CustomUnits(const ScreenInfoProvider* screenInfoProvider);

    ~CustomUnits() override = default;

    CustomUnits(const CustomUnits&) = delete;
    CustomUnits(CustomUnits&&) = delete;
    CustomUnits& operator=(const CustomUnits&) = delete;
    CustomUnits& operator=(CustomUnits&&) = delete;

    /// Persists the custom units conversion factor and basis to the specified configuration object.
    ///
    /// @param[in] config Configuration object into which to persist the custom units.
    ///
    void writeConfig(Config& config) const override;

    /// Restores the custom units conversion factor and basis from the specified configuration object.
    ///
    /// @param[in] config Configuration object from which to restore the custom units.
    ///
    void readConfig(const Config& config) override;

    /// Called when the master setToDefaultValue is initiated. Resets the custom units to its default state.
    ///
    void hardReset() override;

    /// Indicates whether the custom units requires a screen resolution to perform its conversion from pixels.
    ///
    /// @return true if the conversion basis is not based on pixels.
    ///
    [[nodiscard]] bool isResRequired() const override;

    /// Indicates whether custom units have been defined.
    ///
    /// @return true if custom units have been defined. Custom units are considered to be defined if they have a
    ///         name and abbreviation.
    ///
    [[nodiscard]] bool haveCustomUnits() const {
        return !m_name.isEmpty() && !m_abbrev.isEmpty();
    }

    /// Returns the name of the custom units. The name is used on the units menu as well as other places.
    ///
    /// @return Name for the units.
    ///
    [[nodiscard]] QString getName() const {
        return m_name;
    }

    /// Sets a name for the custom units. The name is displayed on the units menu and in other places.
    ///
    /// @param[in] name Name for the units.
    ///
    void setName(const QString& name);

    /// Returns the abbreviation for the custom units. The abbreviation is used by the data display and in other places.
    ///
    /// @return Abbreviation for the units.
    ///
    [[nodiscard]] QString getAbbrev() const {
        return m_abbrev;
    }

    /// Sets an abbreviation for the custom units. The abbreviation is displayed by the data display and in
    /// other places.
    ///
    /// @param[in] abbrev Abbreviation for the units.
    ///
    void setAbbrev(const QString& abbrev);

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the conversion basis.
    ///
    /// @return Custom units conversion basis.
    ///
    [[nodiscard]] ScaleBasis getScaleBasis() const {
        return m_scaleBasis;
    }

    /// Returns the scale basis as an identifying string (e.g. "px").
    ///
    /// @return Conversion basis as a string.
    ///
    [[nodiscard]] QString getScaleBasisStr() const;

    /// Sets the conversion basis.
    ///
    /// @param[in] scaleBasis Conversion basis.
    ///
    void setScaleBasis(ScaleBasis scaleBasis);

    /// Sets the conversion basis using a string identifier.
    ///
    /// @param[in] scaleBasisStr Conversion basis as an identifying string.
    ///
    void setScaleBasis(const QString& scaleBasisStr);

    /// Returns the conversion factor for the custom units.
    ///
    /// @return Conversion factor from the conversion basis to the custom units.
    ///
    [[nodiscard]] double getScaleFactor() const {
        return m_scaleFactor;
    }

    /// Sets the conversion factor for the custom units.
    ///
    /// @param[in] scaleFactor Conversion factor from the conversion basis to the custom units.
    ///
    void setScaleFactor(double scaleFactor);

    /// Returns the X and Y factors to convert from pixels to the custom units. In other words, multiplying the values
    /// returned from this method by pixels results in a value expressed in the custom units.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in custom units/pixels.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;

signals:
    /// Emitted when the custom units definition is changed (e.g. name, abbreviation, scale factor).
    ///
    void customUnitsChanged();

private:
    QString m_name;                     ///< Name for the custom units.
    QString m_abbrev;                   ///< Abbreviation for the custom units.
    ScaleBasis m_scaleBasis;            ///< Conversion basis for the custom units.
    double m_scaleFactor;               ///< Conversion factor for the custom units.
};
