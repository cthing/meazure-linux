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
#include <meazure/units/Units.h>
#include <meazure/units/CustomUnits.h>
#include <test/meazure/testing/TestHelpers.h>
#include <test/meazure/mocks/MockScreenInfoProvider.h>
#include <QPoint>
#include <QSizeF>
#include <QString>
#include <QSignalSpy>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class UnitsTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void init() {
        LinearUnits::setInvertY(false);
        LinearUnits::setOrigin(QPoint());

        AngularUnits::setSupplementalAngle(false);
    }

    [[maybe_unused]] void testSupplementalProperty();
    [[maybe_unused]] void testDegreeUnits();
    [[maybe_unused]] void testDegreeUnitsSupplemental();
    [[maybe_unused]] void testRadianUnits();
    [[maybe_unused]] void testRadianUnitsSupplemental();
    [[maybe_unused]] void testPixelUnits();
    [[maybe_unused]] void testPointUnits();
    [[maybe_unused]] void testPicaUnits();
    [[maybe_unused]] void testTwipUnits();
    [[maybe_unused]] void testInchUnits();
    [[maybe_unused]] void testCentimeterUnits();
    [[maybe_unused]] void testMillimeterUnits();
    [[maybe_unused]] void testCustomUnits();
};


const Units::DisplayPrecisionNames linearPrecisionNames { "x", "y", "w", "h", "d", "area", "rx", "ry" };

void verifyUnitsId(AngularUnits& units, AngularUnitsId unitsId, const QString& unitsStr) {
    QCOMPARE(units.getUnitsStr(), unitsStr);
    QCOMPARE(units.getUnitsId(), unitsId);
}

void verifyUnitsId(LinearUnits& units, LinearUnitsId unitsId, const QString& unitsStr) {
    QCOMPARE(units.getUnitsStr(), unitsStr);
    QCOMPARE(units.getUnitsId(), unitsId);
}

void verifyPrecisions(Units& units, const Units::DisplayPrecisions& defaultPrecisions,
                      const Units::DisplayPrecisionNames& names) {
    QCOMPARE(units.getDefaultPrecisions(), defaultPrecisions);

    const Units::DisplayPrecisions precisions { 10, 20, 30 };
    units.setDisplayPrecisions(precisions);
    QCOMPARE(units.getDisplayPrecisions(), precisions);

    units.restoreDefaultPrecisions();
    QCOMPARE(units.getDisplayPrecisions(), defaultPrecisions);

    QCOMPARE(units.getDisplayPrecisionNames(), names);
}

void verifyFormat(AngularUnits& units, double value, const QString& valueStr) {
    QCOMPARE(units.format(Angle, value), valueStr);
}

void verifyFormat(LinearUnits& units, LinearMeasurementId measurementId, double value, const QString& valueStr) {
    QCOMPARE(units.format(measurementId, value), valueStr);
}

void verifyConvertAngle(AngularUnits& units, double radians, double expected) {
    QCOMPARE(units.convertAngle(radians), expected);
}

void verifyInvertY(LinearUnits& units) {
    QVERIFY(!units.isInvertY());
    LinearUnits::setInvertY(true);
    QVERIFY(units.isInvertY());
}

void verifyOrigin(LinearUnits& units) {
    const QPoint origin(0, 0);
    QCOMPARE(units.getOrigin(), origin);

    const QPoint newOrigin(100, 200);
    LinearUnits::setOrigin(newOrigin);
    QCOMPARE(units.getOrigin(), newOrigin);
}

void verifyRequiresRes(const LinearUnits& units, bool requiresRes) {
    QCOMPARE(units.isResRequired(), requiresRes);
}

void verifyConvertCoords(LinearUnits& units, const QPointF& corrected, const QPointF& uncorrected) {
    LinearUnits::setOrigin(QPoint(100, 200));
    LinearUnits::setInvertY(true);
    const QPoint pos(110, 220);
    QPointF result = units.convertCoord(pos);
    QCOMPARE(result.x(), corrected.x());
    QCOMPARE(result.y(), corrected.y());
    result = units.convertPos(pos);
    QCOMPARE(result.x(), uncorrected.x());
    QCOMPARE(result.y(), uncorrected.y());

    QCOMPARE(units.unconvertCoord(ConvertX, nullptr, corrected.x()), 110.0);
    QCOMPARE(units.unconvertCoord(ConvertY, nullptr, corrected.y()), 220.0);

    int c1 = -1;
    int c2 = -1;
    QVERIFY(units.unconvertCoord(ConvertX, nullptr, corrected.x(), c1, c2));
    QCOMPARE(c1, 110);
    QCOMPARE(c2, 111);
    QVERIFY(units.unconvertCoord(ConvertY, nullptr, corrected.y(), c1, c2));
    QCOMPARE(c1, 220);
    QCOMPARE(c2, 221);

    const QPoint convertedPos(110, 220);
    QCOMPARE(units.unconvertCoord(corrected), convertedPos);
    QCOMPARE(units.unconvertPos(uncorrected), convertedPos);
}

