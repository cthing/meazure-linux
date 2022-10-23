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
    inline bool fuzzyZero(T f) {
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
}
