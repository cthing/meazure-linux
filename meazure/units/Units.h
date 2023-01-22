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

#include <meazure/config/Config.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/utils/EnumIterator.h>
#include <vector>
#include <QString>
#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QWidget>
#include <limits>
#include <array>


/// Identifiers for linear measurement units.
///
enum LinearUnitsId {
    PixelsId = 0,           ///< Pixel units.
    PointsId = 1,           ///< Point units (1/72 inch).
    TwipsId = 2,            ///< Twip units (1/1440 inch).
    InchesId = 3,           ///< Inch units.
    CentimetersId = 4,      ///< Centimeter units (1/2.54 inch)
    MillimetersId = 5,      ///< Millimeter units (1/25.4 inch)
    PicasId = 6,            ///< Pica units (1/6 inch)
    CustomId = 7            ///< User defined units
};

using LinearUnitsIdIter = EnumIterator<LinearUnitsId, PixelsId, CustomId>;

/// Identifiers for angular measurement units.
///
enum AngularUnitsId {
    DegreesId = 0,          ///< Angles reported in degrees.
    RadiansId = 1           ///< Angles reported in radians.
};

using AngularUnitsIdIter = EnumIterator<AngularUnitsId, DegreesId, RadiansId>;


/// Types of linear measurements.
///
enum LinearMeasurementId {
    XCoord = 0,     ///< X coordinate
    YCoord = 1,     ///< Y coordinate
    Width = 2,      ///< Width
    Height = 3,     ///< Height
    Distance = 4,   ///< Distance
    Area = 5,       ///< Area
    ResX = 6,       ///< Screen resolution in the X direction
    ResY = 7        ///< Screen resolution in the Y direction
};

/// Types of angular measurements.
///
enum AngularMeasurementId {
    Angle = 0       ///< Angle
};


/// The screen resolution can differ in the X and Y directions. When conversions require the use of the screen
/// resolution, a direction must be specified so that the correct resolution is used. These identifiers specify
/// the direction for conversions.
///
enum ConvertDir {
    ConvertX = 0,       ///< X axis direction.
    ConvertY = 1        ///< Y axis direction.
};


/// Base class for all measurement units.
///
/// Measurement units are packaged into a class, one for each set of units (e.g. inches). Each class contains methods
/// for conversion, display precision, and persistence of the state of the class to a given configuration object.
/// Related to these classes but not contained by them are the set of unit label classes.
///
class Units {

public:
    using DisplayPrecisions = std::vector<int>;             ///< Decimal places to display for each type of measurement.
    using DisplayPrecisionNames = std::vector<QString>;     ///< Strings to identify units precisions in configurations.

    virtual ~Units() = default;

    /// Persists the state of a units class instance to the specified configuration object.
    ///
    /// @param[in] config Configuration object into which to persist the state of the units class.
    ///
    virtual void writeConfig(Config& config) const;

    /// Restores the state of a units class instance from the specified configuration object.
    ///
    /// @param[in] config Configuration object from which to restore the state of the units class.
    ///
    virtual void readConfig(const Config& config);

    /// Persists the measurement precisions to the specified configuration object.
    ///
    /// @param[in] config Configuration object into which to persist the decimal place precisions.
    ///
    void writePrecision(Config& config) const;

    /// Restores the measurement precisions from the specified configuration object.
    ///
    /// @param[in] config Configuration object from which to restore the decimal place precisions.
    ///
    void readPrecision(const Config& config);

    /// Called to restores the units to their default state.
    ///
    virtual void hardReset();

    /// Returns a string describing the units.
    ///
    /// @return String describing he units (e.g. "px')
    ///
    [[nodiscard]] QString getUnitsStr() const { return m_unitsStr; }

    /// Sets the decimal place precisions for all measurement types.
    ///
    /// @param[in] displayPrecisions Set of precisions for all measurement types.
    ///
    void setDisplayPrecisions(const DisplayPrecisions& displayPrecisions) {
        m_displayPrecisions = displayPrecisions;
    }

