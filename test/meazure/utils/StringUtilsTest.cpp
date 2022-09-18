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
#include <meazure/utils/StringUtils.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class StringUtilsTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testDblToStr();
};


[[maybe_unused]] void StringUtilsTest::testDblToStr() {
        QCOMPARE(StringUtils::dblToStr(123.456), "123.456");
        QCOMPARE(StringUtils::dblToStr(123.4560000), "123.456");
        QCOMPARE(StringUtils::dblToStr(-123.456), "-123.456");
        QCOMPARE(StringUtils::dblToStr(+123.456), "123.456");
        QCOMPARE(StringUtils::dblToStr(0), "0.0");
        QCOMPARE(StringUtils::dblToStr(00), "0.0");
        QCOMPARE(StringUtils::dblToStr(0.0), "0.0");
        QCOMPARE(StringUtils::dblToStr(0.0000000), "0.0");
}


QTEST_MAIN(StringUtilsTest)

#include "StringUtilsTest.moc"
