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

#include <QWidget>
#include <QPoint>
#include <QRect>
#include <QSizeF>
#include <QImage>


struct ScreenInfoProvider {

    ScreenInfoProvider() = default;
    virtual ~ScreenInfoProvider() = default;

    ScreenInfoProvider(const ScreenInfoProvider&) = delete;
    ScreenInfoProvider(ScreenInfoProvider&&) = delete;
    ScreenInfoProvider& operator=(const ScreenInfoProvider&) = delete;

    /// Returns the number of display screens attached to the system.
    ///
    /// @return Number of display screens attached to the system.
    ///
    [[nodiscard]] virtual int getNumScreens() const = 0;

    /// Obtains the screen that contains the specified point.
    ///
    /// @param[in] point Point whose screen is desired.
    /// @return Index of the screen containing the specified point or -1 if the specified point is outside any screen.
    ///
    [[nodiscard]] virtual int screenForPoint(const QPoint& point) const = 0;

    /// Obtains the screen that contains the specified rectangle. The screen containing the majority of the area of
    /// the specified rectangle is returned.
    ///
    /// @param[in] rect Rectangle whose screen is desired.
    /// @return Index of the screen containing the specified rectangle or -1 if the rectangle is outside any screen.
    ///
    [[nodiscard]] virtual int screenForRect(const QRect& rect) const = 0;

    /// Obtains the screen that contains the specified window. The screen containing the majority of the area of
    /// the specified widget is returned.
    ///
    /// @param[in] wnd Window whose screen is desired.
    /// @return Index of the screen containing the specified window or -1 if the specified window is outside any screen.
    ///
    [[nodiscard]] virtual int screenForWindow(const QWidget* wnd) const = 0;

    /// Determines the center of the screen that contains the app's main window.
    ///
    /// @return Center point of the screen containing the app's main window.
    ///
    [[nodiscard]] virtual QPoint getCenter() const = 0;

    /// Returns the virtual screen rectangle. The virtual rectangle is the rectangle containing all display screens
    /// attached to the system.
    ///
    /// @return Rectangle encompassing all display screens attached to the system, in pixels.
    ///
    [[nodiscard]] virtual QRect getVirtualRect() const = 0;

    /// Returns the usable virtual screen rectangle. The usable virtual rectangle is the rectangle containing the
    /// usable area of all display screens attached to the system. The usable area is the rectangle not used by the
    /// window manager.
    ///
    /// @return Rectangle encompassing the usable area of all display screens attached to the system, in pixels.
    ///
    [[nodiscard]] virtual QRect getAvailableVirtualRect() const = 0;

    /// Returns the rectangle for the specified screen.
    ///
    /// @param[in] screenIndex Screen whose rectangle is desired.
    /// @return Screen rectangle, in pixels.
    ///
    [[nodiscard]] virtual QRect getScreenRect(int screenIndex) const = 0;

    /// Returns the manually set screen resolution for the screen pointed to by the specified iterator.
    ///
    /// @param[in] screenIndex Screen whose manual resolution is desired.
    /// @param[out] useManualRes true if the resolution was set manually.
    /// @param[out] manualRes Manually set resolution, in pixels per inch.
    ///
    virtual void getScreenRes(int screenIndex, bool& useManualRes, QSizeF& manualRes) const = 0;

    /// Returns the resolution for the specified screen.
    ///
    /// @param[in] screenIndex Screen whose resolution is desired.
    /// @return Screen resolution, in pixels per inch. If the resolution has been set manually, it is returned.
    ///         Otherwise, the operating system reported resolution is returned.
    ///
    [[nodiscard]] virtual QSizeF getScreenRes(int screenIndex) const = 0;

    /// Indicates whether the resolution for the specified screen has been set manually.
    ///
    /// @param[in] screenIndex Screen whose resolution setting is being queried.
    /// @return true if the screen's resolution was set manually.
    ///
    [[nodiscard]] virtual bool isManualRes(int screenIndex) const = 0;

    /// Indicates whether any screen is using the operating system reported resolution. In other words, are there
    /// any screens that require calibration.
    ///
    /// @return true if there are any screens requiring manual calibration.
    ///
    [[nodiscard]] virtual bool isCalibrationRequired() const = 0;

    /// Indicates if the specified screen is the primary.
    ///
    /// @param[in] screenIndex Screen to test for primary.
    /// @return true if the specified screen is the primary.
    ///
    [[nodiscard]] virtual bool isPrimary(int screenIndex) const = 0;

    /// Returns the name for the specified screen.
    ///
    /// @param[in] screenIndex Screen whose name is desired.
    /// @return Name of the specified screen.
    ///
    [[nodiscard]] virtual QString getScreenName(int screenIndex) const = 0;

    /// Provides the size of the system cursor.
    ///
    /// @param[in] screenIndex Screen whose cursor size is desired.
    /// @return Size of the system cursor
    ///
    [[nodiscard]] virtual QSize getCursorSize(int screenIndex) const = 0;

    /// Indicates whether the virtual screen rectangle has changed since the last time the application was run. If
    /// the virtual screen size has changed, re-calibration is recommended.
    ///
    /// @return true if the virtual screen rectangle has changed.
    ///
    [[nodiscard]] virtual bool sizeChanged() const = 0;

    /// Ensures that the specified point is on a screen.
    ///
    /// @param[in] point Point to check.
    /// @return New point based on the specified point that is guaranteed to be on a screen.
    ///
    [[nodiscard]] virtual QPoint constrainPosition(const QPoint& point) const = 0;

    /// Grabs the specified portion of the screen.
    ///
    /// @param[in] x Left corner of the rectangle to grab
    /// @param[in] y Upper corner of the rectangle to grab
    /// @param[in] width Width of the rectangle to grab
    /// @param[in] height Height of the rectangle to grab
    /// @return Image of the screen contents in the specified rectangle
    ///
    [[nodiscard]] virtual QImage grabScreen(int x, int y, int width, int height) const = 0;
};