    /// Returns the current decimal place precisions for all measurement types.
    ///
    /// @return Set of precisions for all measurement types.
    ///
    [[nodiscard]] const DisplayPrecisions& getDisplayPrecisions() const {
        return m_displayPrecisions;
    }

    /// Returns the default decimal place precisions for all measurement types.
    ///
    /// @return Set of default precisions for all measurement types.
    ///
    [[nodiscard]] const DisplayPrecisions& getDefaultPrecisions() const {
        return m_defaultPrecisions;
    }

    /// Sets the default display precisions as the current precisions.
    ///
    void restoreDefaultPrecisions() {
        m_displayPrecisions = m_defaultPrecisions;
    }

    /// Returns the identifying names for the display precisions.
    ///
    /// @return Set of names identifying precisions for all measurement types.
    ///
    [[nodiscard]] const DisplayPrecisionNames& getDisplayPrecisionNames() const {
        return m_displayPrecisionNames;
    }

protected:
    /// Base class constructor.
    ///
    /// @param[in] unitsStr Name for the units.
    ///
    explicit Units(QString unitsStr);

    Units(const Units&) = default;
    Units(Units&&) = default;
    Units& operator=(const Units&) = default;
    Units& operator=(Units&&) = default;

    /// Adds the specified identifying name to the list of precision names.
    ///
    /// @param[in] name Identifying name for the precision.
    ///
    void addPrecisionName(const QString& name) {
        m_displayPrecisionNames.push_back(name);
    }

    /// Adds the specified precision value to both the list of current precisions and the list of default precisions.
    ///
    /// @param[in] precision Number of decimal places to display.
    ///
    void addPrecision(int precision) {
        m_defaultPrecisions.push_back(precision);
        m_displayPrecisions.push_back(precision);
    }

private:
    DisplayPrecisions m_defaultPrecisions;          ///< Default precisions for all measurement types.
    DisplayPrecisions m_displayPrecisions;          ///< Current precisions for all measurement types.
    DisplayPrecisionNames m_displayPrecisionNames;  ///< Names for all precision values.
    QString m_unitsStr;                             ///< Name for the units.
};


/// There are two major categories of units: linear and angular. The angular units cover the angle measurement units.
/// This is the base class for all angular units.
///
class AngularUnits : public Units {

public:
    /// Destroys an angular-based unit.
    ///
    ~AngularUnits() override = default;

    /// Returns the identifier for the angular units.
    ///
    /// @return Angular units ID.
    ///
    [[nodiscard]] AngularUnitsId getUnitsId() const { return m_unitsId; }

    /// Sets whether to show the included angle (default) or the supplemental angle. The supplemental angle is
    /// defined as:
    /// \f[
    ///     \theta_{s} =
    ///         \begin{cases}
    ///             180^\circ - \theta_{i}  & \theta_{i} \ge 0\\
    ///             -180^\circ - \theta_{i} & \theta_{i} < 0
    ///         \end{cases}
    /// \f]
    ///
    /// @param showSupplemental     [in] true to show supplemental angle instead of included angle
    ///
    void setSupplementalAngle(bool showSupplemental) { m_supplementalAngle = showSupplemental; }

    /// Indicates whether the included angle is shown (default) or the supplemental angle.
    ///
    /// @return true if the supplemental angle is shown.
    ///
    [[nodiscard]] bool isSupplementalAngle() const { return m_supplementalAngle; }

    /// Obtains the number of decimal places to display the specified measurement.
    ///
    /// @param[in] id Identifier of measurement
    /// @return Number of decimal places to display the specified measurement.
    ///
    [[nodiscard]] int getDisplayPrecision(AngularMeasurementId id) const {
        return getDisplayPrecisions().at(id);
    }

    /// Formats the specified angular measurement value using the precision for the specified measurement ID.
    ///
    /// @param[in] id Identifier for the angular units whose precision is to be used to format the specified value.
    /// @param[in] value Measurement value to be formatted.
    ///
    /// @return Measurement value formatted with the appropriate precision and returned as a string.
    ///
    [[nodiscard]] QString format(AngularMeasurementId id, double value) const;

    /// Obtains the user visible label for the units.
    ///
    /// @return User visible label for the units.
    ///
    [[nodiscard]] virtual QString getLabel() const = 0;

