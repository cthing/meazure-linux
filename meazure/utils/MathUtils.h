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

#include <cmath>
#include <limits>


/// Math convenience functions not found in QtMath.
///
namespace MathUtils {

    /// Tests whether the two specified floating point values are equal.
    ///
    /// @tparam T One of float, double, or long double
    /// @param[in] f1 Value to test
    /// @param[in] f2 Value to test
    ///
    /// @return true if the two values are equal within an epsilon of the floating point type compensated
    /// for the magnitude of the values. See
    /// <a href="https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/">Comparing
    /// Floating Point Numbers, 2012 Edition</a> and
    /// <a href="https://embeddeduse.com/2019/08/26/qt-compare-two-floats/">Comparing Two Floating-Point Numbers</a>
    /// for details on this implementation.
    ///
    template<typename T>
    inline bool fuzzyEqual(T f1, T f2) {
        return (std::fabs(f1 - f2) <= std::numeric_limits<T>::epsilon())
               ? true
               : std::fabs(f1 - f2) <= (std::numeric_limits<T>::epsilon() * std::fmax(std::fabs(f1), std::fabs(f2)));
    }

    /// Tests whether the specified floating point value is zero.
    ///
    /// @tparam T One of float, double, or long double
    /// @param[in] f Value to test
    ///
    /// @return true if the value is equal to zero within an epsilon of the floating point type.
    ///
    template<typename T>
    inline bool fuzzyEqualZero(T f) {
        return std::fabs(f) <= std::numeric_limits<T>::epsilon();
    }

    /// Returns the closest odd number greater than or equal to the specified value.
    ///
    /// @param value Number to make odd
    ///
    /// @return Closest odd number greater than or equal to the specified value
    ///
    constexpr int makeOddUp(int value) {
        return (value % 2 == 0) ? (value + 1) : value;
    }

    /// Returns the closest odd number less than or equal to the specified value.
    ///
    /// @param value Number to make odd
    ///
    /// @return Closest odd number less than or equal to the specified value
    ///
    constexpr int makeOddDown(int value) {
        return (value % 2 == 0) ? (value - 1) : value;
    }

    /// Returns the closest even number greater than or equal to the specified value.
    ///
    /// @param value Number to make even
    ///
    /// @return Closest even number greater than or equal to the specified value
    ///
    constexpr int makeEvenUp(int value) {
        return (value % 2 == 0) ? value : (value + 1);
    }

    /// Returns the closest even number less than or equal to the specified value.
    ///
    /// @param value Number to make even
    ///
    /// @return Closest even number less than or equal to the specified value
    ///
    constexpr int makeEvenDown(int value) {
        return (value % 2 == 0) ? value : (value - 1);
    }

    /// Performs linear interpolation between the specified a and b values. Thr implementation is based on the
    /// std::lerp source code for C++20, which itself is based on https://github.com/emsr/cxx_linear/blob/master/lerp.h.
    /// See https://en.cppreference.com/w/cpp/numeric/lerp for a detailed explanation of the behavior of this function.
    /// This function can be removed once C++20 is used to compile the application.
    ///
    /// @tparam T Type of the parameters and return value
    /// @param[in] a First value
    /// @param[in] b Second value
    /// @param[in] t Amount to interpolate between the first and second values. If less than 1.0, interpolation is
    ///     performed. If greater than 1.0, extrapolation is performed.
    /// @return The result of a+t(b-a)
    ///
    template<typename T>
    constexpr T linearInterpolate(T a, T b, T t) {
        if (std::isnan(a) || std::isnan(b) || std::isnan(t)) {
            return std::numeric_limits<T>::quiet_NaN();
        }

        if ((a <= 0.0 && b >= 0.0) || (a >= 0.0 && b <= 0.0)) {
            // ab <= 0 but product could overflow.
            return t * b + (1.0 - t) * a;
        }

        if (t == 1.0) {
            // Exact
            return b;
        }

        // Exact at t=0, monotonic except near t=1, bounded, determinate, and consistent:
        const auto x = a + t * (b - a);
        return (t > 1.0) == (b > a) ? std::max(b, x) : std::min(b, x);
    }
}
