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

/// Performs the same function as QVERIFY without the test and return.
///
/// @param statement Expression to test for truth
///
#define MEA_VERIFY(statement) \
    QTest::qVerify(static_cast<bool>(statement), #statement, "", __FILE__, __LINE__)

/// Performs the same function as QCOMPARE without the test and return.
///
/// @param actual Actual value
/// @param expected Expected value
///
#define MEA_COMPARE(actual, expected) \
    QTest::qCompare(actual, expected, #actual, #expected, __FILE__, __LINE__)

#define MEA_CHECK(statement) \
do { \
    statement; \
    if (QTest::currentTestFailed()) { \
        return; \
    } \
} while(false)