void verifyConvertRes(LinearUnits& units, const QSizeF& convertedRes) {
    const QSizeF res(100.0, 90.0);
    const QSizeF result = units.convertRes(res);
    QCOMPARE(result.width(), convertedRes.width());
    QCOMPARE(result.height(), convertedRes.height());
}

void verifyToPixels(LinearUnits& units, double x, double y, int convertedX, int convertedY) {
    const QSizeF res(100.0, 90.0);
    QCOMPARE(units.convertToPixels(res, x, 1).width(), convertedX);
    QCOMPARE(units.convertToPixels(res, 0.0, 100).width(), 100);
    QCOMPARE(units.convertToPixels(res, y, 1).height(), convertedY);
}

void verifyFromPixels(LinearUnits& units, double xfactor, double yfactor) {
    const QSizeF res(100.0, 90.0);
    const QSizeF result = units.fromPixels(res);
    QCOMPARE(result.width(), xfactor);
    QCOMPARE(result.height(), yfactor);
}


//=========================================================================

[[maybe_unused]] void UnitsTest::testSupplementalProperty() {
    QVERIFY(!AngularUnits::isSupplementalAngle());
    AngularUnits::setSupplementalAngle(true);
    QVERIFY(AngularUnits::isSupplementalAngle());
}

[[maybe_unused]] void UnitsTest::testDegreeUnits() {
    DegreeUnits units;

    MEA_CHECK(verifyUnitsId(units, DegreesId, "deg"));

    const Units::DisplayPrecisions defaultPrecisions { 1 };
    const Units::DisplayPrecisionNames names { "angle" };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, names));

    MEA_CHECK(verifyFormat(units, 90.12345, "90.1"));

    MEA_CHECK(verifyConvertAngle(units, 1.0, 57.2957795131));
    MEA_CHECK(verifyConvertAngle(units, 0.0, 0.0));
    MEA_CHECK(verifyConvertAngle(units, 3.141592653589793, 180.0));
}

[[maybe_unused]] void UnitsTest::testDegreeUnitsSupplemental() {
    DegreeUnits units;

    AngularUnits::setSupplementalAngle(true);
    MEA_CHECK(verifyConvertAngle(units, 1.0, 122.704220487));
    MEA_CHECK(verifyConvertAngle(units, 0.0, 180.0));
    MEA_CHECK(verifyConvertAngle(units, 3.141592653589793, 0.0));
}

[[maybe_unused]] void UnitsTest::testRadianUnits() {
    RadianUnits units;

    MEA_CHECK(verifyUnitsId(units, RadiansId, "rad"));

    const Units::DisplayPrecisions defaultPrecisions { 3 };
    const Units::DisplayPrecisionNames names { "angle" };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, names));

    MEA_CHECK(verifyFormat(units, 1.12345, "1.123"));

    MEA_CHECK(verifyConvertAngle(units, 1.0, 1.0));
    MEA_CHECK(verifyConvertAngle(units, 0.0, 0.0));
    MEA_CHECK(verifyConvertAngle(units, 3.141592653589793, 3.141592653589793));
}

[[maybe_unused]] void UnitsTest::testRadianUnitsSupplemental() {
    RadianUnits units;

    AngularUnits::setSupplementalAngle(true);
    MEA_CHECK(verifyConvertAngle(units, 1.0, 2.141592653589793));
    MEA_CHECK(verifyConvertAngle(units, 0.0, 3.141592653589793));
    MEA_CHECK(verifyConvertAngle(units, 3.1415926535897939, 0.0));
}

[[maybe_unused]] void UnitsTest::testPixelUnits() {
    const MockScreenInfoProvider screenProvider;
    PixelUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, PixelsId, "px"));

    const Units::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 1, 0, 1, 1 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, false));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90"));
    MEA_CHECK(verifyFormat(units, Distance, 90.12345, "90.1"));

    const QPointF correctedPos(10.0, -20.0);
    const QPointF uncorrectedPos(110.0, 220.0);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF res(100.0, 90.0);
    MEA_CHECK(verifyConvertRes(units, res));

    MEA_CHECK(verifyToPixels(units, 10.0, 20.0, 10, 20));
    MEA_CHECK(verifyFromPixels(units, 1.0, 1.0));
}