    /// Converts the specified angle value from its native radians to the desired units.
    ///
    /// @param[in] angle Value to be converted.
    ///
    /// @return Angle value converted to the desired units.
    ///
    [[nodiscard]] virtual double convertAngle(double angle) const = 0;

protected:
    /// Constructs an angular-based unit.
    ///
    /// @param[in] unitsId Identifier for the angular units.
    /// @param[in] unitsStr Name for the units.
    ///
    AngularUnits(AngularUnitsId unitsId, const QString& unitsStr);

    AngularUnits(const AngularUnits&) = default;
    AngularUnits(AngularUnits&&) = default;
    AngularUnits& operator=(const AngularUnits&) = default;
    AngularUnits& operator=(AngularUnits&&) = default;

private:
    AngularUnitsId m_unitsId;           ///< Identifier for the units.
    bool m_supplementalAngle { false }; ///< Indicates if the supplemental rather than included angle should be shown.
};


/// Provides angular measurements in degrees.
///
class DegreeUnits : public AngularUnits {

public:
    /// Constructs a degree measurement unit.
    ///
    DegreeUnits();

    /// Destroys a degree measurement unit.
    ///
    ~DegreeUnits() override = default;

    DegreeUnits(const DegreeUnits&) = default;
    DegreeUnits(DegreeUnits&&) = default;
    DegreeUnits& operator=(const DegreeUnits&) = default;
    DegreeUnits& operator=(DegreeUnits&&) = default;

    [[nodiscard]] QString getLabel() const override;

    /// Converts the specified angle value from its native radians to degrees.
    ///
    /// @param[in] angle Value to be converted from radians to degrees.
    ///
    /// @return Angle value in degrees.
    ///
    [[nodiscard]] double convertAngle(double angle) const override;
};


/// Provides angular measurements in radians.
///
class RadianUnits : public AngularUnits {

public:
    /// Constructs a radian measurement unit.
    ///
    RadianUnits();

    /// Destroys a radian measurement unit.
    ///
    ~RadianUnits() override = default;

    RadianUnits(const RadianUnits&) = default;
    RadianUnits(RadianUnits&&) = default;
    RadianUnits& operator=(const RadianUnits&) = default;
    RadianUnits& operator=(RadianUnits&&) = default;

    [[nodiscard]] QString getLabel() const override;

    /// Passthru method returning the specified angle in radians.
    ///
    /// @param[in] angle Angle value in radians.
    ///
    /// @return Angle value in radians.
    ///
    [[nodiscard]] double convertAngle(double angle) const override;
};


/// There are two major categories of units: linear and angular. The linear units cover distance related measurements
/// (e.g. coordinates, distances, resolutions). This is the base class for all linear units.
///
class LinearUnits : public Units {

public:
    static constexpr int k_aspectPrecision { std::numeric_limits<float>::digits10 - 1 };

    /// Destroys the linear units.
    ///
    ~LinearUnits() override = default;

    LinearUnits& operator=(const LinearUnits&) = delete;
    LinearUnits& operator=(LinearUnits&&) = delete;

    /// Returns the identifier for the linear units.
    ///
    /// @return Linear units ID.
    ///
    [[nodiscard]] LinearUnitsId getUnitsId() const { return m_unitsId; }

    /// By default, the X11 coordinate system is inverted from the typical Cartesian coordinate system in that the
    /// origin is located in the upper left corner of the primary screen and the positive y-axis points downward.
    /// This method sets whether the y-axis should be inverted so that the origin is moved to the lower left corner
    /// of the primary screen and the positive y-axis points up. Note that the actual location of the origin is
    /// determined by the setOrigin method.
    ///
    /// @param[in] invertY true inverts the y-axis, placing the origin at the lower left of the primary
    ///         display screen and making positive y pointing upward.
    ///
    void setInvertY(bool invertY) { m_invertY = invertY; }

