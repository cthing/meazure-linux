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

#include <QTest>
#include <QtPlugin>
#include <meazure/position-log/model/PosLogToolData.h>
#include <QPointF>
#include <QSizeF>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogToolDataTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testSetGet();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogToolDataTest::testConstruction() {
    const PosLogToolData data;
    QVERIFY(data.getPoint1().isNull());
    QVERIFY(data.getPoint2().isNull());
    QVERIFY(data.getPointV().isNull());
    QVERIFY(!data.getWidthHeight().isValid());
    QCOMPARE(data.getDistance(), 0.0);
    QCOMPARE(data.getAngle(), 0.0);
    QCOMPARE(data.getArea(), 0.0);
}

[[maybe_unused]] void PosLogToolDataTest::testSetGet() {
    PosLogToolData data;

    data.setPoint1(QPointF(1.0, 2.0));
    QCOMPARE(data.getPoint1(), QPointF(1.0, 2.0));

    data.setPoint2(QPointF(3.4, 7.0));
    QCOMPARE(data.getPoint2(), QPointF(3.4, 7.0));

    data.setPointV(QPointF(3.0, 4.5));
    QCOMPARE(data.getPointV(), QPointF(3.0, 4.5));

    data.setWidthHeight(QSizeF(3.0, 4.5));
    QCOMPARE(data.getWidthHeight(), QSizeF(3.0, 4.5));

    data.setDistance(3.5);
    QCOMPARE(data.getDistance(), 3.5);

    data.setAngle(4.2);
    QCOMPARE(data.getAngle(), 4.2);

    data.setArea(1.2);
    QCOMPARE(data.getArea(), 1.2);
}

[[maybe_unused]] void PosLogToolDataTest::testAssignmentEquality() {
    PosLogToolData data1;
    PosLogToolData data2;

    data1.setPoint1(QPointF(1.0, 2.0));
    data1.setPoint2(QPointF(3.4, 7.0));
    data1.setPointV(QPointF(3.0, 4.5));
    data1.setWidthHeight(QSizeF(3.0, 4.5));
    data1.setDistance(3.5);
    data1.setAngle(4.2);
    data1.setArea(1.2);

    data2.setPoint1(QPointF(4.0, 1.0));
    data2.setPoint2(QPointF(56.4, 7.7));
    data2.setPointV(QPointF(1.0, -8.5));
    data2.setWidthHeight(QSizeF(3.0, 7.5));
    data2.setDistance(3.9);
    data2.setAngle(2.2);
    data2.setArea(4.2);

    QVERIFY(data1 == data1);
    QVERIFY(data1 != data2);

    data2 = data1;
    QVERIFY(data1 == data2);
}


QTEST_MAIN(PosLogToolDataTest)

#include "PosLogToolDataTest.moc"