[[maybe_unused]] void UnitsTest::testPointUnits() {
    const MockScreenInfoProvider screenProvider;
    PointUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, PointsId, "pt"));

    const Units::DisplayPrecisions defaultPrecisions { 1, 1, 1, 1, 1, 1, 2, 2 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, true));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90.1"));
    MEA_CHECK(verifyFormat(units, ResX, 90.12345, "90.12"));

    const QPointF correctedPos(7.5, -15.0);
    const QPointF uncorrectedPos(82.5, 165.0);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes(1.38888888889, 1.25);
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 15.0, 15.0, 20, 18));
    MEA_CHECK(verifyFromPixels(units, 0.72, 0.8));
}

[[maybe_unused]] void UnitsTest::testPicaUnits() {
    const MockScreenInfoProvider screenProvider;
    PicaUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, PicasId, "pc"));

    const Units::DisplayPrecisions defaultPrecisions { 2, 2, 2, 2, 2, 2, 1, 1 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, true));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90.12"));
    MEA_CHECK(verifyFormat(units, ResX, 90.12345, "90.1"));

    const QPointF correctedPos(0.625, -1.25);
    const QPointF uncorrectedPos(6.875, 13.75);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes(16.0 + 2.0/3.0, 15.0);
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 1.25, 1.25, 20, 18));
    MEA_CHECK(verifyFromPixels(units, 0.06, 0.0666666666667));
}

[[maybe_unused]] void UnitsTest::testTwipUnits() {
    const MockScreenInfoProvider screenProvider;
    TwipUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, TwipsId, "tp"));

    const Units::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 0, 0, 4, 4 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, true));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90"));
    MEA_CHECK(verifyFormat(units, ResX, 90.123456, "90.1235"));

    const QPointF correctedPos(150.0, -300.0);
    const QPointF uncorrectedPos(1650.0, 3300.0);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes(0.0694444444444, 0.0625);
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 5000, 2000, 347, 125));
    MEA_CHECK(verifyFromPixels(units, 14.4, 16.0));
}

[[maybe_unused]] void UnitsTest::testInchUnits() {
    const MockScreenInfoProvider screenProvider;
    InchUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, InchesId, "in"));

    const Units::DisplayPrecisions defaultPrecisions { 3, 3, 3, 3, 3, 3, 1, 1 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, true));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90.123"));
    MEA_CHECK(verifyFormat(units, ResX, 90.123456, "90.1"));

    const QPointF correctedPos(0.10416666666666666, -0.20833333333333331);
    const QPointF uncorrectedPos(1.1458333333333333, 2.2916666666666665);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes(100.0, 90.0);
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 50, 20, 5000, 1800));
    MEA_CHECK(verifyFromPixels(units, 0.01, 0.011111111111111112));
}

[[maybe_unused]] void UnitsTest::testCentimeterUnits() {
    const MockScreenInfoProvider screenProvider;
    CentimeterUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, CentimetersId, "cm"));

    const Units::DisplayPrecisions defaultPrecisions { 2, 2, 2, 2, 2, 2, 1, 1 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, true));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90.12"));
    MEA_CHECK(verifyFormat(units, ResX, 90.123456, "90.1"));

    const QPointF correctedPos(0.26458333333333334, -0.52916666666666667);
    const QPointF uncorrectedPos(2.9104166666666669, 5.8208333333333337);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes = { 39.370078740157481, 35.433070866141733 };
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 50, 20, 1968, 708));
    MEA_CHECK(verifyFromPixels(units, 0.025399999999999999, 0.028222222222222221));
}

[[maybe_unused]] void UnitsTest::testMillimeterUnits() {
    const MockScreenInfoProvider screenProvider;
    MillimeterUnits units(screenProvider);

    MEA_CHECK(verifyUnitsId(units, MillimetersId, "mm"));

    const Units::DisplayPrecisions defaultPrecisions { 1, 1, 1, 1, 1, 1, 2, 2 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));
    MEA_CHECK(verifyRequiresRes(units, true));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90.1"));
    MEA_CHECK(verifyFormat(units, ResX, 90.123456, "90.12"));

    const QPointF correctedPos(2.6458333333333334, -5.2916666666666667);
    const QPointF uncorrectedPos(29.104166666666669, 58.208333333333337);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes(3.9370078740157481, 3.5433070866141733);
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 50, 20, 196, 70));
    MEA_CHECK(verifyFromPixels(units, 0.254, 0.28222222222222221));
}

