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
    QVERIFY(units.getScaleBasisStr().isEmpty());
    QCOMPARE(units.getScaleFactor(), 0.0);
    QCOMPARE(units.getDisplayPrecisions().size(), 8);
    const Units::DisplayPrecisions precisions { 0, 0, 0, 0, 0, 0, 0, 0 };
    QCOMPARE(units.getDisplayPrecisions(), precisions);
}

[[maybe_unused]] void PosLogCustomUnitsTest::testSetGet() {
    PosLogCustomUnits units;

    units.setName("abc");
    QCOMPARE(units.getName(), "abc");

    units.setAbbrev("defg");
    QCOMPARE(units.getAbbrev(), "defg");

    units.setScaleBasisStr("px");
    QCOMPARE(units.getScaleBasisStr(), "px");

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
    units.setDisplayPrecision(LinearMeasurementId::Area, 4);
    const Units::DisplayPrecisions expectedPrecisions { 0, 0, 0, 0, 1, 4, 1, 1 };
    QCOMPARE(units.getDisplayPrecisions(), expectedPrecisions);
}

[[maybe_unused]] void PosLogCustomUnitsTest::testAssignmentEquality() {
    PosLogCustomUnits units1;
    PosLogCustomUnits units2;

    units1.setName("abc");
    units1.setAbbrev("defg");
    units1.setScaleBasisStr("px");
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
    units2.setScaleBasisStr("in");
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
