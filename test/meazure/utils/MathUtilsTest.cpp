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

#include <QTest>
#include <QtPlugin>
#include <meazure/utils/MathUtils.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class MathUtilsTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testFuzzyEqual();
    [[maybe_unused]] void testFuzzyZero();
};


[[maybe_unused]] void MathUtilsTest::testFuzzyEqual() {
    QVERIFY(MathUtils::fuzzyEqual(0.0, 0.0));
    QVERIFY(MathUtils::fuzzyEqual(1.0, 1.0));
    QVERIFY(MathUtils::fuzzyEqual(-1.0, -1.0));
    QVERIFY(MathUtils::fuzzyEqual(3.141596, 3.141596));
    QVERIFY(MathUtils::fuzzyEqual(1.0 / 17.0, 1 / 17.0));
    QVERIFY(MathUtils::fuzzyEqual(std::numeric_limits<double>::epsilon(),
                                  std::numeric_limits<double>::epsilon()));
    QVERIFY(MathUtils::fuzzyEqual(1.000000001F, 1.000000002F));

    QVERIFY(!MathUtils::fuzzyEqual(0.0, 1.5));
    QVERIFY(!MathUtils::fuzzyEqual(-10.0, 3.141596));
    QVERIFY(!MathUtils::fuzzyEqual(1.000001F, 1.000002F));
}

[[maybe_unused]] void MathUtilsTest::testFuzzyZero() {
    QVERIFY(MathUtils::fuzzyZero(0.0));
    QVERIFY(MathUtils::fuzzyZero(-0.0));
    QVERIFY(MathUtils::fuzzyZero(std::numeric_limits<double>::epsilon()));

    QVERIFY(!MathUtils::fuzzyZero(1.0));
    QVERIFY(!MathUtils::fuzzyZero(-1.0));
    QVERIFY(!MathUtils::fuzzyZero(2.0 * std::numeric_limits<double>::epsilon()));
    QVERIFY(!MathUtils::fuzzyZero(-2.0 * std::numeric_limits<double>::epsilon()));
}


QTEST_MAIN(MathUtilsTest)

#include "MathUtilsTest.moc"
