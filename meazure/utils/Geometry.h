/*
 * Copyright 2023 C Thing Software
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "MathUtils.h"
#include <QRect>
#include <QPoint>
#include <QSizeF>
#include <QtMath>
#include <QTransform>
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
    /// @return Area of the rectangle or 0 if the rectangle is null or invalid.
    ///
    inline int area(const QRect& rect) {
        return (rect.isNull() || !rect.isValid()) ? 0 : rect.width() * rect.height();
    }

    /// Calculates the area of the specified rectangle using the formula:
    /// \f[
    ///     area=width*height
    /// \f]
    /// @param[in] wh Rectangle whose area is to be calculated
    /// @return Area of the rectangle or 0.0 if the rectangle is null or invalid.
    ///
    inline double area(const QSizeF& wh) {
        return (wh.isNull() || !wh.isValid()) ? 0.0 : wh.width() * wh.height();
    }

    /// Calculates the area of the circle with the specified radius.
    /// \f[
    ///     area=\pi r^2
    /// \f]
    ///
    /// @param[in] radius Radius of the circle
    /// @return Area of the circle with the specified radius.
    ///
    inline double area(double radius) {
        return M_PI * radius * radius;
    }

    /// Calculates the aspect ratio of the specified rectangle using the formula:
    /// \f[
    ///     aspect=width/height
    /// \f]
    /// @param[in] wh Rectangle whose aspect ratio is to be calculated
    /// @return Aspect ratio of the rectangle or 0.0 if the rectangle has zero width or height.
    ///
    inline double aspectRatio(const QSizeF& wh) {
        return wh.isEmpty() ? 0.0 : wh.width() / wh.height();
    }

    /// Calculates the hypotenuse of the right triangle formed by the specified sides.
    /// \f[
    ///     hypot=\sqrt{width^2 + height^2}
    /// \f]
    ///
    /// @param[in] wh Rectangle whose hypotenuse length is to be calculated
    /// @return Length of the hypotenuse of the right triangle with the specified sides.
    ///
    inline double hypot(const QSizeF& wh) {
        return wh.isEmpty() ? 0.0 : std::hypot(wh.width(), wh.height());
    }

    /// Calculates the distance between the two specified points.
    /// \f[
    ///     hypot=\sqrt{(p2.x-p1.x)^2 + (p2.y-p1.y)^2}
    /// \f]
    ///
    /// @param[in] p1 First point
    /// @param[in] p2 Second point
    /// @return Distance between the two points.
    ///
    inline double hypot(const QPoint& p1, const QPoint& p2) {
        return std::hypot(p2.x() - p1.x(), p2.y() - p1.y());
    }

    /// Calculates the distance between the two specified points.
    /// \f[
    ///     hypot=\sqrt{(p2.x-p1.x)^2 + (p2.y-p1.y)^2}
    /// \f]
    ///
    /// @param[in] p1 First point
    /// @param[in] p2 Second point
    /// @return Distance between the two points.
    ///
    inline double hypot(const QPointF& p1, const QPointF& p2) {
        return std::hypot(p2.x() - p1.x(), p2.y() - p1.y());
    }

    /// Returns the angle of the vector from the specified start point to the specified end point relative to
    /// the horizontal (x-axis), measured in radians. Using the default coordinate system (i.e. positive y-axis
    /// pointed down from the origin), positive angles are measured clockwise from the horizontal to the vector.
    /// The following formula is used:
    /// \f[
    ///     angle=atan2(end_y-start_y, end_x-start_x)
    /// \f]
    /// @param[in] start Starting point for the vector whose angle is to be determined.
    /// @param[in] end Ending point for the vector whose angle is to be determined.
    ///
    /// @return Angle between the x-axis and the line, in radians. Degenerate cases return 0.0.
    ///
    inline double angle(const QPointF& start, const QPointF& end) {
        const QPointF delta = end - start;

        if (MathUtils::fuzzyEqualZero(delta.x()) && MathUtils::fuzzyEqualZero(delta.y())) {
            return 0.0;
        }
        return std::atan2(delta.y(), delta.x());
    }

    /// Returns the angle, in radians, between the vector from the vertex to point p1 and the vector from the
    /// vertex to point p2. Using the default coordinate system (i.e. positive y-axis pointed down from the origin),
    /// positive angles are measured clockwise from the vertex-p1 line to the vertex-p2 line.
    ///
    /// @param[in] vertex Intersection point for the two lines.
    /// @param[in] p1 Forms the end point for the first line from the vertex.
    /// @param[in] p2 Forms the end point for the second line from the vertex.
    ///
    /// @return Angle between the vectors vertex-p1 and vertex-p2, in radians.
    ///
    inline double angle(const QPointF& vertex, const QPointF& p1, const QPointF& p2) {
        const QPointF delta1 = p1 - vertex;
        const QPointF delta2 = p2 - vertex;

        const double numer = delta2.y() * delta1.x() - delta1.y() * delta2.x();
        const double denom = delta2.x() * delta1.x() + delta1.y() * delta2.y();

        if (MathUtils::fuzzyEqualZero(numer) && MathUtils::fuzzyEqualZero(denom)) {
            return 0.0;
        }
        return std::atan2(numer, denom);
    }

    /// Normalizes the specified angle expressed in degrees such that it will always be in the range 0 <= angle < 360.
    ///
    /// @param[in] degrees Angle to normalize (can be negative)
    /// @return Angle in the range 0 <= angle < 360
    ///
    constexpr int normalizeDegrees(int degrees) {
        const int normalized = degrees % 360;
        return (normalized < 0) ? 360 + normalized : normalized;
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
    /// @param[in] rects Rectangles to scan for containment
    /// @param point Point to test
    /// @return The index in the list of the rectangle containing the specified point. If the rectangles in the list
    ///     overlap, the first containing rectangle is returned. Returns -1 if the point is not contained in any
    ///     rectangle.
    ///
    template<class RECT>
    inline int contains(const std::vector<RECT*>& rects, const QPoint& point) {
        const std::size_t size = rects.size();
        for (std::size_t i = 0; i < size; i++) {
            if (rects.at(i)->contains(point, false)) {
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
    /// @param[in] rects Rectangles to scan for containment
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
            const QRect intersection = rects.at(i)->intersected(rect);
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
    /// @param[in] rects Rectangles to scan
    /// @param[in] point Point for which closest rectangle is desired
    /// @return The index in the list of rectangles to which the point is closest. Returns -1 if the list of
    ///         rectangles is empty.
    ///
    template<class RECT>
    inline int closest(const std::vector<RECT*>& rects, const QPoint& point) {
        double minDistance = std::numeric_limits<double>::max();
        int bestIndex = -1;

        const std::size_t size = rects.size();
        for (std::size_t i = 0; i < size; i++) {
            const double d = distance(*rects.at(i), point);
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

    /// Creates a transform that performs a rotation by the specified angle around the specified point.
    ///
    /// @param angle Rotation angle in degrees
    /// @param xc X coordinate of the center of rotation
    /// @param yc Y coordinate of the center of rotation
    /// @return Transform representing the rotation
    ///
    inline QTransform rotateAround(double angle, double xc, double yc) {
        return QTransform::fromTranslate(xc, yc).rotate(angle).translate(-xc, -yc);
    }

    /// Creates a transform that performs a rotation by the specified angle around the specified point.
    ///
    /// @param angle Rotation angle in degrees
    /// @param center Coordinate of the center of rotation
    /// @return Transform representing the rotation
    ///
    inline QTransform rotateAround(double angle, QPoint center) {
        return rotateAround(angle, center.x(), center.y());
    }
}
