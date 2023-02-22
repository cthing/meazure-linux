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

#include <type_traits>
#include <iterator>


/// Provides the ability to iterate over the values of an enum. For example:
/// \code{cpp}
///    enum SampleEnum {
///        Value1,
///        Value2,
///        Value3
///    };
///
///    using AnIter = EnumIterator<SampleEnum, Value1, Value3>;
///
///    for (const SampleEnum v : AnIter()) {
///        printf("value = %d\n", v);
///    }
/// \endcode
///
/// \b Limitation: The enum value must be consecutive.
///
/// @tparam ENUM_TYPE Name of the enum
/// @tparam BEGIN_VALUE First enum value
/// @tparam END_VALUE Last enum value
///
template <typename ENUM_TYPE, ENUM_TYPE BEGIN_VALUE, ENUM_TYPE END_VALUE>
class EnumIterator {

public:
    // Required for use with std algorithms.
    using iterator_category = std::input_iterator_tag;
    using difference_type = int;
    using value_type = typename std::underlying_type<ENUM_TYPE>::type;
    using pointer = value_type*;
    using reference = value_type&;

    /// Constructs an enum iterator starting at the specified enum value.
    ///
    /// @param[in] initialValue Initial enum value to start iteration
    ///
    explicit EnumIterator(const ENUM_TYPE& initialValue) : m_value(static_cast<value_type>(initialValue)) {}

    /// Constructs an enum iterator starting at the first enum value.
    ///
    EnumIterator() : m_value(static_cast<value_type>(BEGIN_VALUE)) {}

    /// Performs a prefix increment of the iterator.
    ///
    /// @return This instance of the iterator.
    ///
    EnumIterator operator++() {
        ++m_value;
        return *this;
    }

    /// Performs a postfix increment of the iterator.
    ///
    /// @return Iterator holding the value before the increment.
    ///
    EnumIterator operator++(int) {      // NOLINT(cert-dcl21-cpp)
        EnumIterator origIter(*this);
        ++m_value;
        return origIter;
    }

    /// Dereferences the iterator to obtain its current enum value.
    ///
    /// @return Current enum value.
    ///
    ENUM_TYPE operator*() {
        return static_cast<ENUM_TYPE>(m_value);
    }

    /// Obtains the start of iteration.
    ///
    /// @return Start of iteration (either the starting value of the enum or initial value specified in the
    ///         constructor).
    ///
    EnumIterator begin() {
        return *this;
    }

    /// Obtains the end of the interation.
    ///
    /// @return End of iteration.
    ///
    EnumIterator end() {
        static const EnumIterator endIter = ++EnumIterator(END_VALUE);
        return endIter;
    }

    /// Obtains the number of enum values.
    ///
    /// @return Number of enum values.
    ///
    int count() {
        return static_cast<value_type>(END_VALUE) - static_cast<value_type>(BEGIN_VALUE) + 1;
    }

    bool operator==(const EnumIterator& other) const { return m_value == other.m_value; }
    bool operator!=(const EnumIterator& other) const { return m_value != other.m_value; }
    bool operator<(const EnumIterator& other) const { return m_value < other.m_value; }
    bool operator>(const EnumIterator& other) const { return m_value > other.m_value; }
    bool operator<=(const EnumIterator& other) const { return m_value <= other.m_value; }
    bool operator>=(const EnumIterator& other) const { return m_value >= other.m_value; }

private:
    value_type m_value;
};
