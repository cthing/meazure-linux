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
#include <meazure/graphics/Plotter.h>
#include <QRect>
#include <QSize>
#include <vector>


Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class PlotterTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testPlotCrosshair();
};

//#include "pch.h"
//#define BOOST_TEST_MODULE PlotterTest
//#include "GlobalFixture.h"
//#include <boost/test/unit_test.hpp>
//#include <meazure/graphics/Plotter.h>
//#include <vector>
//#include <functional>
//
//
//bool operator==(const POINT& lhs, const POINT& rhs) {
//    return lhs.x == rhs.x && lhs.y == rhs.y;
//}
//
//std::ostream& operator<<(std::ostream& os, const POINT& point) {
//    os << '(' << point.x << ',' << point.y << ')';
//    return os;
//}
//
//
//BOOST_AUTO_TEST_CASE(TestPlotLine) {
//        std::vector<POINT> points;
//        std::function<void(int, int)> addPoint = [&](int x, int y) {
//            POINT pt { x, y };
//            points.push_back(pt);
//        };
//
//        POINT start1 { 1, 2 };
//        POINT end1 { 5, 2 };
//        points.clear();
//        MeaPlotter::PlotLine(start1, end1, addPoint);
//
//        BOOST_TEST(points.size() == 4);
//        BOOST_TEST(points[0] == (POINT { 2, 2 }));
//        BOOST_TEST(points[1] == (POINT { 3, 2 }));
//        BOOST_TEST(points[2] == (POINT { 4, 2 }));
//        BOOST_TEST(points[3] == (POINT { 5, 2 }));
//
//        POINT start2 { 1, 2 };
//        POINT end2 { 6, 5 };
//        points.clear();
//        MeaPlotter::PlotLine(start2, end2, addPoint);
//
//        BOOST_TEST(points.size() == 5);
//        BOOST_TEST(points[0] == (POINT { 2, 3 }));
//        BOOST_TEST(points[1] == (POINT { 3, 3 }));
//        BOOST_TEST(points[2] == (POINT { 4, 4 }));
//        BOOST_TEST(points[3] == (POINT { 5, 4 }));
//        BOOST_TEST(points[4] == (POINT { 6, 5 }));
//}
//
//BOOST_AUTO_TEST_CASE(TestPlotCircle) {
//        std::vector<POINT> points;
//        std::function<void(int, int)> addPoint = [&](int x, int y) {
//            POINT pt { x, y };
//            points.push_back(pt);
//        };
//
//        POINT center { 1, 2 };
//        MeaPlotter::PlotCircle(center, 3, addPoint);
//
//        BOOST_TEST(points.size() == 24);
//        BOOST_TEST(points[0] == (POINT { 4, 2 }));
//        BOOST_TEST(points[1] == (POINT { -2, 2 }));
//        BOOST_TEST(points[2] == (POINT { -2, 2 }));
//        BOOST_TEST(points[3] == (POINT { 4, 2 }));
//        BOOST_TEST(points[4] == (POINT { 1, 5 }));
//        BOOST_TEST(points[5] == (POINT { 1, 5 }));
//        BOOST_TEST(points[6] == (POINT { 1, -1 }));
//        BOOST_TEST(points[7] == (POINT { 1, -1 }));
//        BOOST_TEST(points[8] == (POINT { 4, 3 }));
//        BOOST_TEST(points[9] == (POINT { -2, 3 }));
//        BOOST_TEST(points[10] == (POINT { -2, 1 }));
//        BOOST_TEST(points[11] == (POINT { 4, 1 }));
//        BOOST_TEST(points[12] == (POINT { 2, 5 }));
//        BOOST_TEST(points[13] == (POINT { 0, 5 }));
//        BOOST_TEST(points[14] == (POINT { 0, -1 }));
//        BOOST_TEST(points[15] == (POINT { 2, -1 }));
//        BOOST_TEST(points[16] == (POINT { 3, 4 }));
//        BOOST_TEST(points[17] == (POINT { -1, 4 }));
//        BOOST_TEST(points[18] == (POINT { -1, 0 }));
//        BOOST_TEST(points[19] == (POINT { 3, 0 }));
//        BOOST_TEST(points[20] == (POINT { 3, 4 }));
//        BOOST_TEST(points[21] == (POINT { -1, 4 }));
//        BOOST_TEST(points[22] == (POINT { -1, 0 }));
//        BOOST_TEST(points[23] == (POINT { 3, 0 }));
//}

[[maybe_unused]] void PlotterTest::testPlotCrosshair() {
    std::vector<QRect> rects;
    const std::function<void(int, int, int, int)> addRect = [&](int x, int y, int w, int h) {
        const QRect rect(x, y, w, h);
        rects.push_back(rect);
    };

    const QSize size(13, 13);
    const QSize petalWidth(7, 7);
    Plotter::plotCrosshair(size, petalWidth, 2, addRect);

    QCOMPARE(rects.size(), 16);
    QCOMPARE(rects[0], QRect(3, 0, 7, 1));
    QCOMPARE(rects[1], QRect(4, 1, 5, 1));
    QCOMPARE(rects[2], QRect(5, 2, 3, 1));
    QCOMPARE(rects[3], QRect(6, 3, 1, 1));
    QCOMPARE(rects[4], QRect(6, 9, 1, 1));
    QCOMPARE(rects[5], QRect(5, 10, 3, 1));
    QCOMPARE(rects[6], QRect(4, 11, 5, 1));
    QCOMPARE(rects[7], QRect(3, 12, 7, 1));
    QCOMPARE(rects[8], QRect(0, 3, 1, 7));
    QCOMPARE(rects[9], QRect(1, 4, 1, 5));
    QCOMPARE(rects[10], QRect(2, 5, 1, 3));
    QCOMPARE(rects[11], QRect(3, 6, 1, 1));
    QCOMPARE(rects[12], QRect(9, 6, 1, 1));
    QCOMPARE(rects[13], QRect(10, 5, 1, 3));
    QCOMPARE(rects[14], QRect(11, 4, 1, 5));
    QCOMPARE(rects[15], QRect(12, 3, 1, 7));
}


QTEST_MAIN(PlotterTest)

#include "PlotterTest.moc"
