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
#include <QPoint>
#include <QSizeF>
#include <meazure/utils/Geometry.h>
#include <vector>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class GeometryTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testAreaRect();
    [[maybe_unused]] void testAreaSizeF();
    [[maybe_unused]] void testAreaRadius();
    [[maybe_unused]] void testAspectRatio();
    [[maybe_unused]] void testHypot1();
    [[maybe_unused]] void testHypot2();
    [[maybe_unused]] void testHypot3();
    [[maybe_unused]] void testAngle2Points_data();
    [[maybe_unused]] void testAngle2Points();
    [[maybe_unused]] void testAngle3Points();
    [[maybe_unused]] void testNormalizedDegrees();
    [[maybe_unused]] void testDistance();
    [[maybe_unused]] void testCalcSector_data();
    [[maybe_unused]] void testCalcSector();
    [[maybe_unused]] void testContainsPoint();
    [[maybe_unused]] void testContainsRect();
    [[maybe_unused]] void testClosestRect();
    [[maybe_unused]] void testConstrainPointToRect();
    [[maybe_unused]] void testConstrainPointToRects();
    [[maybe_unused]] void testRotateAround();
};


[[maybe_unused]] void GeometryTest::testAreaRect() {
    QCOMPARE(Geometry::area(QRect()), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 0, 0)), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 1, 0)), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 0, 1)), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 10, 20)), 200);
    QCOMPARE(Geometry::area(QRect(1, 2, 10, 20)), 200);
    QCOMPARE(Geometry::area(QRect(-1, 2, 10, 20)), 200);
    QCOMPARE(Geometry::area(QRect(1, -2, 10, 20)), 200);
    QCOMPARE(Geometry::area(QRect(-1, -2, 10, 20)), 200);
}

[[maybe_unused]] void GeometryTest::testAreaSizeF() {
    QCOMPARE(Geometry::area(QSizeF()), 0.0);
    QCOMPARE(Geometry::area(QSizeF(0.0, 0.0)), 0.0);
    QCOMPARE(Geometry::area(QSizeF(1.0, 0.0)), 0.0);
    QCOMPARE(Geometry::area(QSizeF(0.0, 1.0)), 0.0);
    QCOMPARE(Geometry::area(QSizeF(10.5, 20.5)), 215.25);
}

[[maybe_unused]] void GeometryTest::testAreaRadius() {
    QCOMPARE(Geometry::area(0.0), 0.0);
    QCOMPARE(Geometry::area(1.0), M_PI);
    QCOMPARE(Geometry::area(2.0), 4.0 * M_PI);
    QCOMPARE(Geometry::area(5.25), 27.5625 * M_PI);
}

[[maybe_unused]] void GeometryTest::testAspectRatio() {
    QCOMPARE(Geometry::aspectRatio(QSizeF()), 0.0);
    QCOMPARE(Geometry::aspectRatio(QSizeF(0.0, 0.0)), 0.0);
    QCOMPARE(Geometry::aspectRatio(QSizeF(1.0, 0.0)), 0.0);
    QCOMPARE(Geometry::aspectRatio(QSizeF(0.0, 1.0)), 0.0);
    QCOMPARE(Geometry::aspectRatio(QSizeF(10.5, 20.5)), 0.512195121951);
}

[[maybe_unused]] void GeometryTest::testHypot1() {
    QCOMPARE(Geometry::hypot(QSizeF()), 0.0);
    QCOMPARE(Geometry::hypot(QSizeF(0.0, 0.0)), 0.0);
    QCOMPARE(Geometry::hypot(QSizeF(1.0, 0.0)), 0.0);
    QCOMPARE(Geometry::hypot(QSizeF(0.0, 1.0)), 0.0);
    QCOMPARE(Geometry::hypot(QSizeF(10.5, 20.5)), 23.03258561256);
}

[[maybe_unused]] void GeometryTest::testHypot2() {
    QCOMPARE(Geometry::hypot(QPoint(), QPoint()), 0.0);
    QCOMPARE(Geometry::hypot(QPoint(0, 0), QPoint(0, 0)), 0.0);
    QCOMPARE(Geometry::hypot(QPoint(0, 0), QPoint(1, 0)), 1.0);
    QCOMPARE(Geometry::hypot(QPoint(0, 0), QPoint(0, 1)), 1.0);
    QCOMPARE(Geometry::hypot(QPoint(0, 0), QPoint(1, 1)), 1.414213562373);
    QCOMPARE(Geometry::hypot(QPoint(10, 20), QPoint(3, 7)), 14.76482306023);
}

