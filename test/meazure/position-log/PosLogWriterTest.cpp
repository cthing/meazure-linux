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
#include <meazure/position-log/io/PosLogWriter.h>
#include <meazure/position-log/model/PosLogArchive.h>
#include <meazure/position-log/model/PosLogInfo.h>
#include <meazure/position-log/model/PosLogDesktop.h>
#include <meazure/position-log/model/PosLogPosition.h>
#include <meazure/position-log/model/PosLogScreen.h>
#include <meazure/position-log/model/PosLogCustomUnits.h>
#include <meazure/position-log/model/PosLogToolData.h>
#include <test/meazure/mocks/MockScreenInfoProvider.h>
#include <test/meazure/mocks/MockUnitsProvider.h>
#include <QDateTime>
#include <QRectF>
#include <QPointF>
#include <QSizeF>
#include <QString>
#include <memory>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PosLogWriterTest : public QObject {

    Q_OBJECT

private slots:
    [[maybe_unused]] void testWrite();
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
                <displayPrecision units="px">
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


[[maybe_unused]] void PosLogWriterTest::testWrite() {
    const QDateTime created = QDateTime::fromString("2023-01-10T07:45:42Z", Qt::ISODate);

    PosLogInfo info;
    info.setTitle("Test");
    info.setCreated(created);
    info.setAppName("TestRunner");
    info.setAppVersion("1.2.3");
    info.setAppBuild("10");
    info.setMachineName("hostA");
    info.setDescription("A test archive");

    PosLogScreen screen1;
    screen1.setPrimary(true);
    screen1.setRect(QRectF(0.0, 0.0, 2000.0, 1000.0));
    screen1.setRes(QSizeF(100.0, 100.0));
    screen1.setManualRes(false);
    screen1.setDescription("default");

    PosLogScreen screen2;
    screen2.setPrimary(false);
    screen2.setRect(QRectF(0.0, 0.0, 1000.0, 900.0));
    screen2.setRes(QSizeF(96.0, 97.0));
    screen2.setManualRes(true);

    PosLogScreen screen3;
    screen3.setPrimary(true);
    screen3.setRect(QRectF(0.0, 0.0, 1500.0, 950.0));
    screen3.setRes(QSizeF(100.0, 100.0));
    screen3.setManualRes(false);

    PosLogCustomUnits customUnits;
    customUnits.setName("Miles");
    customUnits.setAbbrev("mi");
    customUnits.setScaleBasisStr("px");
    customUnits.setScaleFactor(10.0);

    const PosLogDesktopSharedPtr desktop1 = std::make_shared<PosLogDesktop>("1f48833b-8edc-465e-833f-40065970b877");
    desktop1->setOrigin(QPointF(10.0, 20.0));
    desktop1->setSize(QSizeF(30.0, 25.0));
    desktop1->addScreen(screen1);
    desktop1->addScreen(screen2);

    const PosLogDesktopSharedPtr desktop2 = std::make_shared<PosLogDesktop>("51bc31df-68f3-49c7-b84e-d52e000009b2");
    desktop2->setSize(QSizeF(20.0, 15.0));
    desktop2->addScreen(screen3);
    desktop2->setLinearUnitsId(CustomId);
    desktop2->setCustomUnits(customUnits);

    PosLogToolData data1;
    data1.setPoint1(QPointF(1.0, 2.0));
    data1.setPoint2(QPointF(3.0, 7.0));
    data1.setWidthHeight(QSize(10.0, 20.0));
    PosLogPosition position1;
    position1.setToolName("LineTool");
    position1.setDescription("Position 1");
    position1.setToolData(data1);
    position1.setToolTraits(RadioToolTrait::XY1ReadWrite | RadioToolTrait::XY2ReadWrite | RadioToolTrait::WHReadOnly);
    position1.setRecorded(created);
    position1.setDesktop(desktop1);

    PosLogToolData data2;
    data2.setPoint1(QPointF(4.0, 5.0));
    data2.setPointV(QPointF(7.0, 10.0));
    data2.setDistance(5.0);
    PosLogPosition position2;
    position2.setToolName("CircleTool");
    position2.setDescription("Position 2");
    position2.setToolData(data2);
    position2.setToolTraits(RadioToolTrait::XY1ReadWrite | RadioToolTrait::XYVReadWrite | RadioToolTrait::DistReadOnly);
    position2.setRecorded(created);
    position2.setDesktop(desktop1);

    PosLogToolData data3;
    data3.setPoint1(QPointF(1.0, 2.0));
    data3.setPoint2(QPointF(3.0, 7.5));
    data3.setPointV(QPointF(6.0, 9.0));
    data3.setAngle(20.0);
    PosLogPosition position3;
    position3.setToolName("AngleTool");
    position3.setToolData(data3);
    position3.setToolTraits(RadioToolTrait::XY1ReadWrite | RadioToolTrait::XY2ReadWrite |
                            RadioToolTrait::XYVReadWrite | RadioToolTrait::AngleReadOnly);
    position3.setRecorded(created);
    position3.setDesktop(desktop2);

    PosLogPositionVector positions;
    positions.push_back(position1);
    positions.push_back(position2);
    positions.push_back(position3);

    PosLogArchive archive;
    archive.setVersion(1);
    archive.setInfo(info);
    archive.addDesktop(desktop1);
    archive.addDesktop(desktop2);
    archive.setPositions(positions);

    const MockScreenInfoProvider screenProvider;
    const MockUnitsProvider unitsProvider(screenProvider);

    std::ostringstream stream;
    PosLogWriter logWriter(unitsProvider);
    logWriter.write(stream, archive);

    //std::cerr << stream.str();

    const QStringList actualLines = QString(stream.str().c_str()).split('\n');
    const QStringList expectedLines = positionLog.split('\n');

    const unsigned int actualNumberLines = actualLines.size();
    const unsigned int expectedNumberLines = expectedLines.size();
    const unsigned int minNumberLines = std::min(actualNumberLines, expectedNumberLines);

    bool failed = false;

    for (unsigned int lineNumber = 0; lineNumber < minNumberLines; lineNumber++) {
        if (actualLines[lineNumber] != expectedLines[lineNumber]) {
            failed = true;
            std::cout << "Line " << lineNumber << " differs:" << std::endl;
            std::cout << "    Actual:   " << actualLines[lineNumber].toUtf8().constData() << std::endl;
            std::cout << "    Expected: " << expectedLines[lineNumber].toUtf8().constData() << std::endl;
        }
    }

    if (actualNumberLines != expectedNumberLines) {
        failed = true;

        if (actualNumberLines < expectedNumberLines) {
            std::cout << "Missing lines:" << std::endl;
            for (unsigned int lineNumber = minNumberLines; lineNumber < expectedNumberLines; lineNumber++) {
                std::cout << expectedLines[lineNumber].toUtf8().constData() << std::endl;
            }
        } else {
            std::cout << "Extra lines:" << std::endl;
            for (unsigned int lineNumber = minNumberLines; lineNumber < actualNumberLines; lineNumber++) {
                std::cout << actualLines[lineNumber].toUtf8().constData() << std::endl;
            }
        }
    }

    if (failed) {
        QFAIL("PosLogWriter output not expected");
    }
}


QTEST_MAIN(PosLogWriterTest)

#include "PosLogWriterTest.moc"
