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
#include <meazure/position-log/model/PosLogCustomUnits.h>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogCustomUnitsTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testSetGet();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogCustomUnitsTest::testConstruction() {
    const PosLogCustomUnits units;
    QVERIFY(units.getName().isEmpty());
    QVERIFY(units.getAbbrev().isEmpty());
    QCOMPARE(units.getScaleBasis(), CustomUnits::ScaleBasis::PixelBasis);
    QCOMPARE(units.getScaleFactor(), 0.0);
    QVERIFY(units.getDisplayPrecisions().empty());
}

[[maybe_unused]] void PosLogCustomUnitsTest::testSetGet() {
    PosLogCustomUnits units;

    units.setName("abc");
    QCOMPARE(units.getName(), "abc");

    units.setAbbrev("defg");
    QCOMPARE(units.getAbbrev(), "defg");

    units.setScaleBasis(CustomUnits::ScaleBasis::CentimeterBasis);
    QCOMPARE(units.getScaleBasis(), CustomUnits::ScaleBasis::CentimeterBasis);

    units.setScaleFactor(2.5);
    QCOMPARE(units.getScaleFactor(), 2.5);

    Units::DisplayPrecisions precisions;
    precisions.push_back(0);        // XCoord
    precisions.push_back(0);        // YCoord
    precisions.push_back(0);        // Width
    precisions.push_back(0);        // Height
    precisions.push_back(1);        // Distance
    precisions.push_back(0);        // Area
    precisions.push_back(1);        // ResX
    precisions.push_back(1);        // ResY
    units.setDisplayPrecisions(precisions);
    QCOMPARE(units.getDisplayPrecisions(), precisions);
}

[[maybe_unused]] void PosLogCustomUnitsTest::testAssignmentEquality() {
    PosLogCustomUnits units1;
    PosLogCustomUnits units2;

    units1.setName("abc");
    units1.setAbbrev("defg");
    units1.setScaleBasis(CustomUnits::ScaleBasis::CentimeterBasis);
    units1.setScaleFactor(2.5);

    Units::DisplayPrecisions precisions1;
    precisions1.push_back(0);        // XCoord
    precisions1.push_back(0);        // YCoord
    precisions1.push_back(0);        // Width
    precisions1.push_back(0);        // Height
    precisions1.push_back(1);        // Distance
    precisions1.push_back(0);        // Area
    precisions1.push_back(1);        // ResX
    precisions1.push_back(1);        // ResY
    units1.setDisplayPrecisions(precisions1);

    units2.setName("asdf");
    units2.setAbbrev("wertwer");
    units2.setScaleBasis(CustomUnits::ScaleBasis::InchBasis);
    units2.setScaleFactor(3.0);

    Units::DisplayPrecisions precisions2;
    precisions2.push_back(0);        // XCoord
    precisions2.push_back(1);        // YCoord
    precisions2.push_back(3);        // Width
    precisions2.push_back(3);        // Height
    precisions2.push_back(2);        // Distance
    precisions2.push_back(1);        // Area
    precisions2.push_back(2);        // ResX
    precisions2.push_back(2);        // ResY
    units1.setDisplayPrecisions(precisions2);

    QVERIFY(units1 == units1);
    QVERIFY(units1 != units2);

    units2 = units1;
    QVERIFY(units1 == units2);
}


QTEST_MAIN(PosLogCustomUnitsTest)

#include "PosLogCustomUnitsTest.moc"
