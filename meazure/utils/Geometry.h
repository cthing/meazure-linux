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
#include <QtMath>
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

    /// Returns the angular sector containing the vector specified by the two points. A circle centered at the
    /// starting point of the vector is divided into eight 45 degree sectors numbered -4 through 4 inclusive.
    /// The sectors are numbered according to the following table of angle ranges. Positive angles go clockwise
    /// from the positive x-axis.
    ///
    /// <table>
    /// <tr>
    ///     <th>Angle Range (deg)</th>
    ///     <th>Sector</th>
    /// </tr>
    /// <tr>
    ///     <td>[0, 45)</td>
    ///     <td>1</td>
    /// </tr>
    /// <tr>
    ///     <td>[45, 90)</td>
    ///     <td>2</td>
    /// </tr>
    /// <tr>
    ///     <td>[90, 135)</td>
    ///     <td>3</td>
    /// </tr>
    /// <tr>
    ///     <td>[135, 180)</td>
    ///     <td>4</td>
    /// </tr>
    /// <tr>
    ///     <td>[180, 225]</td>
    ///     <td>-4</td>
    /// </tr>
    /// <tr>
    ///     <td>(225, 270]</td>
    ///     <td>-3</td>
    /// </tr>
    /// <tr>
    ///     <td>(270, 315]</td>
    ///     <td>-2</td>
    /// </tr>
    /// <tr>
    ///     <td>(315, 0]</td>
    ///     <td>-1</td>
    /// </tr>
    /// <tr>
    ///     <td>Zero vector</td>
    ///     <td>0</td>
    /// </tr>
    /// </table>
    ///
    /// @param[in] start Starting point for the vector.
    /// @param[in] end Ending point for the vector.
    ///
    /// @return The circular sector corresponding to the vector in the range [-4.0, 4.0].
    ///
    inline int calcSector(const QPoint& start, const QPoint& end) {
        const QPoint delta = end - start;

        if (delta.isNull()) {
            return 0;
        }
        if (delta.x() == 0) {
            return (delta.y() < 0) ? -3 : 3;
        }
        if (delta.y() == 0) {
            return (delta.x() < 0) ? -4 : 1;
        }

        // Note: 45 degrees == PI/4 radians
        const double result = atan2(static_cast<double>(delta.y()), static_cast<double>(delta.x())) / M_PI_4;
        return static_cast<int>(result + std::copysign(1.0, result));
    }

    /// Indicates whether the specified vector is oriented in a vertical direction. A vector is considered
    /// vertically oriented if it is in sector +/-2 or +/-3. See the documentation for the CalcSector method.
    ///
    /// @param[in] start Starting point for the vector
    /// @param[in] end Ending point for the vector
    /// @return true if the specified vector is vertically oriented. Returns false if the vector is horizontally
    ///     oriented or is the empty vector.
    ///
    inline bool isVerticallyOriented(const QPoint& start, const QPoint& end) {
        switch (calcSector(start, end)) {
            case 2:
            case -2:
            case 3:
            case -3:
                return true;
            default:
                return false;
        }
    }

    /// Indicates whether the specified vector is oriented in a horizontal direction. A vector is considered
    /// horizontally oriented if it is in sector +/-1 or +/-4. See the documentation for the CalcSector method.
    ///
    /// @param[in] start Starting point for the vector
    /// @param[in] end Ending point for the vector
    /// @return true if the specified vector is horizontally oriented. Returns false if the vector is vertically
    ///     oriented or is the empty vector.
    ///
    inline bool isHorizontallyOriented(const QPoint& start, const QPoint& end) {
        switch (calcSector(start, end)) {
            case 1:
            case -1:
            case 4:
            case -4:
                return true;
            default:
                return false;
        }
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
            if (rects[i]->contains(point, false)) {
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
            const QRect intersection = rects[i]->intersected(rect);
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
            const double d = distance(*rects[i], point);
            if (d < minDistance) {
                minDistance = d;
                bestIndex = static_cast<int>(i);
            }
        }

        return bestIndex;
    }

    /// Constrains the specified point such that it is contained within the specified rectangle.
    ///
    /// @tparam RECT Type derived from QRect
    /// @param[in] rect Rectangle to constrain the specified point
    /// @param[in] point Point to be constrained into the rectangle
    /// @return Point constrained such that it is contained within the rectangle
    ///
    template<class RECT>
    inline QPoint constrain(RECT* rect, const QPoint& point) {
        const QRect* r = rect;
        if (r->isNull() || !r->isValid() || r->contains(point, false)) {
            return point;
        }

        QPoint constrainedPoint(point);

        if (constrainedPoint.x() < r->left()) {
            constrainedPoint.rx() = r->left();
        } else if (constrainedPoint.x() > r->right()) {
            constrainedPoint.rx() = r->right();
        }

        if (constrainedPoint.y() < r->top()) {
            constrainedPoint.ry() = r->top();
        } else if (constrainedPoint.y() > r->bottom()) {
            constrainedPoint.ry() = r->bottom();
        }

        return constrainedPoint;
    }

    /// Constrains the specified point such that it is contained within the closest rectangle.
    ///
    /// @tparam RECT Type derived from QRect
    /// @param[in] rects Rectangles to constrain the specified point
    /// @param[in] point Point to be constrained into the closest rectangle
    /// @return Point constrained such that it is contained within the closest rectangle
    ///
    template<class RECT>
    inline QPoint constrain(const std::vector<RECT*>& rects, const QPoint& point) {
        const int closestRectIndex = closest(rects, point);
        return (closestRectIndex == -1) ? point : constrain(rects[closestRectIndex], point);
    }
}
