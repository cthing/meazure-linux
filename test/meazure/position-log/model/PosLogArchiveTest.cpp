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
#include <meazure/position-log/model/PosLogArchive.h>
#include <meazure/position-log/model/PosLogInfo.h>
#include <meazure/position-log/model/PosLogDesktop.h>
#include <meazure/position-log/model/PosLogPosition.h>
#include <memory>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogArchiveTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testConstruction();
    [[maybe_unused]] void testMutation();
    [[maybe_unused]] void testAssignmentEquality();
};


[[maybe_unused]] void PosLogArchiveTest::testConstruction() {
    const PosLogArchive archive;
    QCOMPARE(archive.getInfo(), PosLogInfo());
    QVERIFY(archive.getDesktops().empty());
    QVERIFY(archive.getPositions().empty());
}

[[maybe_unused]] void PosLogArchiveTest::testMutation() {
    PosLogArchive archive;

    PosLogInfo info;
    info.setAppName("abcd");

    const PosLogDesktopSharedPtr desktop = std::make_shared<PosLogDesktop>();
    desktop->setInvertY(true);

    PosLogPosition position;
    position.setDesktop(desktop);
    position.setToolName("asdb");
    PosLogPositionVector positions;
    positions.push_back(position);

    archive.setInfo(info);
    QCOMPARE(archive.getInfo(), info);

    archive.addDesktop(desktop);
    QCOMPARE(archive.getDesktops().size(), 1);
    QCOMPARE(archive.getDesktops()[0], desktop);

    archive.setPositions(positions);
    QCOMPARE(archive.getPositions(), positions);
}

[[maybe_unused]] void PosLogArchiveTest::testAssignmentEquality() {
    PosLogArchive archive1;
    PosLogArchive archive2;

    PosLogInfo info1;
    info1.setAppName("abcd");

    PosLogInfo info2;
    info2.setAppName("adbbed");

    const PosLogDesktopSharedPtr desktop1 = std::make_shared<PosLogDesktop>();
    desktop1->setInvertY(true);

    const PosLogDesktopSharedPtr desktop2 = std::make_shared<PosLogDesktop>();
    desktop2->setLinearUnitsId(TwipsId);

    PosLogPosition position1;
    position1.setDesktop(desktop1);
    position1.setToolName("asdb");
    PosLogPositionVector positions1;
    positions1.push_back(position1);

    PosLogPosition position2;
    position2.setDesktop(desktop2);
    position2.setToolName("asaseeeee");
    PosLogPositionVector positions2;
    positions2.push_back(position2);

    archive1.setInfo(info1);
    archive1.addDesktop(desktop1);
    archive1.setPositions(positions1);

    archive1.setInfo(info2);
    archive1.addDesktop(desktop2);
    archive1.setPositions(positions2);

    QVERIFY(archive1 == archive1);
    QVERIFY(archive1 != archive2);

    archive2 = archive1;
    QVERIFY(archive1 == archive2);
}


QTEST_MAIN(PosLogArchiveTest)

#include "PosLogArchiveTest.moc"