[[maybe_unused]] void GeometryTest::testHypot3() {
    QCOMPARE(Geometry::hypot(QPointF(), QPointF()), 0.0);
    QCOMPARE(Geometry::hypot(QPointF(0.0, 0.0), QPointF(0.0, 0.0)), 0.0);
    QCOMPARE(Geometry::hypot(QPointF(0.0, 0.0), QPointF(1.0, 0.0)), 1.0);
    QCOMPARE(Geometry::hypot(QPointF(0.0, 0.0), QPointF(0.0, 1.0)), 1.0);
    QCOMPARE(Geometry::hypot(QPointF(0.0, 0.0), QPointF(1.0, 1.0)), 1.414213562373);
    QCOMPARE(Geometry::hypot(QPointF(10.0, 20.0), QPointF(3.0, 7.0)), 14.76482306023);
}

[[maybe_unused]] void GeometryTest::testAngle2Points_data() {
    QTest::addColumn<double>("x");
    QTest::addColumn<double>("y");
    QTest::addColumn<double>("angle");

    QTest::newRow("0.0,0.0,0.0")               <<  0.0 <<  0.0 <<  0.0;             // Zero vector
    QTest::newRow("1.0,0.0,0.0")               <<  1.0 <<  0.0 <<  0.0;             // 0 degrees
    QTest::newRow("2.0,1.0,0.463647609001")    <<  2.0 <<  1.0 <<  0.463647609001;  // 27 degrees
    QTest::newRow("1.0,1.0,0.785398163397")    <<  1.0 <<  1.0 <<  0.785398163397;  // 45 degrees
    QTest::newRow("1.0,2.0,1.107148717794")    <<  1.0 <<  2.0 <<  1.107148717794;  // 63 degrees
    QTest::newRow("0.0,1.0,1.570796326795")    <<  0.0 <<  1.0 <<  1.570796326795;  // 90 degrees
    QTest::newRow("-1.0,2.0,2.034443935796")   << -1.0 <<  2.0 <<  2.034443935796;  // 117 degrees
    QTest::newRow("-1.0,1.0,2.35619449019")    << -1.0 <<  1.0 <<  2.35619449019;   // 135 degrees
    QTest::newRow("-2.0,1.0,2.67794504459")    << -2.0 <<  1.0 <<  2.67794504459;   // 153 degrees
    QTest::newRow("-1.0,0.0,3.14159265359")    << -1.0 <<  0.0 <<  3.14159265359;   // 180 degrees
    QTest::newRow("-2.0,-1.0,-2.67794504459")  << -2.0 << -1.0 << -2.67794504459;   // 207 degrees
    QTest::newRow("-1.0,-1.0,-2.35619449019")  << -1.0 << -1.0 << -2.35619449019;   // 225 degrees
    QTest::newRow("-1.0,-2.0,-2.034443935796") << -1.0 << -2.0 << -2.034443935796;  // 243 degrees
    QTest::newRow("0.0,-1.0,-1.570796326795")  <<  0.0 << -1.0 << -1.570796326795;  // 270 degrees
    QTest::newRow("1.0,-2.0,-1.107148717794")  <<  1.0 << -2.0 << -1.107148717794;  // 297 degrees
    QTest::newRow("1.0,-1.0,-0.785398163397")  <<  1.0 << -1.0 << -0.785398163397;  // 315 degrees
    QTest::newRow("2.0,-1.0,-0.463647609001")  <<  2.0 << -1.0 << -0.463647609001;  // 333 degrees
    QTest::newRow("1.0,-1.0,-0.785398163397")  <<  1.0 << -1.0 << -0.785398163397;  // 315 degrees
}

[[maybe_unused]] void GeometryTest::testAngle2Points() {
    const QFETCH(double, x);
    const QFETCH(double, y);
    const QFETCH(double, angle);

    const QPointF p0(1.0, 2.0);
    const QPointF p1(p0.x() + x, p0.y() + y);
    QCOMPARE(Geometry::angle(p0, p1), angle);
}

[[maybe_unused]] void GeometryTest::testAngle3Points() {
    const QPointF p0(1.0, 2.0);
    QCOMPARE(Geometry::angle(p0, p0, p0), 0.0);

    const QPointF p1(5.0, 6.0);
    QCOMPARE(Geometry::angle(p0, p1, p1), 0.0);

    const QPointF p2(4.0, -1.0);
    const QPointF p3(5.0, 6.0);
    QCOMPARE(Geometry::angle(p0, p2, p3), 1.570796326795);
    QCOMPARE(Geometry::angle(p0, p3, p2), -1.570796326795);

    const QPointF p4(1.0, 5.0);
    const QPointF p5(5.0, 2.0);
    QCOMPARE(Geometry::angle(p0, p4, p5), -1.570796326795);
    QCOMPARE(Geometry::angle(p0, p5, p4), 1.570796326795);

    const QPointF p6(5.0, -2.0);
    const QPointF p7(-4.0, 4.0);
    QCOMPARE(Geometry::angle(p0, p6, p7), -2.736700867305);
}

