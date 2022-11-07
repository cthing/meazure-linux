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

#include "UnitsProvider.h"
#include "Units.h"
#include "CustomUnits.h"
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/profile/Profile.h>
#include <map>


class UnitsMgr : public QObject, public UnitsProvider {

    Q_OBJECT

public:
    static constexpr bool k_defHaveWarned { false };            ///< Indicates whether the user has already been
                                                                ///< warned about using the operating system
                                                                ///< reported resolution.
    static constexpr const char* k_defLinearUnits { "px" };     ///< Default units for linear measurements.
    static constexpr const char* k_defAngularUnits { "deg" };   ///< Default units for angular measurements.
    static constexpr bool k_defInvertY { false };               ///< Default orientation of the y-axis.
    static constexpr bool k_defSupplmentalAngle { false };      ///< Default angle to show


    ~UnitsMgr() override = default;

    UnitsMgr(const UnitsMgr&) = delete;
    UnitsMgr(UnitsMgr&&) = delete;
    UnitsMgr& operator=(const UnitsMgr&) = delete;

    /// Persists the state of the units manager to the specified profile object.
    ///
    /// @param[in] profile Profile object into which the manager state is persisted.
    ///
    void saveProfile(Profile& profile) const;

    /// Restores the state of the units manager from the specified profile object.
    ///
    /// @param[in] profile Profile object from which the manager state is restored.
    ///
    void loadProfile(Profile& profile);

    /// Called when the master application reset is initiated. The units manager is reset to its default state.
    ///
    void masterReset();

    /// Sets the current linear measurement units based on the specified units identifier.
    ///
    /// @param[in] unitsId Measurement units identifier.
    ///
    void setLinearUnits(LinearUnitsId unitsId);

    /// Sets the current linear measurement units based on the specified units identifier string (e.g. "in").
    ///
    /// @param[in] unitsStr Measurement units identifier string (e.g. "in").
    ///
    void setLinearUnits(const QString& unitsStr);

    [[nodiscard]] LinearUnits* getLinearUnits() const override {
        return m_currentLinearUnits;
    }

    /// Returns the identifier for the current linear measurement units.
    ///
    /// @return Identifier for the current linear measurement units.
    ///
    [[nodiscard]] LinearUnitsId getLinearUnitsId() const {
        return m_currentLinearUnits->getUnitsId();
    }

    /// Returns the identifier string for the current linear measurement units (e.g. "px").
    ///
    /// @return Identifier string for the current linear measurement units.
    ///
    [[nodiscard]] QString getLinearUnitsStr() const {
        return m_currentLinearUnits->getUnitsStr();
    }

    [[nodiscard]] LinearUnits* getLinearUnits(LinearUnitsId unitsId) const override {
        return m_linearUnitsMap.at(unitsId);
    }

    [[nodiscard]] LinearUnits* getLinearUnits(const QString& unitsStr) const override {
        for (const auto& unitsEntry : m_linearUnitsMap) {
            if (unitsEntry.second->getUnitsStr() == unitsStr) {
                return unitsEntry.second;
            }
        }
        return nullptr;
    }

    /// Sets the current angular measurement units based on the specified units identifier.
    ///
    /// @param[in] unitsId Measurement units identifier.
    ///
    void setAngularUnits(AngularUnitsId unitsId);

    /// Sets the current angular measurement units based on the specified units identifier string (e.g. "deg").
    ///
    /// @param[in] unitsStr Measurement units identifier.
    ///
    void setAngularUnits(const QString& unitsStr);

    [[nodiscard]] AngularUnits* getAngularUnits() const override {
        return m_currentAngularUnits;
    }

    /// Returns the identifier for the current angular measurement units.
    ///
    /// @return Identifier for the current angular measurement units.
    ///
    [[nodiscard]] AngularUnitsId getAngularUnitsId() const {
        return m_currentAngularUnits->getUnitsId();
    }

    /// Returns the identifier string for the current angular measurement units (e.g. "deg").
    ///
    /// @return Identifier string for the current angular measurement units.
    ///
    [[nodiscard]] QString getAngularUnitsStr() const {
        return m_currentAngularUnits->getUnitsStr();
    }

    [[nodiscard]] AngularUnits* getAngularUnits(AngularUnitsId unitsId) const override {
        return m_angularUnitsMap.at(unitsId);
    }

