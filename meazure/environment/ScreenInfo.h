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

#include "ScreenInfoProvider.h"
#include <meazure/profile/Profile.h>
#include <QList>
#include <QScreen>
#include <vector>


class App;


class ScreenInfo : public ScreenInfoProvider {

public:
    static constexpr bool k_defUseManualRes {false };  ///< Use manual resolution by default.
    static constexpr bool k_defCalInInches {true };    ///< Calibrate in inches by default.

    ~ScreenInfo() override;

    ScreenInfo(const ScreenInfo&) = delete;
    ScreenInfo(ScreenInfo&&) = delete;
    ScreenInfo& operator=(const ScreenInfo&) = delete;

    /// Persists the state of the manager to the specified profile object.
    ///
    /// @param[in] profile Profile object into which the manager state is persisted.
    ///
    void saveProfile(Profile& profile) const;

    /// Restore the state of the manager from the specified profile object.
    ///
    /// @param[in] profile Profile object from which the manager state is restored.
    ///
    void loadProfile(Profile& profile);

    /// Resets the screen manager to its default state.
    ///
    void masterReset() const;

    [[nodiscard]] int getNumScreens() const override;

    [[nodiscard]] int screenForPoint(const QPoint& point) const override;

    [[nodiscard]] int screenForRect(const QRect& rect) const override;

    [[nodiscard]] int screenForWindow(const QWidget* wnd) const override;

    [[nodiscard]] QPoint getCenter() const override;

    [[nodiscard]] QPoint getOffScreen() const override;

    [[nodiscard]] QRect getVirtualRect() const override;

    [[nodiscard]] QRect getScreenRect(int screenIndex) const override;

    void getScreenRes(int screenIndex, bool& useManualRes, QSizeF& manualRes) const override;

    [[nodiscard]] QSizeF getScreenRes(int screenIndex) const override;

    /// Sets the resolution for the screen pointed to by the specified iterator.
    ///
    /// @param[in] screenIndex Screen whose resolution is to be set.
    /// @param[in] useManualRes true if the resolution is manually calibrated.
    /// @param[in] manualRes Manually calibrated screen resolution, in pixels.
    ///
    void setScreenRes(int screenIndex, bool useManualRes, const QSizeF* manualRes = nullptr) const;

    [[nodiscard]] bool isManualRes(int screenIndex) const override;

    [[nodiscard]] bool isCalibrationRequired() const override;

    [[nodiscard]] bool isPrimary(int screenIndex) const override;

    [[nodiscard]] QString getScreenName(int screenIndex) const override;

    [[nodiscard]] QSize getCursorSize(int screenIndex) const override;

    /// Indicates if the specified screen was calibrated in inches or centimeters.
    ///
    /// @param[in] screenIndex Index of the screen whose calibration units are desired.
    ///
    /// @return true if the resolution was calibrated in inches.
    ///
    [[nodiscard]] bool isCalInInches(int screenIndex) const;

    /// Specifies calibration units for the specified screen.
    ///
    /// @param[in] screenIndex Index of the screen whose calibration units are to be set.
    /// @param[in] calInInches true if the resolution is calibrated in inches.
    ///
    void setCalInInches(int screenIndex, bool calInInches);

    [[nodiscard]] bool sizeChanged() const override;

    [[nodiscard]] QPoint constrainPosition(const QPoint& point) const override;

private:
    class Screen;

    using Screens = std::vector<Screen*>;

    explicit ScreenInfo(const QList<QScreen*>& screens);

    [[nodiscard]] constexpr bool isValidScreen(int screenIndex) const {
        return (screenIndex >= 0 && screenIndex < m_numScreens);
    }

    int m_numScreens;
    Screens m_screens;
    QRect m_virtualGeometry;
    bool m_sizeChanged { false };   ///< Virtual screen rectangle changed since last run.

    friend class App;
};