[[maybe_unused]] void GeometryTest::testNormalizedDegrees() {
    QCOMPARE(Geometry::normalizeDegrees(0), 0);
    QCOMPARE(Geometry::normalizeDegrees(10), 10);
    QCOMPARE(Geometry::normalizeDegrees(190), 190);
    QCOMPARE(Geometry::normalizeDegrees(290), 290);
    QCOMPARE(Geometry::normalizeDegrees(359), 359);
    QCOMPARE(Geometry::normalizeDegrees(360), 0);
    QCOMPARE(Geometry::normalizeDegrees(370), 10);
    QCOMPARE(Geometry::normalizeDegrees(550), 190);
    QCOMPARE(Geometry::normalizeDegrees(-10), 350);
    QCOMPARE(Geometry::normalizeDegrees(-180), 180);
    QCOMPARE(Geometry::normalizeDegrees(-360), 0);
}

[[maybe_unused]] void GeometryTest::testDistance() {
    QCOMPARE(Geometry::distance(QRect(), QPoint()), std::numeric_limits<double>::max());
    QCOMPARE(Geometry::distance(QRect(), QPoint(1, 2)), std::numeric_limits<double>::max());
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(20, 25)), 0.0);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(5, 10)), 7.07106781187);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(5, 20)), 5.0);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(5, 40)), 7.81024967591);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(40, 40)), 6.0);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(55, 40)), 8.48528137424);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(55, 20)), 6.0);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(55, 10)), 7.81024967591);
    QCOMPARE(Geometry::distance(QRect(10, 15, 40, 20), QPoint(40, 10)), 5.0);
}

[[maybe_unused]] void GeometryTest::testCalcSector_data() {
    QTest::addColumn<int>("x");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("sector");
    QTest::addColumn<bool>("horizontal");
    QTest::addColumn<bool>("vertical");

    QTest::newRow("0,0,0")    <<  0 <<  0 <<  0 << false << false;      // Zero vector
    QTest::newRow("1,0,1")    <<  1 <<  0 <<  1 <<  true << false;      // 0 degrees
    QTest::newRow("2,1,1")    <<  2 <<  1 <<  1 <<  true << false;      // 27 degrees
    QTest::newRow("1,1,2")    <<  1 <<  1 <<  2 << false <<  true;      // 45 degrees
    QTest::newRow("1,2,2")    <<  1 <<  2 <<  2 << false <<  true;      // 63 degrees
    QTest::newRow("0,1,3")    <<  0 <<  1 <<  3 << false <<  true;      // 90 degrees
    QTest::newRow("-1,2,3")   << -1 <<  2 <<  3 << false <<  true;      // 117 degrees
    QTest::newRow("-1,1,4")   << -1 <<  1 <<  4 <<  true << false;      // 135 degrees
    QTest::newRow("-2,1,4")   << -2 <<  1 <<  4 <<  true << false;      // 153 degrees
    QTest::newRow("-1,0,-4")  << -1 <<  0 << -4 <<  true << false;      // 180 degrees
    QTest::newRow("-2,-1,-4") << -2 << -1 << -4 <<  true << false;      // 207 degrees
    QTest::newRow("-1,-1,-4") << -1 << -1 << -4 <<  true << false;      // 225 degrees
    QTest::newRow("-1,-2,-3") << -1 << -2 << -3 << false <<  true;      // 243 degrees
    QTest::newRow("0,-1,-3")  <<  0 << -1 << -3 << false <<  true;      // 270 degrees
    QTest::newRow("1,-2,-2")  <<  1 << -2 << -2 << false <<  true;      // 297 degrees
    QTest::newRow("1,-1,-2")  <<  1 << -1 << -2 << false <<  true;      // 315 degrees
    QTest::newRow("2,-1,-1")  <<  2 << -1 << -1 <<  true << false;      // 333 degrees
}

