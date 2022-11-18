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
    [[maybe_unused]] void testFuzzyEqualZero();
    [[maybe_unused]] void testMakeOddUp();
    [[maybe_unused]] void testMakeOddDown();
    [[maybe_unused]] void testMakeEvenUp();
    [[maybe_unused]] void testMakeEvenDown();
    [[maybe_unused]] void testLinearInterpolate();
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

[[maybe_unused]] void MathUtilsTest::testFuzzyEqualZero() {
    QVERIFY(MathUtils::fuzzyEqualZero(0.0));
    QVERIFY(MathUtils::fuzzyEqualZero(-0.0));
    QVERIFY(MathUtils::fuzzyEqualZero(std::numeric_limits<double>::epsilon()));

    QVERIFY(!MathUtils::fuzzyEqualZero(1.0));
    QVERIFY(!MathUtils::fuzzyEqualZero(-1.0));
    QVERIFY(!MathUtils::fuzzyEqualZero(2.0 * std::numeric_limits<double>::epsilon()));
    QVERIFY(!MathUtils::fuzzyEqualZero(-2.0 * std::numeric_limits<double>::epsilon()));
}

[[maybe_unused]] void MathUtilsTest::testMakeOddUp() {
    QCOMPARE(MathUtils::makeOddUp(1), 1);
    QCOMPARE(MathUtils::makeOddUp(13), 13);
    QCOMPARE(MathUtils::makeOddUp(2), 3);
    QCOMPARE(MathUtils::makeOddUp(12), 13);
    QCOMPARE(MathUtils::makeOddUp(0), 1);
    QCOMPARE(MathUtils::makeOddUp(-1), -1);
    QCOMPARE(MathUtils::makeOddUp(-2), -1);
    QCOMPARE(MathUtils::makeOddUp(-10), -9);
}

[[maybe_unused]] void MathUtilsTest::testMakeOddDown() {
    QCOMPARE(MathUtils::makeOddDown(1), 1);
    QCOMPARE(MathUtils::makeOddDown(13), 13);
    QCOMPARE(MathUtils::makeOddDown(2), 1);
    QCOMPARE(MathUtils::makeOddDown(12), 11);
    QCOMPARE(MathUtils::makeOddDown(0), -1);
    QCOMPARE(MathUtils::makeOddDown(-1), -1);
    QCOMPARE(MathUtils::makeOddDown(-2), -3);
    QCOMPARE(MathUtils::makeOddDown(-10), -11);
}

[[maybe_unused]] void MathUtilsTest::testMakeEvenUp() {
    QCOMPARE(MathUtils::makeEvenUp(1), 2);
    QCOMPARE(MathUtils::makeEvenUp(13), 14);
    QCOMPARE(MathUtils::makeEvenUp(2), 2);
    QCOMPARE(MathUtils::makeEvenUp(12), 12);
    QCOMPARE(MathUtils::makeEvenUp(0), 0);
    QCOMPARE(MathUtils::makeEvenUp(-1), 0);
    QCOMPARE(MathUtils::makeEvenUp(-2), -2);
    QCOMPARE(MathUtils::makeEvenUp(-10), -10);
}

[[maybe_unused]] void MathUtilsTest::testMakeEvenDown() {
    QCOMPARE(MathUtils::makeEvenDown(1), 0);
    QCOMPARE(MathUtils::makeEvenDown(13), 12);
    QCOMPARE(MathUtils::makeEvenDown(2), 2);
    QCOMPARE(MathUtils::makeEvenDown(12), 12);
    QCOMPARE(MathUtils::makeEvenDown(0), 0);
    QCOMPARE(MathUtils::makeEvenDown(-1), -2);
    QCOMPARE(MathUtils::makeEvenDown(-2), -2);
    QCOMPARE(MathUtils::makeEvenDown(-10), -10);
}

[[maybe_unused]] void MathUtilsTest::testLinearInterpolate() {
    QCOMPARE(MathUtils::linearInterpolate(5.0, 10.0, 0.75), 8.75);
    QCOMPARE(MathUtils::linearInterpolate(5.0, 10.0, 0.0), 5.0);
    QCOMPARE(MathUtils::linearInterpolate(5.0, 10.0, 1.0), 10.0);
    QCOMPARE(MathUtils::linearInterpolate(5.0, 10.0, 1.25), 11.25);
    QCOMPARE(MathUtils::linearInterpolate(-3.0, 5.0, 0.75), 3.0);
    QCOMPARE(MathUtils::linearInterpolate(-3.0, 5.0, 0.0), -3.0);
    QCOMPARE(MathUtils::linearInterpolate(-3.0, 5.0, 1.0), 5.0);
    QCOMPARE(MathUtils::linearInterpolate(3.0, -5.0, 0.75), -3.0);
    QCOMPARE(MathUtils::linearInterpolate(3.0, -5.0, 0.0), 3.0);
    QCOMPARE(MathUtils::linearInterpolate(3.0, -5.0, 1.0), -5.0);
    QVERIFY(std::isnan(MathUtils::linearInterpolate(std::numeric_limits<double>::quiet_NaN(),
                                                    std::numeric_limits<double>::quiet_NaN(),
                                                    std::numeric_limits<double>::quiet_NaN())));
    QCOMPARE(MathUtils::linearInterpolate(5.0 * std::numeric_limits<double>::denorm_min(),
                                          10.0 * std::numeric_limits<double>::denorm_min(),
                                          0.75), 4.44659081257e-323);
    QCOMPARE(MathUtils::linearInterpolate(500.0 * std::numeric_limits<double>::denorm_min(),
                                          1000.0 * std::numeric_limits<double>::denorm_min(),
                                          0.75), 4.44659081257e-323);
    QCOMPARE(MathUtils::linearInterpolate(5.0, 5.0, std::numeric_limits<double>::infinity()), 5.0);
    QCOMPARE(MathUtils::linearInterpolate(5.0 * std::numeric_limits<double>::max() / 16.0,
                                          10.0 * std::numeric_limits<double>::max() / 16.0,
                                          3.0 / 4.0), 10 * std::numeric_limits<double>::max() / 32);
}


QTEST_MAIN(MathUtilsTest)

#include "MathUtilsTest.moc"