[[maybe_unused]] void UnitsTest::testCustomUnits() {
    const MockScreenInfoProvider screenProvider;
    CustomUnits units(screenProvider);

    const QSignalSpy unitsSpy(&units, SIGNAL(labelChanged(LinearUnitsId, const QString&, const QString&)));
    QVERIFY(unitsSpy.isValid());

    QVERIFY(!units.haveCustomUnits());

    units.setName("Abcd");
    QCOMPARE(unitsSpy.size(), 1);
    QList<QVariant> arguments = unitsSpy[0];
    QCOMPARE(arguments.size(), 3);
    QCOMPARE(arguments[0].toInt(), 7);
    QCOMPARE(arguments[1].toString(), "Abcd");
    QCOMPARE(arguments[2].toString(), "");

    units.setAbbrev("ab");
    QCOMPARE(unitsSpy.size(), 2);
    arguments = unitsSpy[1];
    QCOMPARE(arguments.size(), 3);
    QCOMPARE(arguments[0].toInt(), 7);
    QCOMPARE(arguments[1].toString(), "Abcd");
    QCOMPARE(arguments[2].toString(), "ab");

    QVERIFY(units.haveCustomUnits());
    QCOMPARE(units.getName(), "Abcd");
    QCOMPARE(units.getAbbrev(), "ab");

    QCOMPARE(units.getScaleBasis(), CustomUnits::PixelBasis);
    QCOMPARE(units.getScaleBasisStr(), "px");
    QVERIFY(!units.isResRequired());
    units.setScaleBasis(CustomUnits::CentimeterBasis);
    QCOMPARE(units.getScaleBasis(), CustomUnits::CentimeterBasis);
    QCOMPARE(units.getScaleBasisStr(), "cm");
    QVERIFY(units.isResRequired());
    units.setScaleBasis(CustomUnits::InchBasis);
    QCOMPARE(units.getScaleBasis(), CustomUnits::InchBasis);
    QCOMPARE(units.getScaleBasisStr(), "in");
    QVERIFY(units.isResRequired());

    units.setScaleBasis("px");
    QCOMPARE(units.getScaleBasis(), CustomUnits::PixelBasis);
    QCOMPARE(units.getScaleBasisStr(), "px");
    QVERIFY(!units.isResRequired());
    units.setScaleBasis("cm");
    QCOMPARE(units.getScaleBasis(), CustomUnits::CentimeterBasis);
    QCOMPARE(units.getScaleBasisStr(), "cm");
    QVERIFY(units.isResRequired());
    units.setScaleBasis("in");
    QCOMPARE(units.getScaleBasis(), CustomUnits::InchBasis);
    QCOMPARE(units.getScaleBasisStr(), "in");
    QVERIFY(units.isResRequired());

    QCOMPARE(units.getScaleFactor(), 1.0);
    units.setScaleFactor(2.0);
    QCOMPARE(units.getScaleFactor(), 2.0);

    MEA_CHECK(verifyUnitsId(units, CustomId, "custom"));

    const Units::DisplayPrecisions defaultPrecisions { 0, 0, 0, 0, 1, 0, 1, 1 };
    MEA_CHECK(verifyPrecisions(units, defaultPrecisions, linearPrecisionNames));

    MEA_CHECK(verifyInvertY(units));
    MEA_CHECK(verifyOrigin(units));

    MEA_CHECK(verifyFormat(units, XCoord, 90.12345, "90"));
    MEA_CHECK(verifyFormat(units, ResX, 90.123456, "90.1"));

    const QPointF correctedPos(0.0520833333333333294, -0.10416666666666666);
    const QPointF uncorrectedPos(0.57291666666666663, 1.1458333333333333);
    MEA_CHECK(verifyConvertCoords(units, correctedPos, uncorrectedPos));

    const QSizeF convertedRes(200.0, 180.0);
    MEA_CHECK(verifyConvertRes(units, convertedRes));

    MEA_CHECK(verifyToPixels(units, 50, 20, 10000, 3600));
    MEA_CHECK(verifyFromPixels(units, 0.0050000000000000001, 0.0055555555555555558));
}


QTEST_MAIN(UnitsTest)

#include "UnitsTest.moc"
