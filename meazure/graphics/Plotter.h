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

#include <QSize>
#include <functional>
#include <cmath>
#include <meazure/utils/MathUtils.h>


/// Plots the points for drawing various shapes (e.g. lines, circles).
///
namespace Plotter {

    /// Plots the points comprising the crosshair. Four petals comprise the top, bottom, left and right of the
    /// crosshair. Each petal of the crosshair is made up of stacked rectangles whose heights are one pixels
    /// and whose widths are odd pixel values to ensure a symmetric figure, which is essential for measurement
    /// purposes. It is not possible to use line drawing methods because it will result in an asymmetric figure
    /// especially at the small sizes typical of the crosshair. In the figure below, the left and right petals
    /// are omitted for clarity.
    /// <pre>
    ///          |-------| petalWidth
    ///          ********* ---------------------------
    ///          *       *         |                 |
    ///           *     *          |                 |
    ///           *     *          |                 |
    ///            *   *           |                 |
    ///            *   *        petalHeight          |
    ///             * *            |                 |
    ///             * *            |                 |
    ///              *             |                 |
    ///              * -------------                 |
    ///                        centerOffset        size
    ///              x -------------                 |
    ///                                              |
    ///              *                               |
    ///              *                               |
    ///             * *                              |
    ///             * *                              |
    ///            *   *                             |
    ///            *   *                             |
    ///           *     *                            |
    ///           *     *                            |
    ///          *       *                           |
    ///          *********   -------------------------
    /// </pre>
    ///
    /// @param[in] size Total size of the crosshair, in pixels. Must be an odd number.
    /// @param[in] petalWidth Width of the petal at its widest point, in pixels. Must be an odd number.
    /// @param[in] centerOffset Number of pixels from the center of the crosshair to the point of a petal, in pixels.
    ///         Can be even or odd.
    /// @param[in] addRect Function called to record the plotted rectangle (x, y, width, height)
    ///
    inline void plotCrosshair(const QSize& size, const QSize& petalWidth, int centerOffset,
                              const std::function<void(int, int, int, int)>& addRect) {
        const auto slopeTimes = [](double m, int v) { return static_cast<int>(std::round(m * v)); };

        double slope = static_cast<double>(2.0 * petalWidth.width()) / (size.height() - 2.0 * centerOffset);
        const int xc = size.width() / 2;
        int petalHeight = size.height() / 2 - centerOffset;

        //
        // Top petal
        //
        for (int y = 0; y < petalHeight; y++) {
            const int w = MathUtils::makeOddDown(petalWidth.width() - slopeTimes(slope, y));
            const int x = xc - w / 2;
            addRect(x, y, w, 1);
        }

        //
        // Bottom petal
        //
        const int yOffset = size.height() - 1;
        for (int y = 1 - petalHeight; y <= 0; y++) {
            const int w = MathUtils::makeOddDown(petalWidth.width() + slopeTimes(slope, y));
            const int x = xc - w / 2;
            addRect(x, y + yOffset, w, 1);
        }

        slope = static_cast<double>(2.0 * petalWidth.height()) / (size.width() - 2.0 * centerOffset);
        const int yc = size.height() / 2;
        petalHeight = size.width() / 2 - centerOffset;

        //
        // Left petal
        //
        for (int x = 0; x < petalHeight; x++) {
            const int h = MathUtils::makeOddDown(petalWidth.height() - slopeTimes(slope, x));
            const int y = yc - h / 2;
            addRect(x, y, 1, h);
        }

        //
        // Right petal
        //
        const int xOffset = size.width() - 1;
        for (int x = 1 - petalHeight; x <= 0; x++) {
            const int h = MathUtils::makeOddDown(petalWidth.height() + slopeTimes(slope, x));
            const int y = yc - h / 2;
            addRect(x + xOffset, y, 1, h);
        }
    }
}
