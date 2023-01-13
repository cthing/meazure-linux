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
#include <meazure/position-log/PosLogReader.h>
#include <meazure/position-log/model/PosLogArchive.h>
#include <test/meazure/mocks/MockScreenInfoProvider.h>
#include <test/meazure/mocks/MockUnitsProvider.h>
#include <QString>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogReaderTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testRead();
};


QString positionLog = R"HERE(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE positionLog SYSTEM "https://www.cthing.com/dtd/PositionLog1.dtd">
<positionLog version="1">
    <info>
        <title>Test</title>
        <created date="2023-01-10T07:45:42Z"/>
        <generator name="TestRunner" version="1.2.3" build="10"/>
        <machine name="hostA"/>
        <desc>A test archive</desc>
    </info>
    <desktops>
        <desktop id="1f48833b-8edc-465e-833f-40065970b877">
            <units length="px" angle="deg"/>
            <origin xoffset="10.0" yoffset="20.0" invertY="false"/>
            <size x="30.0" y="25.0"/>
            <screens>
                <screen desc="default" primary="true">
                    <rect top="0.0" bottom="1000.0" left="0.0" right="2000.0"/>
                    <resolution x="100.0" y="100.0" manual="false"/>
                </screen>
                <screen desc="" primary="false">
                    <rect top="0.0" bottom="900.0" left="0.0" right="1000.0"/>
                    <resolution x="96.0" y="97.0" manual="true"/>
                </screen>
            </screens>
        </desktop>
        <desktop id="51bc31df-68f3-49c7-b84e-d52e000009b2">
            <units length="px" angle="deg"/>
            <customUnits name="Miles" abbrev="mi" scaleBasis="px" scaleFactor="10.0"/>
            <origin xoffset="0.0" yoffset="0.0" invertY="false"/>
            <size x="20.0" y="15.0"/>
            <screens>
                <screen desc="" primary="true">
                    <rect top="0.0" bottom="950.0" left="0.0" right="1500.0"/>
                    <resolution x="100.0" y="100.0" manual="false"/>
                </screen>
            </screens>
            <displayPrecisions>
                <displayPrecision units="custom">
                    <measurement name="x" decimalPlaces="0"/>
                    <measurement name="y" decimalPlaces="0"/>
                    <measurement name="w" decimalPlaces="0"/>
                    <measurement name="h" decimalPlaces="0"/>
                    <measurement name="d" decimalPlaces="1"/>
                    <measurement name="area" decimalPlaces="0"/>
                    <measurement name="rx" decimalPlaces="1"/>
                    <measurement name="ry" decimalPlaces="1"/>
                </displayPrecision>
            </displayPrecisions>
        </desktop>
    </desktops>
    <positions>
        <position desktopRef="1f48833b-8edc-465e-833f-40065970b877" tool="LineTool" date="2023-01-10T07:45:42Z">
            <desc>Position 1</desc>
            <points>
                <point name="1" x="1.0" y="2.0"/>
                <point name="2" x="3.0" y="7.0"/>
            </points>
            <properties>
                <width value="10.0"/>
                <height value="20.0"/>
            </properties>
        </position>
        <position desktopRef="1f48833b-8edc-465e-833f-40065970b877" tool="CircleTool" date="2023-01-10T07:45:42Z">
            <desc>Position 2</desc>
            <points>
                <point name="1" x="4.0" y="5.0"/>
                <point name="v" x="7.0" y="10.0"/>
            </points>
            <properties>
                <distance value="5.0"/>
            </properties>
        </position>
        <position desktopRef="51bc31df-68f3-49c7-b84e-d52e000009b2" tool="AngleTool" date="2023-01-10T07:45:42Z">
            <points>
                <point name="1" x="1.0" y="2.0"/>
                <point name="2" x="3.0" y="7.5"/>
                <point name="v" x="6.0" y="9.0"/>
            </points>
            <properties>
                <angle value="20.0"/>
            </properties>
        </position>
    </positions>
</positionLog>
)HERE";


