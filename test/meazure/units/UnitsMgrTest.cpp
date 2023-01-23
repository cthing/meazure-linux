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
#include <meazure/units/UnitsMgr.h>
#include <test/meazure/mocks/MockScreenInfoProvider.h>
#include <QPoint>
#include <QSizeF>
#include <QSignalSpy>
#include <vector>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class UnitsMgrTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testDefaults();
    [[maybe_unused]] void testInvertY();
    [[maybe_unused]] void testOrigin();
    [[maybe_unused]] void testSupplementalAngle();
    [[maybe_unused]] void testSetLinearUnitsId();
    [[maybe_unused]] void testGetLinearUnitsId();
    [[maybe_unused]] void testSetAngularUnitsId();
    [[maybe_unused]] void testGetAngularUnitsId();
    [[maybe_unused]] void testGetCustomUnits();
    [[maybe_unused]] void testLinearPrecisions();
    [[maybe_unused]] void testAngularPrecisions();
    [[maybe_unused]] void testGetWidthHeight();
    [[maybe_unused]] void testFormat();
    [[maybe_unused]] void testConversion();
};


[[maybe_unused]] void UnitsMgrTest::testDefaults() {
    const MockScreenInfoProvider screenProvider;
    const UnitsMgr mgr(&screenProvider);

    QCOMPARE(mgr.getLinearUnits()->getUnitsId(), LinearUnitsId::PixelsId);
    QCOMPARE(mgr.getLinearUnitsId(), LinearUnitsId::PixelsId);
    QCOMPARE(mgr.getLinearUnitsStr(), "px");
    QCOMPARE(mgr.getAngularUnits()->getUnitsId(), AngularUnitsId::DegreesId);
    QCOMPARE(mgr.getAngularUnitsId(), AngularUnitsId::DegreesId);
    QCOMPARE(mgr.getAngularUnitsStr(), "deg");

    QVERIFY(!mgr.isInvertY());
    QCOMPARE(mgr.getOrigin(), QPoint());
    QVERIFY(!mgr.isSupplementalAngle());

    QCOMPARE(mgr.getMajorTickCount(), 10);
    QCOMPARE(mgr.getMinorTickIncr(QRect(10, 10, 20, 20)), QSizeF(10.0, 10.0));
}

[[maybe_unused]] void UnitsMgrTest::testInvertY() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    QVERIFY(!mgr.isInvertY());
    mgr.setInvertY(true);
    QVERIFY(mgr.isInvertY());
}

[[maybe_unused]] void UnitsMgrTest::testOrigin() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    QCOMPARE(mgr.getOrigin(), QPoint());
    mgr.setOrigin(QPoint(100, 200));
    QCOMPARE(mgr.getOrigin(), QPoint(100, 200));
}

[[maybe_unused]] void UnitsMgrTest::testSupplementalAngle() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    QVERIFY(!mgr.isSupplementalAngle());
    mgr.setSupplementalAngle(true);
    QVERIFY(mgr.isSupplementalAngle());
}

[[maybe_unused]] void UnitsMgrTest::testSetLinearUnitsId() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    const QSignalSpy unitsChangedSpy(&mgr, SIGNAL(linearUnitsChanged(LinearUnitsId)));
    QVERIFY(unitsChangedSpy.isValid());
    const QSignalSpy calibrationSpy(&mgr, SIGNAL(calibrationRequired()));
    QVERIFY(calibrationSpy.isValid());

    mgr.setLinearUnits(LinearUnitsId::InchesId);

    QCOMPARE(unitsChangedSpy.size(), 1);
    QList<QVariant> arguments = unitsChangedSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), LinearUnitsId::InchesId);

    QCOMPARE(calibrationSpy.size(), 1);

    QCOMPARE(mgr.getLinearUnits()->getUnitsId(), LinearUnitsId::InchesId);
    QCOMPARE(mgr.getLinearUnitsId(), LinearUnitsId::InchesId);
    QCOMPARE(mgr.getLinearUnitsStr(), "in");

    mgr.setLinearUnits("pc");

    QCOMPARE(unitsChangedSpy.size(), 2);
    arguments = unitsChangedSpy[1];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), LinearUnitsId::PicasId);

    QCOMPARE(calibrationSpy.size(), 1);

    QCOMPARE(mgr.getLinearUnits()->getUnitsId(), LinearUnitsId::PicasId);
    QCOMPARE(mgr.getLinearUnitsId(), LinearUnitsId::PicasId);
    QCOMPARE(mgr.getLinearUnitsStr(), "pc");
}