    /// Returns the orientation of the y-axis.
    ///
    /// @return If true is returned, the origin of the coordinate system is located at the lower left of the primary
    ///         screen, and the positive y-axis is pointing upward. If false is returned, the origin is located at
    ///         the upper left corner and the positive y-axis is pointing down. Note that the actual location of the
    ///         origin is determined by the setOrigin method.
    ///
    [[nodiscard]] bool isInvertY() const { return m_invertY; }

    /// Moves the origin of the coordinate system to the specified point. The orientation of the axes is not
    /// effected by this method. To change the  orientation of the y-axis use the setInvertY method.
    ///
    /// @param[in] origin New location for the origin of the coordinate system, in pixels.
    ///
    void setOrigin(const QPoint& origin) { m_originOffset = origin; }

    /// Returns the location of the origin of the coordinate system.
    ///
    /// @return Location of the origin of the coordinate system, in pixels.
    ///
    [[nodiscard]] const QPoint& getOrigin() const { return m_originOffset; }

    /// Internally all measurements are in pixels. Measurement units based solely on pixels do not require the use of
    /// the screen resolution for conversion. Measurement units such as inches, require the screen resolution for
    /// conversion from pixels. This method indicates whether the screen resolution is required for conversion from
    /// pixels to the desired measurement units and vice versa.
    ///
    /// @return true if the screen resolution must be used to convert to and from pixels. This base class
    ///         implementation always returns true.
    ///
    [[nodiscard]] virtual bool isResRequired() const;

    /// Obtains the number of decimal places to display the specified measurement.
    ///
    /// @param[in] id Identifier of measurement
    /// @return Number of decimal places to display the specified measurement.
    ///
    [[nodiscard]] int getDisplayPrecision(LinearMeasurementId id) const {
        return getDisplayPrecisions().at(id);
    }

    /// Formats the specified linear measurement value using the precision for the specified measurement ID.
    ///
    /// @param[in] id Identifier for the linear units whose precision is to be used to format the specified value.
    /// @param[in] value Measurement value to be formatted.
    ///
    /// @return Measurement value formatted with the appropriate precision and returned as a string.
    ///
    [[nodiscard]] QString format(LinearMeasurementId id, double value) const;

    /// Formats the specifiedaspect ratio value with the appropriate precision.
    ///
    /// @param[in] value Aspect ratio
    /// @return Aspect ratio formatted for display
    ///
    [[nodiscard]] static QString formatAspect(double value) {
        return QString::number(value, 'f', k_aspectPrecision);
    }

    /// Obtains the user visible length measurement label.
    ///
    /// @return User visible length measurement label.
    ///
    [[nodiscard]] virtual QString getLengthLabel() const = 0;

    /// Obtains the user visible area measurement label.
    ///
    /// @return User visible area measurement label.
    ///
    [[nodiscard]] virtual QString getAreaLabel() const;

    /// Obtains the user visible resolution measurement label.
    ///
    /// @return User visible resolution measurement label.
    ///
    [[nodiscard]] virtual QString getResLabel() const;

    /// Converts the specified coordinate from pixels to the desired units. This conversion takes into account the
    /// location of the origin and the orientation of the y-axis.
    ///
    /// @param[in] pos Coordinates in pixels to convert to the desired units.
    ///
    /// @return Coordinates converted from pixels to the desired units. The conversion takes into account the location
    ///         of the origin and the orientation of the y-axis.
    ///
    [[nodiscard]] QPointF convertCoord(const QPoint& pos) const;

    /// Converts the specified position from pixels to the desired units. This conversion does not take into account
    /// the location of the origin nor does it compensate for the orientation of the y-axis.
    ///
    /// @param[in] pos Position in pixels to convert to the desired units.
    ///
    /// @return Position converted from pixels to the desired units. The conversion does not take into account the
    ///         location of the origin nor does it compensate for the orientation of the y-axis.
    ///
    [[nodiscard]] QPointF convertPos(const QPoint& pos) const;

    /// Converts the specified resolution in pixels/inch to the desired units.
    ///
    /// @param[in] res Resolution in pixels/inch to convert to the desired units.
    ///
    /// @return Resolution converted from pixels/inch to the desired units.
    ///
    [[nodiscard]] QSizeF convertRes(const QSizeF& res) const;