    [[nodiscard]] AngularUnits* getAngularUnits(const QString& unitsStr) const override {
        for (const auto& unitsEntry : m_angularUnitsMap) {
            if (unitsEntry.second->getUnitsStr() == unitsStr) {
                return unitsEntry.second;
            }
        }
        return nullptr;
    }

    /// Returns the user defined units object. This is not necessarily the current measurement units object.
    ///
    /// @return User defined measurement units object.
    ///
    CustomUnits* getCustomUnits() {
        return &m_customUnits;
    }

    /// Set the number of decimal places to show on the data display for all types of linear measurements in the
    /// specified units (e.g. coordinates, length, area, etc.).
    ///
    /// @param[in] unitsId Identifier for the units whose precisions are to be set.
    /// @param[in] precisions Array of decimal places for each measurement type.
    ///
    void setLinearDisplayPrecisions(LinearUnitsId unitsId, const Units::DisplayPrecisions& precisions) {
        m_linearUnitsMap.at(unitsId)->setDisplayPrecisions(precisions);
    }

    /// Returns the array of decimal places currently set for each linear measurement type in the specified units.
    ///
    /// @param[in] unitsId Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places for each linear measurement type.
    ///
    [[nodiscard]] const Units::DisplayPrecisions& getLinearDisplayPrecisions(LinearUnitsId unitsId) const {
        return m_linearUnitsMap.at(unitsId)->getDisplayPrecisions();
    }

    /// Returns the array of default decimal places for each linear measurement type in the specified units.
    ///
    /// @param[in] unitsId Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places for each linear measurement type.
    ///
    [[nodiscard]] const Units::DisplayPrecisions& getLinearDefaultPrecisions(LinearUnitsId unitsId) const {
        return m_linearUnitsMap.at(unitsId)->getDefaultPrecisions();
    }

    /// Set the number of decimal places to show on the data display for angular measurements in the specified units.
    ///
    /// @param[in] unitsId Identifier for the units whose precisions are to be set.
    /// @param[in] precisions Array of decimal places.
    ///
    void setAngularDisplayPrecisions(AngularUnitsId unitsId, const Units::DisplayPrecisions& precisions) {
        m_angularUnitsMap.at(unitsId)->setDisplayPrecisions(precisions);
    }

    /// Returns the array of decimal places currently set for the specified angular measurement units.
    ///
    /// @param[in] unitsId Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places.
    ///
    [[nodiscard]] const Units::DisplayPrecisions& getAngularDisplayPrecisions(AngularUnitsId unitsId) const {
        return m_angularUnitsMap.at(unitsId)->getDisplayPrecisions();
    }

    /// Returns the array of default decimal places for the specified angular measurement units.
    ///
    /// @param[in] unitsId Identifier for the units whose precisions are desired.
    ///
    /// @return Array of decimal places.
    ///
    [[nodiscard]] const Units::DisplayPrecisions& getAngularDefaultPrecisions(AngularUnitsId unitsId) const {
        return m_angularUnitsMap.at(unitsId)->getDefaultPrecisions();
    }

    /// By default, the X11 coordinate system is inverted from the typical Cartesian coordinate system in that
    /// the origin is located in the upper left corner of the primary screen and the positive y-axis points downward.
    /// This method sets whether the y-axis should be inverted so that the origin is moved to the lower left corner
    /// of the primary screen and the positive y-axis points up. Note that the actual location of the origin is
    /// determined by the setOrigin method.
    ///
    /// @param[in] invertY true inverts the y-axis, placing the origin at the lower left of the primary display
    ///         screen and making positive y pointing upward.
    ///
    static void setInvertY(bool invertY) {
        LinearUnits::setInvertY(invertY);
    }

    [[nodiscard]] bool isInvertY() const override {
        return LinearUnits::isInvertY();
    }

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
    /// @param[in] showSupplemental true to show supplemental angle instead of included angle
    ///
    static void setSupplementalAngle(bool showSupplemental) {
        AngularUnits::setSupplementalAngle(showSupplemental);
    }

    /// Indicates whether the included angle is shown (default) or the supplemental angle.
    ///
    /// @return true if the supplemental angle is shown.
    ///
    static bool isSupplementalAngle() {
        return AngularUnits::isSupplementalAngle();
    }

    /// Moves the origin of the coordinate system to the specified point. The orientation of the axes is not
    /// effected by this method. To change the orientation of the y-axis use the setInvertY method.
    ///
    /// @param[in] origin New location for the origin of the coordinate system, in pixels.
    ///
    static void setOrigin(const QPoint& origin) {
        LinearUnits::setOrigin(origin);
    }