[[maybe_unused]] void UnitsMgrTest::testGetLinearUnitsId() {
    const MockScreenInfoProvider screenProvider;
    const UnitsMgr mgr(&screenProvider);

    LinearUnits* units = mgr.getLinearUnits(LinearUnitsId::TwipsId);
    QCOMPARE(units->getUnitsId(), LinearUnitsId::TwipsId);

    units = mgr.getLinearUnits("pc");
    QCOMPARE(units->getUnitsId(), LinearUnitsId::PicasId);

    units = mgr.getLinearUnits("zzzzzz");
    QCOMPARE(units, nullptr);
}

[[maybe_unused]] void UnitsMgrTest::testSetAngularUnitsId() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    const QSignalSpy unitsChangedSpy(&mgr, SIGNAL(angularUnitsChanged(AngularUnitsId)));
    QVERIFY(unitsChangedSpy.isValid());

    mgr.setAngularUnits(AngularUnitsId::RadiansId);

    QCOMPARE(unitsChangedSpy.size(), 1);
    QList<QVariant> arguments = unitsChangedSpy[0];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), AngularUnitsId::RadiansId);

    QCOMPARE(mgr.getAngularUnits()->getUnitsId(), AngularUnitsId::RadiansId);
    QCOMPARE(mgr.getAngularUnitsId(), AngularUnitsId::RadiansId);
    QCOMPARE(mgr.getAngularUnitsStr(), "rad");

    mgr.setAngularUnits("deg");

    QCOMPARE(unitsChangedSpy.size(), 2);
    arguments = unitsChangedSpy[1];
    QCOMPARE(arguments.size(), 1);
    QCOMPARE(arguments[0].toInt(), AngularUnitsId::DegreesId);

    QCOMPARE(mgr.getAngularUnits()->getUnitsId(), AngularUnitsId::DegreesId);
    QCOMPARE(mgr.getAngularUnitsId(), AngularUnitsId::DegreesId);
    QCOMPARE(mgr.getAngularUnitsStr(), "deg");
}

[[maybe_unused]] void UnitsMgrTest::testGetAngularUnitsId() {
    const MockScreenInfoProvider screenProvider;
    const UnitsMgr mgr(&screenProvider);

    AngularUnits* units = mgr.getAngularUnits(AngularUnitsId::RadiansId);
    QCOMPARE(units->getUnitsId(), AngularUnitsId::RadiansId);

    units = mgr.getAngularUnits("rad");
    QCOMPARE(units->getUnitsId(), AngularUnitsId::RadiansId);

    units = mgr.getAngularUnits("zzzzzz");
    QCOMPARE(units, nullptr);
}

[[maybe_unused]] void UnitsMgrTest::testGetCustomUnits() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    CustomUnits* units = mgr.getCustomUnits();
    QCOMPARE(units->getUnitsId(), LinearUnitsId::CustomId);
}

[[maybe_unused]] void UnitsMgrTest::testLinearPrecisions() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    const std::vector<int> expectedDefaults { 3, 3, 3, 3, 3, 3, 1, 1 };
    const Units::DisplayPrecisions& defaultPrecisions1 = mgr.getLinearDefaultPrecisions(LinearUnitsId::InchesId);
    QCOMPARE(defaultPrecisions1, expectedDefaults);
    const Units::DisplayPrecisions& currentPrecisions1 = mgr.getLinearDisplayPrecisions(LinearUnitsId::InchesId);
    QCOMPARE(currentPrecisions1, expectedDefaults);

    const std::vector<int> expectedNew { 1, 2, 3, 4, 5, 6, 7, 8 };
    mgr.setLinearDisplayPrecisions(LinearUnitsId::InchesId, expectedNew);
    const Units::DisplayPrecisions& defaultPrecisions2 = mgr.getLinearDefaultPrecisions(LinearUnitsId::InchesId);
    QCOMPARE(defaultPrecisions2, expectedDefaults);
    const Units::DisplayPrecisions& currentPrecisions2 = mgr.getLinearDisplayPrecisions(LinearUnitsId::InchesId);
    QCOMPARE(currentPrecisions2, expectedNew);
}

