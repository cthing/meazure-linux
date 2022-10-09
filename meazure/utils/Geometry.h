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


/// Convenience methods for calculating various geometric values (e.g. area, circumference, angle).
///
namespace Geometry {

    /// Calculates the area of the specified rectangle using the formula:
    /// \f[
    ///     area=width*height
    /// \f]
    /// @param[in] rect Rectangle whose area is to be calculated
    ///
    /// @return Area of the rectangle.
    ///
    constexpr int area(const QRect& rect) {
        return (rect.isNull() || !rect.isValid()) ? 0 : rect.width() * rect.height();
    }

    /// Determines whether one of the specified rectangles contains the specified point.
    ///
    /// @param[in] rects Rectangles to search for containment
    /// @param point Point to test
    /// @return The index in the list of the rectangle containing the specified point. If the rectangles in the list
    ///     overlap, the first containing rectangle is returned. Returns -1 if the point is not contained in any
    ///     rectangle.
    ///
    inline int contains(const std::vector<QRect>& rects, const QPoint& point) {
        for (std::size_t i = 0; i < rects.size(); i++) {
            if (rects[i].contains(point, false)) {
                return static_cast<int>(i);
            }
        }

        return -1;
    }

    /// Determines whether one of the specified list of rectangles contains the specified rectangle. Containment is
    /// based on intersection area. The rectangle in the list with the highest containment area of the rectangle to
    /// test is considered the best match.
    ///
    /// @param[in] rects Rectangles to search for containment
    /// @param[in] rect Rectangle to test for containment
    /// @return The index in the list of the rectangle containing the specified rectangle. Returns -1 if the
    ///     rectangle is not contained in any rectangle.
    ///
    inline int contains(const std::vector<QRect>& rects, const QRect& rect) {
        int maxArea = 0;
        int bestIndex = -1;

        for (std::size_t i = 0; i < rects.size(); i++) {
            const QRect intersection = rects[i].intersected(rect);
            const int intersectionArea = area(intersection);
            if (intersectionArea > maxArea) {
                maxArea = intersectionArea;
                bestIndex = static_cast<int>(i);
            }
        }

        return bestIndex;
    }
}