    /// Converts from the current units to pixels. The conversion takes into account the location of the origin and
    /// the orientation of the y-axis. The conversion is performed for the specified coordinate located on the
    /// specified axis.
    ///
    /// @param[in] dir Axis on which the conversion should take place.
    /// @param[in] wnd Window used to determine the screen resolution.
    /// @param[in] pos Coordinate to convert to pixels.
    ///
    /// @return Coordinate converted from the current units to pixels. The conversion takes into account the location
    ///         of the origin and the orientation of the y-axis.
    ///
    double unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos) const;

    /// Converts from the current units to pixels. The conversion takes into account the location of the origin and
    /// the orientation of the y-axis. The conversion is performed for the specified coordinate located on the
    /// specified axis. Since the specified coordinate might not convert to an integral number of pixels, this method
    /// returns the bounding integral pixel values and indicates whether an exact integral conversion could be
    /// performed.
    ///
    /// @param[in] dir Axis on which the conversion should take place.
    /// @param[in] wnd Window used to determine the screen resolution.
    /// @param[in] pos Coordinate to convert to pixels.
    /// @param[out] c1 Closest integral pixel value below the converted coordinate.
    /// @param[out] c2 Closest integral pixel value above the converted coordinate.
    ///
    /// @return true if the conversion to pixels was exact.
    ///
    bool unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos, int& c1, int& c2) const;

    /// Converts from the current units to pixels. The conversion takes into account the location of the origin and
    /// the orientation of the y-axis.
    ///
    /// @param[in] pos Coordinates to convert to pixels.
    ///
    /// @return Coordinates converted from the current units to pixels. The conversion takes into account the location
    ///         of the origin and the orientation of the y-axis.
    ///
    [[nodiscard]] QPoint unconvertCoord(const QPointF& pos) const;

    /// Converts from the current units to pixels. The conversion does not take into account the location of the
    /// origin nor the orientation of the y-axis.
    ///
    /// @param[in] pos Position to convert to pixels.
    ///
    /// @return Position converted from the current units to pixels. The conversion does not take into account the
    ///         location of the origin nor the orientation of the y-axis.
    ///
    [[nodiscard]] QPoint unconvertPos(const QPointF& pos) const;

    /// Converts the specified value from the current units to pixels. A minimum pixel value is specified in case the
    /// resolution is such that the conversion to pixels results in a value that is too small.
    ///
    /// @param[in] res Screen resolution to use to perform the conversion.
    /// @param[in] value Value to be converted to pixels.
    /// @param[in] minPixels If the converted pixel value in less than this minimum value, the minimum value is
    ///         returned.
    ///
    /// @return X and Y pixel values.
    ///
    [[nodiscard]] QSize convertToPixels(const QSizeF& res, double value, int minPixels) const;

    /// Returns the X and Y factors to convert from pixels to the current units. In other words, multiplying the
    /// values returned from this method by pixels results in a value expressed in the current units.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in units/pixels.
    ///
    [[nodiscard]] virtual QSizeF fromPixels(const QSizeF& res) const = 0;

