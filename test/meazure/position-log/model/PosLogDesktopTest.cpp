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
#include <meazure/position-log/model/PosLogDesktop.h>
#include <QPointF>
#include <QSizeF>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogDesktopTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testDefaultCtor();
    [[maybe_unused]] void testIdCtor();
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testMutation();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogDesktopTest::testDefaultCtor() {
    const PosLogDesktop desktop;
    QVERIFY(!desktop.getId().isNull());
}

[[maybe_unused]] void PosLogDesktopTest::testIdCtor() {
    const PosLogDesktop desktop("3b347415-ee22-42a4-9269-4e22f4b9fab7");
    QCOMPARE(desktop.getId(), "3b347415-ee22-42a4-9269-4e22f4b9fab7");
}

[[maybe_unused]] void PosLogDesktopTest::testConstruction() {
    const PosLogDesktop desktop;
    QVERIFY(desktop.getOrigin().isNull());
    QVERIFY(!desktop.isInvertY());
    QVERIFY(!desktop.getSize().isValid());
    QCOMPARE(desktop.getLinearUnitsId(), PixelsId);
    QCOMPARE(desktop.getAngularUnitsId(), DegreesId);
    QVERIFY(desktop.getScreens().empty());
    QCOMPARE(desktop.getCustomUnits(), PosLogCustomUnits());
}

[[maybe_unused]] void PosLogDesktopTest::testMutation() {
    PosLogDesktop desktop;

    desktop.setOrigin(QPointF(1.5, 2.6));
    QCOMPARE(desktop.getOrigin(), QPointF(1.5, 2.6));

    desktop.setInvertY(true);
    QVERIFY(desktop.isInvertY());

    desktop.setSize(QSizeF(10.0, 20.0));
    QCOMPARE(desktop.getSize(), QSizeF(10.0, 20.0));

    desktop.setLinearUnitsId(PicasId);
    QCOMPARE(desktop.getLinearUnitsId(), PicasId);

    desktop.setAngularUnitsId(RadiansId);
    QCOMPARE(desktop.getAngularUnitsId(), RadiansId);

    desktop.addScreen(PosLogScreen());
    QCOMPARE(desktop.getScreens().size(), 1);

    PosLogCustomUnits customUnits;
    customUnits.setName("foo");
    desktop.setCustomUnits(customUnits);
    QCOMPARE(desktop.getCustomUnits().getName(), "foo");
}

[[maybe_unused]] void PosLogDesktopTest::testAssignmentEquality() {
    PosLogDesktop desktop1;
    PosLogDesktop desktop2;
    PosLogDesktop desktop3;

    PosLogCustomUnits customUnits1;
    customUnits1.setName("foo");

    PosLogCustomUnits customUnits2;
    customUnits2.setName("bar");

    desktop1.setOrigin(QPointF(1.5, 2.6));
    desktop1.setInvertY(true);
    desktop1.setSize(QSizeF(10.0, 20.0));
    desktop1.setLinearUnitsId(PicasId);
    desktop1.setAngularUnitsId(RadiansId);
    desktop1.addScreen(PosLogScreen());
    desktop1.setCustomUnits(customUnits1);

    desktop2.setOrigin(QPointF(1.5, 2.6));
    desktop2.setInvertY(true);
    desktop2.setSize(QSizeF(10.0, 20.0));
    desktop2.setLinearUnitsId(PicasId);
    desktop2.setAngularUnitsId(RadiansId);
    desktop2.addScreen(PosLogScreen());
    desktop2.setCustomUnits(customUnits1);

    desktop3.setOrigin(QPointF(3.5, 8.6));
    desktop3.setInvertY(false);
    desktop3.setSize(QSizeF(11.0, 23.0));
    desktop3.setLinearUnitsId(TwipsId);
    desktop3.setAngularUnitsId(DegreesId);
    desktop3.addScreen(PosLogScreen());
    desktop3.setCustomUnits(customUnits2);

    QVERIFY(desktop1.getId() != desktop2.getId());
    QVERIFY(desktop1.getId() != desktop3.getId());

    QVERIFY(desktop1 != desktop2);
    QVERIFY(desktop1 != desktop3);
    QVERIFY(desktop1.isSame(desktop1));
    QVERIFY(desktop1.isSame(desktop2));
    QVERIFY(!desktop1.isSame(desktop3));

    desktop3 = desktop1;
    QVERIFY(desktop1 == desktop3);
    QVERIFY(desktop1.isSame(desktop3));
    QVERIFY(desktop3.isSame(desktop2));
}


QTEST_MAIN(PosLogDesktopTest)

#include "PosLogDesktopTest.moc"
