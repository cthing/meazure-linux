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

#include <meazure/utils/MathUtils.h>
#include <QString>
#include <QPointF>
#include <QSizeF>


/// Position log measurement tool data holder.
///
class PosLogToolData {

public:
    [[nodiscard]] const QPointF& getPoint1() const {
        return m_point1;
    }

    void setPoint1(const QPointF& coord) {
        m_point1 = coord;
    }

    [[nodiscard]] const QPointF& getPoint2() const {
        return m_point2;
    }

    void setPoint2(const QPointF& coord) {
        m_point2 = coord;
    }

    [[nodiscard]] const QPointF& getPointV() const {
        return m_pointV;
    }

    void setPointV(const QPointF& coord) {
        m_pointV = coord;
    }

    [[nodiscard]] const QSizeF& getWidthHeight() const {
        return m_widthHeight;
    }

    void setWidthHeight(const QSizeF& widthHeight) {
        m_widthHeight = widthHeight;
    }

    [[nodiscard]] double getDistance() const {
        return m_distance;
    }

    void setDistance(double distance) {
        m_distance = distance;
    }

    [[nodiscard]] double getAngle() const {
        return m_angle;
    }

    void setAngle(double angle) {
        m_angle = angle;
    }

    [[nodiscard]] double getArea() const {
        return m_area;
    }

    void setArea(double area) {
        m_area = area;
    }

    bool operator==(const PosLogToolData &rhs) const {
        return m_point1 == rhs.m_point1 &&
               m_point2 == rhs.m_point2 &&
               m_pointV == rhs.m_pointV &&
               m_widthHeight == rhs.m_widthHeight &&
               MathUtils::fuzzyEqual(m_distance, rhs.m_distance) &&
               MathUtils::fuzzyEqual(m_angle, rhs.m_angle) &&
               MathUtils::fuzzyEqual(m_area, rhs.m_area);
    }

    bool operator!=(const PosLogToolData &rhs) const {
        return !(rhs == *this);
    }

private:
    QPointF m_point1;
    QPointF m_point2;
    QPointF m_pointV;
    QSizeF m_widthHeight;
    double m_distance { 0.0 };
    double m_angle { 0.0 };
    double m_area { 0.0 };
};