protected:
    /// Constructs the linear units.
    ///
    /// @param[in] unitsId Identifier for the linear units.
    /// @param[in] unitsStr Name for the units.
    /// @param[in] screenInfoProvider Provides information about screen resolution
    ///
    LinearUnits(LinearUnitsId unitsId, const QString& unitsStr, const ScreenInfoProvider& screenInfoProvider);

    LinearUnits(const LinearUnits&) = default;
    LinearUnits(LinearUnits&&) = default;

    /// Returns a resolution conversion factor. For most units this is the same as the values returned by the
    /// fromPixels method. However, since internally the resolution is expressed as pixels per inch, the pixel units
    /// need to provide their own conversion factor.
    ///
    /// @param[in] res Resolution to convert.
    ///
    /// @return Resolution conversion factor.
    ///
    [[nodiscard]] virtual QSizeF getResFromPixels(const QSizeF& res) const;

    /// Converts from the pixels to the current units. The conversion takes into account the location of the origin
    /// and the orientation of the y-axis. The conversion is performed for the specified coordinate located on the
    /// specified axis.
    ///
    /// @param[in] dir Axis on which the conversion should take place.
    /// @param[in] wnd Window used to determine the screen resolution.
    /// @param[in] pos Coordinate to convert from pixels.
    ///
    /// @return Coordinate converted from pixels to the current units. The conversion takes into account the location
    ///         of the origin and the orientation of the y-axis.
    ///
    double convertCoord(ConvertDir dir, const QWidget* wnd, int pos) const;

    /// In a multiple monitor environment there are multiple screen resolutions, one set per monitor. Therefore, to
    /// determine the a resolution, a screen must be determined. This method uses the specified position to determined
    /// a screen and thus a resolution. The method compensates for the location of the origin and the orientation of
    /// the y-axis.
    ///
    /// @param[in] pos Position used to determine a screen and thus a resolution, in the current units.
    ///
    /// @return Screen resolution, in the current units.
    ///
    [[nodiscard]] QSizeF findResFromCoord(const QPointF& pos) const;

    /// In a multiple monitor environment there are multiple screen resolutions, one set per monitor. Therefore, to
    /// determine the a resolution, a screen must be determined. This method uses the specified position to determined
    /// a screen and thus a resolution. The method does not compensate for the location of the origin nor the
    /// orientation of the y-axis.
    ///
    /// @param[in] pos Position used to determine a screen and thus a resolution, in the current units.
    ///
    /// @return Screen resolution, in the current units.
    ///
    [[nodiscard]] QSizeF findResFromPos(const QPointF& pos) const;

private:
    QPoint m_originOffset { 0, 0 };                 ///< Offset of the origin from the system origin, in pixels.
    bool m_invertY { false };                       ///< Indicates if the y-axis direction is inverted.
    const ScreenInfoProvider& m_screenInfoProvider; ///< Display screen information
    LinearUnitsId m_unitsId;                        ///< Linear units identifier.
};


/// Provides linear measurements in pixels.
///
class PixelUnits : public LinearUnits {

public:
    /// Constructs pixel units.
    ///
    /// @param[in] screenInfoProvider Information about display screens
    ///
    explicit PixelUnits(const ScreenInfoProvider& screenInfoProvider);

    ~PixelUnits() override = default;

    PixelUnits(const PixelUnits&) = default;
    PixelUnits(PixelUnits&&) = default;
    PixelUnits& operator=(const PixelUnits&) = delete;
    PixelUnits& operator=(PixelUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    [[nodiscard]] QString getResLabel() const override;

    /// Internally all measurements are in pixels. Measurement units based solely on pixels do not require the use
    /// of the screen resolution for conversion. Measurement units such as inches, require the screen resolution for
    /// conversion from pixels. This method indicates whether the screen resolution is required for conversion from
    /// pixels to the desired measurement units and vice versa.
    ///
    /// @return Always returns false since the resolution is not used when measuring using pixels.
    ///
    [[nodiscard]] bool isResRequired() const override;

    /// Returns the X and Y factors to convert from pixels to the current units. Since the current units are pixels,
    /// the factor return is 1.0 in both directions.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return 1.0 in both directions.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;

protected:
    /// Returns a resolution conversion factor. Since internally the resolution is expressed as pixels per inch,
    /// the pixel units need to provide a resolution conversion factor that is effectively cancels out the conversion.
    ///
    /// @param[in] res Resolution to convert.
    ///
    /// @return Resolution conversion factor.
    ///
    [[nodiscard]] QSizeF getResFromPixels(const QSizeF& res) const override;
};


/// Provides linear measurements in points.
///
class PointUnits : public LinearUnits {

public:
    static constexpr double k_ptPerIn { 72.0 };

    /// Constructs point units.
    ///
    /// @param[in] screenInfoProvider Information about display screens
    ///
    explicit PointUnits(const ScreenInfoProvider& screenInfoProvider);

    ~PointUnits() override = default;

    PointUnits(const PointUnits&) = default;
    PointUnits(PointUnits&&) = default;
    PointUnits& operator=(const PointUnits&) = delete;
    PointUnits& operator=(PointUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the X and Y factors to convert from pixels to points. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in points.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in points/pixels.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;
};


/// Provides linear measurements in picas.
///
class PicaUnits : public LinearUnits {

public:
    static constexpr double k_pcPerIn { 6.0 };