[[maybe_unused]] void GeometryTest::testCalcSector() {
    const QFETCH(int, x);
    const QFETCH(int, y);
    const QFETCH(int, sector);
    const QFETCH(bool, horizontal);
    const QFETCH(bool, vertical);

    const QPoint p0(1, 2);
    const QPoint p1(p0.x() + x, p0.y() + y);
    QCOMPARE(Geometry::calcSector(p0, p1), sector);
    QCOMPARE(Geometry::isHorizontallyOriented(p0, p1), horizontal);
    QCOMPARE(Geometry::isVerticallyOriented(p0, p1), vertical);
}

[[maybe_unused]] void GeometryTest::testContainsPoint() {
    std::vector<QRect*> rects;
    QCOMPARE(Geometry::contains(rects, QPoint()), -1);
    QCOMPARE(Geometry::contains(rects, QPoint(10, 20)), -1);

    QRect rect1;
    QRect rect2;
    rects = { &rect1, &rect2 };
    QCOMPARE(Geometry::contains(rects, QPoint()), -1);
    QCOMPARE(Geometry::contains(rects, QPoint(10, 20)), -1);

    QRect rect3(0, 0, 100, 200);
    rects = { &rect3 };
    QCOMPARE(Geometry::contains(rects, QPoint(30, 40)), 0);
    QCOMPARE(Geometry::contains(rects, QPoint(0, 0)), 0);
    QCOMPARE(Geometry::contains(rects, QPoint(99, 199)), 0);
    QCOMPARE(Geometry::contains(rects, QPoint(100, 200)), -1);

    QRect rect4(0, 0, 100, 200);
    QRect rect5(300, 400, 200, 300);
    rects = { &rect4, & rect5 };
    QCOMPARE(Geometry::contains(rects, QPoint(350, 500)), 1);
    QCOMPARE(Geometry::contains(rects, QPoint(700, 500)), -1);

    QRect rect6;
    QRect rect7(300, 400, 200, 300);
    rects = { &rect6, &rect7 };
    QCOMPARE(Geometry::contains(rects, QPoint(350, 500)), 1);
    QCOMPARE(Geometry::contains(rects, QPoint(700, 500)), -1);
}

[[maybe_unused]] void GeometryTest::testContainsRect() {
    std::vector<QRect*> rects;
    QCOMPARE(Geometry::contains(rects, QRect()), -1);
    QCOMPARE(Geometry::contains(rects, QRect(10, 20, 100, 200)), -1);

    QRect rect1;
    QRect rect2;
    rects = { &rect1, &rect2 };
    QCOMPARE(Geometry::contains(rects, QRect()), -1);
    QCOMPARE(Geometry::contains(rects, QRect(10, 20, 100, 200)), -1);

    QRect rect3(0, 0, 100, 200);
    rects = { &rect3 };
    QCOMPARE(Geometry::contains(rects, QRect(10, 40, 10, 20)), 0);
    QCOMPARE(Geometry::contains(rects, QRect(0, 0, 100, 200)), 0);
    QCOMPARE(Geometry::contains(rects, QRect(75, 150, 100, 200)), 0);
    QCOMPARE(Geometry::contains(rects, QRect(400, 150, 100, 200)), -1);

    QRect rect4(0, 0, 100, 200);
    QRect rect5(100, 0, 200, 300);
    rects = { &rect4, &rect5 };
    QCOMPARE(Geometry::contains(rects, QRect(50, 100, 10, 20)), 0);
    QCOMPARE(Geometry::contains(rects, QRect(150, 100, 10, 20)), 1);
    QCOMPARE(Geometry::contains(rects, QRect(50, 100, 75, 20)), 0);
    QCOMPARE(Geometry::contains(rects, QRect(80, 100, 75, 20)), 1);
    QCOMPARE(Geometry::contains(rects, QRect()), -1);
}

[[maybe_unused]] void GeometryTest::testClosestRect() {
    std::vector<QRect*> rects;
    QCOMPARE(Geometry::closest(rects, QPoint()), -1);
    QCOMPARE(Geometry::closest(rects, QPoint(10, 20)), -1);

    QRect rect1;
    QRect rect2;
    rects = { &rect1, &rect2 };
    QCOMPARE(Geometry::closest(rects, QPoint()), -1);
    QCOMPARE(Geometry::closest(rects, QPoint(10, 20)), -1);

    QRect rect3(5, 10, 100, 200);
    rects = { &rect3 };
    QCOMPARE(Geometry::closest(rects, QPoint(30, 40)), 0);
    QCOMPARE(Geometry::closest(rects, QPoint(0, 0)), 0);
    QCOMPARE(Geometry::closest(rects, QPoint(2, 400)), 0);
    QCOMPARE(Geometry::closest(rects, QPoint(300, 400)), 0);

    QRect rect4(5, 5, 100, 200);
    QRect rect5(105, 5, 200, 300);
    rects = { &rect4, & rect5 };
    QCOMPARE(Geometry::closest(rects, QPoint(50, 50)), 0);
    QCOMPARE(Geometry::closest(rects, QPoint(200, 200)), 1);
    QCOMPARE(Geometry::closest(rects, QPoint(2, 50)), 0);
    QCOMPARE(Geometry::closest(rects, QPoint(400, 50)), 1);
    QCOMPARE(Geometry::closest(rects, QPoint(200, 275)), 1);
    QCOMPARE(Geometry::closest(rects, QPoint(110, 3)), 1);
    QCOMPARE(Geometry::closest(rects, QPoint(100, 3)), 0);
}

