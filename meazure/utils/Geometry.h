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

#include <QRect>
#include <QPoint>
#include <vector>
#include <cstddef>
#include <cmath>
#include <limits>


/// Convenience methods for calculating various geometric values (e.g. area, circumference, angle).
///
namespace Geometry {

    /// Calculates the area of the specified rectangle using the formula:
    /// \f[
    ///     area=width*height
    /// \f]
    /// @param[in] rect Rectangle whose area is to be calculated
    ///
    /// @return Area of the rectangle or 0 if the rectangle is null or invalid.
    ///
    constexpr inline int area(const QRect& rect) {
        return (rect.isNull() || !rect.isValid()) ? 0 : rect.width() * rect.height();
    }

    /// Calculates the distance between a point and a rectangle.
    ///
    /// @param[in] rect Rectangle to which the distance should be calculated
    /// @param[in] point Point from which to calculate the distance to the rectangle
    /// @return Distance from the point to the rectangle. Returns 0 if the point is contained within the rectangle.
    ///         Returns the maximum double value if the rectangle is null or invalid.
    ///
    inline double distance(const QRect& rect, const QPoint point) {
        if (rect.isNull() || !rect.isValid()) {
            return std::numeric_limits<double>::max();
        }

        if (point.x() < rect.left()) {
            if (point.y() < rect.top()) {
                return std::hypot(rect.left() - point.x(), rect.top() - point.y());
            }
            if (point.y() <= rect.bottom()) {
                return rect.left() - point.x();
            }
            return std::hypot(rect.left() - point.x(), rect.bottom() - point.y());
        }

        if (point.x() <= rect.right()) {
            if (point.y() < rect.top()) {
                return rect.top() - point.y();
            }
            if (point.y() <= rect.bottom()) {
                return 0.0;
            }
            return point.y() - rect.bottom();
        }

        if (point.y() < rect.top()) {
            return std::hypot(rect.right() - point.x(), rect.top() - point.y());
        }
        if (point.y() <= rect.bottom()) {
            return point.x() - rect.right();
        }
        return std::hypot(rect.right() - point.x(), rect.bottom() - point.y());
    }

    /// Determines whether one of the specified rectangles contains the specified point.
    ///
    /// @tparam Type derived from QRect
    /// @param[in] rects Rectangles to search for containment
    /// @param point Point to test
    /// @return The index in the list of the rectangle containing the specified point. If the rectangles in the list
    ///     overlap, the first containing rectangle is returned. Returns -1 if the point is not contained in any
    ///     rectangle.
    ///
    template<class RECT>
    inline int contains(const std::vector<RECT*>& rects, const QPoint& point) {
        const std::size_t size = rects.size();
        for (std::size_t i = 0; i < size; i++) {
            const QRect* r = rects[i];
            if (r->contains(point, false)) {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    /// Determines whether one of the specified list of rectangles contains the specified rectangle. Containment is
    /// based on intersection area. The rectangle in the list with the highest containment area of the rectangle to
    /// test is considered the best match.
    ///
    /// @tparam Type derived from QRect
    /// @param[in] rects Rectangles to search for containment
    /// @param[in] rect Rectangle to test for containment
    /// @return The index in the list of the rectangle containing the specified rectangle. Returns -1 if the
    ///     rectangle is not contained in any rectangle.
    ///
    template<class RECT>
    inline int contains(const std::vector<RECT*>& rects, const QRect& rect) {
        int maxArea = 0;
        int bestIndex = -1;

        const std::size_t size = rects.size();
        for (std::size_t i = 0; i < size; i++) {
            const QRect* r = rects[i];
            const QRect intersection = r->intersected(rect);
            const int intersectionArea = area(intersection);
            if (intersectionArea > maxArea) {
                maxArea = intersectionArea;
                bestIndex = static_cast<int>(i);
            }
        }

        return bestIndex;
    }

    /// Finds the rectangle closest to the specified point.
    ///
    /// @tparam RECT Type derived from QRect
    /// @param[in] rects Rectangles to search
    /// @param[in] point Point for which closest rectangle is desired
    /// @return The index in the list of the rectangle to which the point is closest. Returns -1 if the list of
    ///         rectangles is empty.
    ///
    template<class RECT>
    inline int closest(const std::vector<RECT*>& rects, const QPoint& point) {
        double minDistance = std::numeric_limits<double>::max();
        int bestIndex = -1;

        const std::size_t size = rects.size();
        for (std::size_t i = 0; i < size; i++) {
            const QRect* r = rects[i];
            const double d = distance(*r, point);
            if (d < minDistance) {
                minDistance = d;
                bestIndex = static_cast<int>(i);
            }
        }

        return bestIndex;
    }
}
