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

#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/utils/Geometry.h>
#include <QRect>
#include <QPoint>
#include <QSizeF>


class MockScreenInfoProvider : public ScreenInfoProvider {

public:
    MockScreenInfoProvider() : m_rect(0, 0, 1280, 1024) {
        m_center = m_rect.center();
        m_res.setWidth(96.0);
        m_res.setHeight(96.0);
    }

    [[nodiscard]] int getNumScreens() const override {
        return 1;
    }

    [[nodiscard]] int screenForPoint(const QPoint&) const override {
        return 0;
    }

    [[nodiscard]] int screenForRect(const QRect&) const override {
        return 0;
    }

    [[nodiscard]] int screenForWindow(const QWidget*) const override {
        return 0;
    }

    [[nodiscard]] QPoint getCenter() const override {
        return m_center;
    }

    [[nodiscard]] QRect getVirtualRect() const override {
        return m_rect;
    }

    [[nodiscard]] QRect getAvailableVirtualRect() const override {
        return m_rect;
    }

    [[nodiscard]] QRect getScreenRect(int /* screenIndex */) const override {
        return m_rect;
    }

    void getScreenRes(int /* screenIndex */, bool& useManualRes, QSizeF& manualRes) const override {
        useManualRes = false;
        manualRes = m_res;
    }

    [[nodiscard]] QSizeF getScreenRes(int /* screenIndex */) const override {
        return m_res;
    }

    [[nodiscard]] bool isManualRes(int /* screenIndex */) const override {
        return false;
    }

    [[nodiscard]] bool isCalibrationRequired() const override {
        return true;
    }

    [[nodiscard]] bool isPrimary(int /* screenIndex */) const override {
        return true;
    }

    [[nodiscard]] QString getScreenName(int /* screenIndex */) const override {
        return "MockScreen";
    }

    [[nodiscard]] QSize getCursorSize(int /* screenIndex */) const override {
        return { 24, 24 };
    }

    [[nodiscard]] bool sizeChanged() const override {
        return false;
    }

    [[nodiscard]] QPoint constrainPosition(const QPoint& pt) const override {
        return Geometry::constrain(&m_rect, pt);
    }

    [[nodiscard]] QImage grabScreen(int, int, int, int) const override {
        return {};
    }

private:
    QRect m_rect;
    QPoint m_center;
    QSizeF m_res;
};