[[maybe_unused]] void UnitsMgrTest::testAngularPrecisions() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    const std::vector<int> expectedDefaults { 3 };
    const Units::DisplayPrecisions& defaultPrecisions1 = mgr.getAngularDefaultPrecisions(AngularUnitsId::RadiansId);
    QCOMPARE(defaultPrecisions1, expectedDefaults);
    const Units::DisplayPrecisions& currentPrecisions1 = mgr.getAngularDisplayPrecisions(AngularUnitsId::RadiansId);
    QCOMPARE(currentPrecisions1, expectedDefaults);

    const std::vector<int> expectedNew { 6 };
    mgr.setAngularDisplayPrecisions(AngularUnitsId::RadiansId, expectedNew);
    const Units::DisplayPrecisions& defaultPrecisions2 = mgr.getAngularDefaultPrecisions(AngularUnitsId::RadiansId);
    QCOMPARE(defaultPrecisions2, expectedDefaults);
    const Units::DisplayPrecisions& currentPrecisions2 = mgr.getAngularDisplayPrecisions(AngularUnitsId::RadiansId);
    QCOMPARE(currentPrecisions2, expectedNew);
}

[[maybe_unused]] void UnitsMgrTest::testGetWidthHeight() {
    const MockScreenInfoProvider screenProvider;
    const UnitsMgr mgr(&screenProvider);

    const QPoint p1(10, 200);
    const QPoint p2(20, 300);
    const QSizeF wh = mgr.getWidthHeight(p1, p2);
    QCOMPARE(wh.width(), 11.0);
    QCOMPARE(wh.height(), 101.0);
}

[[maybe_unused]] void UnitsMgrTest::testFormat() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    mgr.setAngularUnits(AngularUnitsId::DegreesId);
    QCOMPARE(mgr.formatConvertAngle(qDegreesToRadians(120.0)), "120.0");
    mgr.setAngularUnits(AngularUnitsId::RadiansId);
    QCOMPARE(mgr.formatConvertAngle(qDegreesToRadians(120.0)), "2.094");

    mgr.setAngularUnits(AngularUnitsId::DegreesId);
    QCOMPARE(mgr.format(Angle, 120.0), "120.0");
    mgr.setAngularUnits(AngularUnitsId::RadiansId);
    QCOMPARE(mgr.format(Angle, qDegreesToRadians(120.0)), "2.094");

    mgr.setLinearUnits(LinearUnitsId::InchesId);
    QCOMPARE(mgr.format(XCoord, 120.0), "120.000");
    mgr.setLinearUnits(LinearUnitsId::PixelsId);
    QCOMPARE(mgr.format(YCoord, 120.0), "120");

    mgr.setSupplementalAngle(true);
    mgr.setAngularUnits(AngularUnitsId::DegreesId);
    QCOMPARE(mgr.formatConvertAngle(qDegreesToRadians(120.0)), "60.0");
    mgr.setAngularUnits(AngularUnitsId::RadiansId);
    QCOMPARE(mgr.formatConvertAngle(qDegreesToRadians(120.0)), "1.047");
}

[[maybe_unused]] void UnitsMgrTest::testConversion() {
    const MockScreenInfoProvider screenProvider;
    UnitsMgr mgr(&screenProvider);

    mgr.setOrigin(QPoint(10, 20));
    mgr.setInvertY(true);

    mgr.setLinearUnits(LinearUnitsId::PixelsId);

    const QPoint pos1(100, 200);
    const QPointF expectedPos1(90.0, -180.0);
    const QPointF convertedPos1 = mgr.convertCoord(pos1);
    QCOMPARE(convertedPos1, expectedPos1);
    QCOMPARE(mgr.unconvertCoord(convertedPos1), pos1);

    const QPoint pos2(100, 200);
    const QPointF expectedPos2(100.0, 200.0);
    const QPointF convertedPos2 = mgr.convertPos(pos2);
    QCOMPARE(convertedPos2, expectedPos2);
    QCOMPARE(mgr.unconvertPos(convertedPos2), pos2);

    const QSize convertedSize = mgr.convertToPixels(LinearUnitsId::InchesId, QSizeF(2.0, 3.0), 120.0, 1);
    QCOMPARE(convertedSize, QSize(240, 360));

    mgr.setLinearUnits(LinearUnitsId::PointsId);

    const QSizeF convertedRes = mgr.convertRes(QSizeF(20.0, 30.0));
    QCOMPARE(convertedRes, QSizeF(20.0 / 72.0, 30.0 / 72.0));
}


QTEST_MAIN(UnitsMgrTest)

#include "UnitsMgrTest.moc"
