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
    [[maybe_unused]] void testContainsPoint();
    [[maybe_unused]] void testContainsRect();
};


[[maybe_unused]] void GeometryTest::testArea() {
    QCOMPARE(Geometry::area(QRect()), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 0, 0)), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 1, 0)), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 0, 1)), 0);
    QCOMPARE(Geometry::area(QRect(0, 0, 10, 20)), 200);
    QCOMPARE(Geometry::area(QRect(1, 2, 10, 20)), 200);
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


QTEST_MAIN(GeometryTest)

#include "GeometryTest.moc"
