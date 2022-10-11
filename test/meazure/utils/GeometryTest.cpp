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
#include <meazure/utils/Geometry.h>
#include <vector>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class GeometryTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testArea();
    [[maybe_unused]] void testDistance();
    [[maybe_unused]] void testContainsPoint();
    [[maybe_unused]] void testContainsRect();
    [[maybe_unused]] void testClosestRect();
    [[maybe_unused]] void testConstrainPointToRect();
    [[maybe_unused]] void testConstrainPointToRects();
};


[[maybe_unused]] void GeometryTest::testArea() {
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


QTEST_MAIN(GeometryTest)

#include "GeometryTest.moc"