[[maybe_unused]] void PosLogReaderTest::testRead() {
    const MockScreenInfoProvider screenProvider;
    const MockUnitsProvider unitsProvider(screenProvider);

    PosLogReader logReader(unitsProvider);
    const PosLogArchiveSharedPtr archive = logReader.readString(positionLog);

    const PosLogInfo& info = archive->getInfo();
    QCOMPARE(info.getTitle(), "Test");
    QCOMPARE(info.getCreated(), QDateTime::fromString("2023-01-10T07:45:42Z", Qt::ISODate));
    QCOMPARE(info.getAppName(), "TestRunner");
    QCOMPARE(info.getAppVersion(), "1.2.3");
    QCOMPARE(info.getAppBuild(), "10");
    QCOMPARE(info.getMachineName(), "hostA");
    QCOMPARE(info.getDescription(), "A test archive");

    const PosLogDesktopSharedPtrVector desktops = archive->getDesktops();
    QCOMPARE(desktops.size(), 2);

    const PosLogDesktopSharedPtr desktop1 = desktops[0]; // NOLINT(performance-unnecessary-copy-initialization)
    QCOMPARE(desktop1->getId(), "1f48833b-8edc-465e-833f-40065970b877");
    QCOMPARE(desktop1->getLinearUnitsId(), PixelsId);
    QCOMPARE(desktop1->getAngularUnitsId(), DegreesId);
    QCOMPARE(desktop1->getOrigin(), QPointF(10.0, 20.0));
    QVERIFY(!desktop1->isInvertY());
    QCOMPARE(desktop1->getSize(), QSizeF(30.0, 25.0));

    const PosLogScreenVector screens1 = desktop1->getScreens();
    QCOMPARE(screens1.size(), 2);

    const PosLogScreen& screen1 = screens1[0];
    QCOMPARE(screen1.getDescription(), "default");
    QVERIFY(screen1.isPrimary());
    QCOMPARE(screen1.getRect(), QRectF(QPointF(0.0, 0.0), QPointF(2000.0, 1000.0)));
    QCOMPARE(screen1.getRes(), QSizeF(100.0, 100.0));
    QVERIFY(!screen1.isManualRes());

    const PosLogScreen& screen2 = screens1[1];
    QVERIFY(screen2.getDescription().isEmpty());
    QVERIFY(!screen2.isPrimary());
    QCOMPARE(screen2.getRect(), QRectF(QPointF(0.0, 0.0), QPointF(1000.0, 900.0)));
    QCOMPARE(screen2.getRes(), QSizeF(96.0, 97.0));
    QVERIFY(screen2.isManualRes());

    const PosLogDesktopSharedPtr desktop2 = desktops[1]; // NOLINT(performance-unnecessary-copy-initialization)
    QCOMPARE(desktop2->getId(), "51bc31df-68f3-49c7-b84e-d52e000009b2");
    QCOMPARE(desktop2->getLinearUnitsId(), PixelsId);
    QCOMPARE(desktop2->getAngularUnitsId(), DegreesId);
    QCOMPARE(desktop2->getOrigin(), QPointF(0.0, 0.0));
    QVERIFY(!desktop2->isInvertY());
    QCOMPARE(desktop2->getSize(), QSizeF(20.0, 15.0));

    const PosLogCustomUnits& customUnits = desktop2->getCustomUnits();
    QCOMPARE(customUnits.getName(), "Miles");
    QCOMPARE(customUnits.getAbbrev(), "mi");
    QCOMPARE(customUnits.getScaleBasisStr(), "px");
    QCOMPARE(customUnits.getScaleFactor(), 10.0);

    const Units::DisplayPrecisions precisions { 0, 0, 0, 0, 1, 0, 1, 1 };
    QCOMPARE(customUnits.getDisplayPrecisions(), precisions);

    const PosLogScreenVector screens2 = desktop2->getScreens();
    QCOMPARE(screens2.size(), 1);

    const PosLogScreen& screen3 = screens2[0];
    QVERIFY(screen3.getDescription().isEmpty());
    QVERIFY(screen3.isPrimary());
    QCOMPARE(screen3.getRect(), QRectF(QPointF(0.0, 0.0), QPointF(1500.0, 950.0)));
    QCOMPARE(screen3.getRes(), QSizeF(100.0, 100.0));
    QVERIFY(!screen3.isManualRes());

    const PosLogPositionVector positions = archive->getPositions();
    QCOMPARE(positions.size(), 3);

    const PosLogPosition& position1 = positions[0];
    QCOMPARE(position1.getDesktop()->getId(), "1f48833b-8edc-465e-833f-40065970b877");
    QCOMPARE(position1.getToolName(), "LineTool");
    QCOMPARE(position1.getRecorded(), QDateTime::fromString("2023-01-10T07:45:42Z", Qt::ISODate));
    QCOMPARE(position1.getDescription(), "Position 1");
    QCOMPARE(position1.getToolTraits(),
             RadioToolTrait::XY1Available | RadioToolTrait::XY2Available | RadioToolTrait::WHAvailable);
    const PosLogToolData& toolData1 = position1.getToolData();
    QCOMPARE(toolData1.getPoint1(), QPointF(1.0, 2.0));
    QCOMPARE(toolData1.getPoint2(), QPointF(3.0, 7.0));
    QCOMPARE(toolData1.getWidthHeight(), QSizeF(10.0, 20.0));

    const PosLogPosition& position2 = positions[1];
    QCOMPARE(position2.getDesktop()->getId(), "1f48833b-8edc-465e-833f-40065970b877");
    QCOMPARE(position2.getToolName(), "CircleTool");
    QCOMPARE(position2.getRecorded(), QDateTime::fromString("2023-01-10T07:45:42Z", Qt::ISODate));
    QCOMPARE(position2.getDescription(), "Position 2");
    QCOMPARE(position2.getToolTraits(),
             RadioToolTrait::XY1Available | RadioToolTrait::XYVAvailable | RadioToolTrait::DistAvailable);
    const PosLogToolData& toolData2 = position2.getToolData();
    QCOMPARE(toolData2.getPoint1(), QPointF(4.0, 5.0));
    QCOMPARE(toolData2.getPointV(), QPointF(7.0, 10.0));
    QCOMPARE(toolData2.getDistance(), 5.0);

    const PosLogPosition& position3 = positions[2];
    QCOMPARE(position3.getDesktop()->getId(), "51bc31df-68f3-49c7-b84e-d52e000009b2");
    QCOMPARE(position3.getToolName(), "AngleTool");
    QCOMPARE(position3.getRecorded(), QDateTime::fromString("2023-01-10T07:45:42Z", Qt::ISODate));
    QVERIFY(position3.getDescription().isEmpty());
    QCOMPARE(position3.getToolTraits(),
             RadioToolTrait::XY1Available | RadioToolTrait::XY2Available | RadioToolTrait::XYVAvailable |
             RadioToolTrait::AngleAvailable);
    const PosLogToolData& toolData3 = position3.getToolData();
    QCOMPARE(toolData3.getPoint1(), QPointF(1.0, 2.0));
    QCOMPARE(toolData3.getPoint2(), QPointF(3.0, 7.5));
    QCOMPARE(toolData3.getPointV(), QPointF(6.0, 9.0));
    QCOMPARE(toolData3.getAngle(), 20.0);
}


QTEST_GUILESS_MAIN(PosLogReaderTest)

#include "PosLogReaderTest.moc"