    [[nodiscard]] QPoint getOrigin() const override {
        return LinearUnits::getOrigin();
    }

    [[nodiscard]] QSizeF getWidthHeight(const QPoint& p1, const QPoint& p2) const override;

    [[nodiscard]] QString formatConvertAngle(double angle) const override {
        return format(Angle, convertAngle(angle));
    }

    [[nodiscard]] QString format(LinearMeasurementId id, double value) const override {
        return m_currentLinearUnits->format(id, value);
    }

    [[nodiscard]] QString format(AngularMeasurementId id, double value) const override {
        return m_currentAngularUnits->format(id, value);
    }

    [[nodiscard]] QPointF convertCoord(const QPoint& pos) const override {
        return m_currentLinearUnits->convertCoord(pos);
    }

    [[nodiscard]] double unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos) const override {
        return m_currentLinearUnits->unconvertCoord(dir, wnd, pos);
    }

    /// Converts from the current units to pixels. The conversion takes into account the location of the origin
    /// and the orientation of the y-axis.
    ///
    /// @param[in] pos Coordinates to convert to pixels.
    ///
    /// @return Coordinates converted from the current units to pixels. The conversion takes into account the
    ///         location of the origin and the orientation of the y-axis.
    ///
    [[nodiscard]] QPoint unconvertCoord(const QPointF& pos) const {
        return m_currentLinearUnits->unconvertCoord(pos);
    }

    [[nodiscard]] QPointF convertPos(const QPoint& pos) const override {
        return m_currentLinearUnits->convertPos(pos);
    }

    [[nodiscard]] QPoint unconvertPos(const QPointF& pos) const override {
        return m_currentLinearUnits->unconvertPos(pos);
    }

    [[nodiscard]] QSizeF convertRes(const QSizeF& res) const override {
        return m_currentLinearUnits->convertRes(res);
    }

    [[nodiscard]] QSize convertToPixels(LinearUnitsId id, const QSizeF& res, double value,
                                        int minPixels) const override {
        return ((*m_linearUnitsMap.find(id)).second)->convertToPixels(res, value, minPixels);
    }

    [[nodiscard]] double convertAngle(double angle) const override {
        return m_currentAngularUnits->convertAngle(angle);
    }

    [[nodiscard]] int getMajorTickCount() const override {
        return m_majorTickCount;
    }

    [[nodiscard]] QSizeF getMinorIncr(const QRect& rect) const override;

signals:
    void linearUnitsChanged(LinearUnitsId unitsId);

    void angularUnitsChanged(AngularUnitsId unitsId);

    void calibrationRequired();

private:
    /// Ruler tick increments. The order of magnitude of these values is adjusted based on the units.
    static constexpr double k_tickIncrements[] {10.0, 25.0, 50.0, 75.0, 100.0 };

    /// Number of tick mark increments in the kTickIncrements array.
    static constexpr int k_numTickIncrements {sizeof(k_tickIncrements) / sizeof(*k_tickIncrements) };

    /// Minimum separation between ruler tick marks, in pixels.
    static constexpr int k_minSepPixels {5 };

    ///< Minimum separation between ruler tick marks, in inches.
    static constexpr double k_minSepInches {0.1 };


    using LinearUnitsMap = std::map<LinearUnitsId, LinearUnits*>;
    using AngularUnitsMap = std::map<AngularUnitsId, AngularUnits*>;

    explicit UnitsMgr(const ScreenInfoProvider& screenInfoProvider);

    const ScreenInfoProvider& m_screenInfoProvider;
    PixelUnits m_pixelUnits;
    PointUnits m_pointUnits;
    TwipUnits m_twipUnits;
    InchUnits m_inchUnits;
    CentimeterUnits m_cmUnits;
    MillimeterUnits m_mmUnits;
    PicaUnits m_picaUnits;
    CustomUnits m_customUnits;
    DegreeUnits m_degreeUnits;
    RadianUnits m_radianUnits;
    LinearUnits* m_currentLinearUnits;
    AngularUnits* m_currentAngularUnits;
    LinearUnitsMap m_linearUnitsMap;
    AngularUnitsMap m_angularUnitsMap;
    bool m_haveWarned {k_defHaveWarned };    ///< Indicates whether the user has already been warned about using
                                            ///< the operating system reported resolution.
    int m_majorTickCount { 10 };            ///< Number of minor ruler tick marks between major tick marks.

    friend class App;
    friend class UnitsMgrTest;
};