[[maybe_unused]] void GeometryTest::testConstrainPointToRect() {
    const QRect rect1;
    QCOMPARE(Geometry::constrain(&rect1, QPoint()), QPoint());
    QCOMPARE(Geometry::constrain(&rect1, QPoint(10, 20)), QPoint(10, 20));

    const QRect rect2(10, 20, 100, 200);
    QCOMPARE(Geometry::constrain(&rect2, QPoint(50, 100)), QPoint(50, 100));
    QCOMPARE(Geometry::constrain(&rect2, QPoint(5, 100)), QPoint(10, 100));
    QCOMPARE(Geometry::constrain(&rect2, QPoint(300, 100)), QPoint(109, 100));
    QCOMPARE(Geometry::constrain(&rect2, QPoint(30, 10)), QPoint(30, 20));
    QCOMPARE(Geometry::constrain(&rect2, QPoint(30, 700)), QPoint(30, 219));

}

[[maybe_unused]] void GeometryTest::testConstrainPointToRects() {
    std::vector<QRect*> rects;
    QCOMPARE(Geometry::constrain(rects, QPoint()), QPoint());
    QCOMPARE(Geometry::constrain(rects, QPoint(10, 20)), QPoint(10, 20));

    QRect rect1;
    QRect rect2;
    rects = { &rect1, &rect2 };
    QCOMPARE(Geometry::constrain(rects, QPoint()), QPoint());
    QCOMPARE(Geometry::constrain(rects, QPoint(10, 20)), QPoint(10, 20));

    QRect rect3(5, 10, 100, 200);
    rects = { &rect3 };
    QCOMPARE(Geometry::constrain(rects, QPoint(30, 40)), QPoint(30, 40));
    QCOMPARE(Geometry::constrain(rects, QPoint(0, 0)), QPoint(5, 10));
    QCOMPARE(Geometry::constrain(rects, QPoint(2, 400)), QPoint(5, 209));
    QCOMPARE(Geometry::constrain(rects, QPoint(300, 400)), QPoint(104, 209));

    QRect rect4(5, 5, 100, 200);
    QRect rect5(105, 5, 200, 300);
    rects = { &rect4, & rect5 };
    QCOMPARE(Geometry::constrain(rects, QPoint(50, 50)), QPoint(50, 50));
    QCOMPARE(Geometry::constrain(rects, QPoint(200, 200)), QPoint(200, 200));
    QCOMPARE(Geometry::constrain(rects, QPoint(2, 50)), QPoint(5, 50));
    QCOMPARE(Geometry::constrain(rects, QPoint(400, 50)), QPoint(304, 50));
    QCOMPARE(Geometry::constrain(rects, QPoint(200, 275)), QPoint(200, 275));
    QCOMPARE(Geometry::constrain(rects, QPoint(110, 3)), QPoint(110, 5));
    QCOMPARE(Geometry::constrain(rects, QPoint(100, 3)), QPoint(100, 5));
}

[[maybe_unused]] void GeometryTest::testRotateAround() {
    const QTransform transform = Geometry::rotateAround(20, 100, 200);
    QCOMPARE(transform.m11(), 0.939692620786);
    QCOMPARE(transform.m12(), 0.342020143326);
    QCOMPARE(transform.m13(), 0.0);
    QCOMPARE(transform.m21(), -0.342020143326);
    QCOMPARE(transform.m22(), 0.939692620786);
    QCOMPARE(transform.m23(), 0.0);
    QCOMPARE(transform.m31(), 74.4347665865);
    QCOMPARE(transform.m32(), -22.14053848976);
    QCOMPARE(transform.m33(), 1.0);
}


QTEST_MAIN(GeometryTest)

#include "GeometryTest.moc"
