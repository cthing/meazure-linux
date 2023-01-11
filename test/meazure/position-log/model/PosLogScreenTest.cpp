/*
 * Copyright 2023 C Thing Software
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
#include <meazure/position-log/model/PosLogScreen.h>
#include <QPointF>
#include <QSizeF>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogScreenTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testSetGet();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogScreenTest::testConstruction() {
    const PosLogScreen screen;
    QVERIFY(!screen.isPrimary());
    QVERIFY(!screen.getRect().isValid());
    QVERIFY(!screen.getRes().isValid());
    QVERIFY(!screen.isManualRes());
    QVERIFY(screen.getDescription().isEmpty());
}

[[maybe_unused]] void PosLogScreenTest::testSetGet() {
    PosLogScreen screen;

    screen.setPrimary(true);
    QVERIFY(screen.isPrimary());

    screen.setRect(QRectF(3.4, 7.0, 10.0, 20.0));
    QCOMPARE(screen.getRect(), QRectF(3.4, 7.0, 10.0, 20.0));

    screen.setRes(QSizeF(3.0, 4.5));
    QCOMPARE(screen.getRes(), QSizeF(3.0, 4.5));

    screen.setManualRes(true);
    QVERIFY(screen.isManualRes());

    screen.setDescription("adfggee");
    QCOMPARE(screen.getDescription(), "adfggee");
}

[[maybe_unused]] void PosLogScreenTest::testAssignmentEquality() {
    PosLogScreen screen1;
    PosLogScreen screen2;

    screen1.setPrimary(true);
    screen1.setRect(QRectF(3.4, 7.0, 10.0, 20.0));
    screen1.setRes(QSizeF(3.0, 4.5));
    screen1.setManualRes(true);
    screen1.setDescription("adfggee");

    screen2.setPrimary(false);
    screen2.setRect(QRectF(6.4, 9.0, 30.0, 10.0));
    screen2.setRes(QSizeF(3.7, 8.5));
    screen2.setManualRes(false);
    screen2.setDescription("ddfert");

    QVERIFY(screen1 == screen1);
    QVERIFY(screen1 != screen2);

    screen2 = screen1;
    QVERIFY(screen1 == screen2);
}


QTEST_MAIN(PosLogScreenTest)

#include "PosLogScreenTest.moc"