    /// Constructs pica units.
    ///
    /// @param[in] screenInfoProvider Information about display screens
    ///
    explicit PicaUnits(const ScreenInfoProvider& screenInfoProvider);

    ~PicaUnits() override = default;

    PicaUnits(const PicaUnits&) = default;
    PicaUnits(PicaUnits&&) = default;
    PicaUnits& operator=(const PicaUnits&) = delete;
    PicaUnits& operator=(PicaUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the X and Y factors to convert from pixels to picas. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in picas.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in picas/pixels.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;
};


/// Provides linear measurements in twips.
///
class TwipUnits : public LinearUnits {

public:
    static constexpr double k_tpPerIn { 1440.0 };

    /// Constructs twips units.
    ///
    /// @param[in] screenInfoProvider Screen information provider
    ///
    explicit TwipUnits(const ScreenInfoProvider& screenInfoProvider);

    ~TwipUnits() override = default;

    TwipUnits(const TwipUnits&) = default;
    TwipUnits(TwipUnits&&) = default;
    TwipUnits& operator=(const TwipUnits&) = delete;
    TwipUnits& operator=(TwipUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the X and Y factors to convert from pixels to twips. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in twips.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in twips/pixels.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;
};


/// Provides linear measurements in inches.
///
class InchUnits : public LinearUnits {

public:
    /// Constructs inch units.
    ///
    /// @param[in] screenInfoProvider Screen information provider
    ///
    explicit InchUnits(const ScreenInfoProvider& screenInfoProvider);

    ~InchUnits() override = default;

    InchUnits(const InchUnits&) = default;
    InchUnits(InchUnits&&) = default;
    InchUnits& operator=(const InchUnits&) = delete;
    InchUnits& operator=(InchUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the X and Y factors to convert from pixels to inches. In other words, multiplying the values returned
    /// from this method by pixels results in a value expressed in inches.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in inches/pixels (i.e. the inverse of the resolution).
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;
};


/// Provides linear measurements in centimeters.
///
class CentimeterUnits : public LinearUnits {

public:
    static constexpr double k_cmPerIn { 2.54 };

    /// Constructs centimeter units.
    ///
    /// @param[in] screenInfoProvider Screen information provider
    ///
    explicit CentimeterUnits(const ScreenInfoProvider& screenInfoProvider);

    ~CentimeterUnits() override = default;

    CentimeterUnits(const CentimeterUnits&) = default;
    CentimeterUnits(CentimeterUnits&&) = default;
    CentimeterUnits& operator=(const CentimeterUnits&) = delete;
    CentimeterUnits& operator=(CentimeterUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the X and Y factors to convert from pixels to centimeters. In other words, multiplying the values
    /// returned from this method by pixels results in a value expressed in centimeters.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in centimeters/pixels.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;
};


/// Provides linear measurements in millimeters.
///
class MillimeterUnits : public LinearUnits {

public:
    static constexpr double k_mmPerIn { 25.4 };

    /// Constructs millimeter units.
    ///
    /// @param[in] screenInfoProvider Screen information provider
    ///
    explicit MillimeterUnits(const ScreenInfoProvider& screenInfoProvider);

    ~MillimeterUnits() override = default;

    MillimeterUnits(const MillimeterUnits&) = default;
    MillimeterUnits(MillimeterUnits&&) = default;
    MillimeterUnits& operator=(const MillimeterUnits&) = delete;
    MillimeterUnits& operator=(MillimeterUnits&&) = delete;

    [[nodiscard]] QString getLengthLabel() const override;

    /// Returns the X and Y factors to convert from pixels to millimeters. In other words, multiplying the values
    /// returned from this method by pixels results in a value expressed in millimeters.
    ///
    /// @param[in] res Screen resolution, in pixels/inch.
    ///
    /// @return X and Y conversion factors, in millimeters/pixels.
    ///
    [[nodiscard]] QSizeF fromPixels(const QSizeF& res) const override;
};
