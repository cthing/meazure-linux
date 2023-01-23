/*
 * Copyright 2023 C Thing Software
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

#include "MockScreenInfoProvider.h"
#include <meazure/units/UnitsProvider.h>
#include <meazure/units/Units.h>
#include <meazure/units/CustomUnits.h>
#include <QPoint>
#include <QSizeF>


class MockUnitsProvider : public UnitsProvider {

public:
    explicit MockUnitsProvider(const MockScreenInfoProvider* screenProvider) :
            m_linearUnits(new PixelUnits(screenProvider)),
            m_angularUnits(new DegreeUnits()),
            m_customUnits(new CustomUnits(screenProvider)),
            m_origin(0, 0)
    {
        m_customUnits->setName("MockCustomUnits");
        m_customUnits->setAbbrev("mcu");
        m_customUnits->setScaleBasis(CustomUnits::ScaleBasis::PixelBasis);
        m_customUnits->setScaleFactor(1.5);
    }

    ~MockUnitsProvider() override {
        delete m_linearUnits;
        delete m_angularUnits;
        delete m_customUnits;
    }

    MockUnitsProvider(const MockUnitsProvider&) = delete;
    MockUnitsProvider(MockUnitsProvider&&) = delete;
    MockUnitsProvider& operator=(const MockUnitsProvider&) = delete;

    [[nodiscard]] LinearUnitsId getLinearUnitsId() const override {
        return m_linearUnits->getUnitsId();
    }

    [[nodiscard]] LinearUnits* getLinearUnits() const override {
        return m_linearUnits;
    }

    [[nodiscard]] LinearUnits* getLinearUnits(LinearUnitsId) const override {
        return m_linearUnits;
    }

    [[nodiscard]] LinearUnits* getLinearUnits(const QString&) const override {
        return m_linearUnits;
    }

    [[nodiscard]] AngularUnitsId getAngularUnitsId() const override {
        return m_angularUnits->getUnitsId();
    }

    [[nodiscard]] AngularUnits* getAngularUnits() const override {
        return m_angularUnits;
    }

    [[nodiscard]] AngularUnits* getAngularUnits(AngularUnitsId) const override {
        return m_angularUnits;
    }

    [[nodiscard]] AngularUnits* getAngularUnits(const QString&) const override {
        return m_angularUnits;
    }

    [[nodiscard]] CustomUnits* getCustomUnits() override {
        return m_customUnits;
    }

    [[nodiscard]] const CustomUnits* getCustomUnits() const override {
        return m_customUnits;
    }

    [[nodiscard]] bool isInvertY() const override {
        return false;
    }

    [[nodiscard]] QPoint getOrigin() const override {
        return m_origin;
    }

    [[nodiscard]] QSizeF getWidthHeight(const QPoint& p1, const QPoint& p2) const override {
        QPoint np1 = p1;
        QPoint np2 = p2;

        if (np1.x() < np2.x()) {
            np2.rx()++;
        } else {
            np1.rx()++;
        }

        if (np1.y() < np2.y()) {
            np2.ry()++;
        } else {
            np1.ry()++;
        }

        return { std::fabs(np1.x() - np2.x()), std::fabs(np1.y() - np2.y()) };
    }

    [[nodiscard]] QString formatConvertAngle(double angle) const override {
        return format(Angle, convertAngle(angle));
    }

    [[nodiscard]] QString format(LinearMeasurementId id, double value) const override {
        return m_linearUnits->format(id, value);
    }

    [[nodiscard]] QString format(AngularMeasurementId id, double value) const override {
        return m_angularUnits->format(id, value);
    }

    [[nodiscard]] QPointF convertCoord(const QPoint& pos) const override {
        return m_linearUnits->convertCoord(pos);
    }

    [[nodiscard]] QPointF convertPos(const QPoint& pos) const override {
        return m_linearUnits->convertPos(pos);
    }

    [[nodiscard]] double unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos) const override {
        return m_linearUnits->unconvertCoord(dir, wnd, pos);
    }

    [[nodiscard]] QPoint unconvertCoord(const QPointF& pos) const override {
        return m_linearUnits->unconvertCoord(pos);
    }

    [[nodiscard]] QPoint unconvertPos(const QPointF& pos) const override {
        return m_linearUnits->unconvertPos(pos);
    }

    [[nodiscard]] QSizeF convertRes(const QSizeF& res) const override {
        return m_linearUnits->convertRes(res);
    }

    [[nodiscard]] QSize convertToPixels(LinearUnitsId, const QSizeF& res, double value, int minPixels) const override {
        return m_linearUnits->convertToPixels(res, value, minPixels);
    }

    [[nodiscard]] double convertAngle(double angle) const override {
        return m_angularUnits->convertAngle(angle);
    }

    [[nodiscard]] int getMajorTickCount() const override {
        return 10;
    }

    [[nodiscard]] QSizeF getMinorTickIncr(const QRect&) const override {
        return { 2.0, 2.0 };
    }

private:
    LinearUnits* m_linearUnits;
    AngularUnits* m_angularUnits;
    CustomUnits* m_customUnits;
    QPoint m_origin;
};
