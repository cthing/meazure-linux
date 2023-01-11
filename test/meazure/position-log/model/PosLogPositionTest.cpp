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
#include <meazure/position-log/model/PosLogPosition.h>
#include <meazure/position-log/model/PosLogToolData.h>
#include <meazure/position-log/model/PosLogDesktop.h>
#include <memory>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogPositionTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testSetGet();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogPositionTest::testConstruction() {
    const PosLogPosition position;
    QVERIFY(position.getToolName().isEmpty());
    QCOMPARE(position.getToolTraits().toInt(), 0);
    QCOMPARE(position.getToolData(), PosLogToolData());
    QVERIFY(position.getDescription().isEmpty());
    QVERIFY(!position.getRecorded().isValid());
    QVERIFY(!position.getDesktop());
}

[[maybe_unused]] void PosLogPositionTest::testSetGet() {
    PosLogPosition position;

    position.setToolName("abc");
    QCOMPARE(position.getToolName(), "abc");

    position.setToolTraits(RadioToolTrait::XY1ReadWrite | RadioToolTrait::XY2ReadOnly);
    QCOMPARE(position.getToolTraits(), RadioToolTrait::XY1ReadWrite | RadioToolTrait::XY2ReadOnly);

    PosLogToolData toolData;
    toolData.setArea(10.0);
    position.setToolData(toolData);
    QCOMPARE(position.getToolData(), toolData);

    position.setDescription("fghj");
    QCOMPARE(position.getDescription(), "fghj");

    const PosLogDesktopSharedPtr desktop = std::make_shared<PosLogDesktop>();
    desktop->setLinearUnitsId(TwipsId);
    desktop->setInvertY(true);
    position.setDesktop(desktop);
    QCOMPARE(position.getDesktop(), desktop);
}

[[maybe_unused]] void PosLogPositionTest::testAssignmentEquality() {
    PosLogPosition position1;
    PosLogPosition position2;

    PosLogToolData toolData1;
    toolData1.setArea(10.0);

    PosLogToolData toolData2;
    toolData2.setAngle(12.0);

    const PosLogDesktopSharedPtr desktop1 = std::make_shared<PosLogDesktop>();
    desktop1->setLinearUnitsId(TwipsId);
    desktop1->setInvertY(true);

    const PosLogDesktopSharedPtr desktop2 = std::make_shared<PosLogDesktop>();
    desktop2->setLinearUnitsId(InchesId);
    desktop2->setAngularUnitsId(RadiansId);

    position1.setToolName("abc");
    position1.setToolTraits(RadioToolTrait::XY1ReadWrite | RadioToolTrait::XY2ReadOnly);
    position1.setToolData(toolData1);
    position1.setDescription("fghj");
    position1.setDesktop(desktop1);

    position2.setToolName("foobar");
    position2.setToolTraits(RadioToolTrait::XY2ReadWrite | RadioToolTrait::XYVReadOnly);
    position2.setToolData(toolData2);
    position2.setDescription("kjhgfd");
    position2.setDesktop(desktop2);

    QVERIFY(position1 == position1);
    QVERIFY(position1 != position2);

    position2 = position1;
    QVERIFY(position1 == position2);
}


QTEST_MAIN(PosLogPositionTest)

#include "PosLogPositionTest.moc"
