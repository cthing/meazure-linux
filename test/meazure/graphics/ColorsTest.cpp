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
#include <meazure/graphics/Colors.h>
#include <cmath>


Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
Q_IMPORT_PLUGIN(QSvgIconPlugin)


class ColorsTest : public QObject {

Q_OBJECT

private slots:
    [[maybe_unused]] void testCMY();
    [[maybe_unused]] void testCMYK();
    [[maybe_unused]] void testHSL();
    [[maybe_unused]] void testYCbCr();
    [[maybe_unused]] void testYIQ();
    [[maybe_unused]] void testXYZ();
    [[maybe_unused]] void testLab();
    [[maybe_unused]] void testRGBtoCMY_data();
    [[maybe_unused]] void testRGBtoCMY();
    [[maybe_unused]] void testRGBtoCMYK_data();
    [[maybe_unused]] void testRGBtoCMYK();
    [[maybe_unused]] void testRGBtoHSL_data();
    [[maybe_unused]] void testRGBtoHSL();
    [[maybe_unused]] void testHSLtoRGB_data();
    [[maybe_unused]] void testHSLtoRGB();
    [[maybe_unused]] void testRGBtoYCbCr_data();
    [[maybe_unused]] void testRGBtoYCbCr();
    [[maybe_unused]] void testRGBtoYIQ_data();
    [[maybe_unused]] void testRGBtoYIQ();
    [[maybe_unused]] void testRGBtoY_data();
    [[maybe_unused]] void testRGBtoY();
    [[maybe_unused]] void testRGBtoXYZ_data();
    [[maybe_unused]] void testRGBtoXYZ();
    [[maybe_unused]] void testXYZtoLab_data();
    [[maybe_unused]] void testXYZtoLab();
    [[maybe_unused]] void testRGBtoLab_data();
    [[maybe_unused]] void testRGBtoLab();
    [[maybe_unused]] void testInterpolateColor();
    [[maybe_unused]] void testColorDifferenceLab_data();
    [[maybe_unused]] void testColorDifferenceLab();
    [[maybe_unused]] void testMatchBasicColor();
    [[maybe_unused]] void testMatchExtendedColor();
    [[maybe_unused]] void testColorItem();
    [[maybe_unused]] void testOpacity();
    [[maybe_unused]] void testOpacityConversion();

private:
    constexpr double roundf(double value) { return std::round(value * 10000.0) / 10000.0; }
};


[[maybe_unused]] void ColorsTest::testCMY() {
    const Colors::CMY cmy1;
    QCOMPARE(cmy1.cyan, 0);
    QCOMPARE(cmy1.magenta, 0);
    QCOMPARE(cmy1.yellow, 0);

    const Colors::CMY cmy2(10, 255, 0);
    QCOMPARE(cmy2.cyan, 10);
    QCOMPARE(cmy2.magenta, 255);
    QCOMPARE(cmy2.yellow, 0);
}

[[maybe_unused]] void ColorsTest::testCMYK() {
    const Colors::CMYK cmyk1;
    QCOMPARE(cmyk1.cyan, 0);
    QCOMPARE(cmyk1.magenta, 0);
    QCOMPARE(cmyk1.yellow, 0);
    QCOMPARE(cmyk1.black, 0);

    const Colors::CMYK cmyk2(10, 255, 0, 100);
    QCOMPARE(cmyk2.cyan, 10);
    QCOMPARE(cmyk2.magenta, 255);
    QCOMPARE(cmyk2.yellow, 0);
    QCOMPARE(cmyk2.black, 100);
}

[[maybe_unused]] void ColorsTest::testHSL() {
    const Colors::HSL hsl1;
    QCOMPARE(hsl1.hue, 0);
    QCOMPARE(hsl1.saturation, 0);
    QCOMPARE(hsl1.lightness, 0);

    const Colors::HSL hsl2(1, 2, 3);
    QCOMPARE(hsl2.hue, 1);
    QCOMPARE(hsl2.saturation, 2);
    QCOMPARE(hsl2.lightness, 3);
}

[[maybe_unused]] void ColorsTest::testYCbCr() {
    const Colors::YCbCr ycbcr1;
    QCOMPARE(ycbcr1.y, 0);
    QCOMPARE(ycbcr1.cb, 0);
    QCOMPARE(ycbcr1.cr, 0);

    const Colors::YCbCr ycbcr2(20, 30, 40);
    QCOMPARE(ycbcr2.y, 20);
    QCOMPARE(ycbcr2.cb, 30);
    QCOMPARE(ycbcr2.cr, 40);
}

[[maybe_unused]] void ColorsTest::testYIQ() {
    const Colors::YIQ yiq1;
    QCOMPARE(yiq1.y, 0);
    QCOMPARE(yiq1.i, 0);
    QCOMPARE(yiq1.q, 0);

    const Colors::YIQ yiq2(20, 30, 40);
    QCOMPARE(yiq2.y, 20);
    QCOMPARE(yiq2.i, 30);
    QCOMPARE(yiq2.q, 40);
}

[[maybe_unused]] void ColorsTest::testXYZ() {
    const Colors::XYZ xyz1;
    QCOMPARE(xyz1.x, 0.0);
    QCOMPARE(xyz1.y, 0.0);
    QCOMPARE(xyz1.z, 0.0);

    const Colors::XYZ xyz2(25.0, 0.50, 7.5);
    QCOMPARE(xyz2.x, 25.0);
    QCOMPARE(xyz2.y, 0.50);
    QCOMPARE(xyz2.z, 7.5);
}

[[maybe_unused]] void ColorsTest::testLab() {
    const Colors::Lab lab1;
    QCOMPARE(lab1.l, 0.0);
    QCOMPARE(lab1.a, 0.0);
    QCOMPARE(lab1.b, 0.0);

    const Colors::Lab lab2(25.0, 0.50, 7.5);
    QCOMPARE(lab2.l, 25.0);
    QCOMPARE(lab2.a, 0.50);
    QCOMPARE(lab2.b, 7.5);
}

[[maybe_unused]] void ColorsTest::testRGBtoCMY_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("cyan");
    QTest::addColumn<int>("magenta");
    QTest::addColumn<int>("yellow");

    QTest::newRow("255,255,255 -> 0,0,0")    << qRgb(255, 255, 255) <<   0 <<   0 <<   0;
    QTest::newRow("0,0,0 -> 255,255,255")    << qRgb(  0,   0,   0) << 255 << 255 << 255;
    QTest::newRow("127,245,255 -> 128,10,0") << qRgb(127, 245, 255) << 128 <<  10 <<   0;
    QTest::newRow("254,253,252 -> 1,2,3")    << qRgb(254, 253, 252) <<   1 <<   2 <<   3;
}

[[maybe_unused]] void ColorsTest::testRGBtoCMY() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, cyan);
    const QFETCH(int, magenta);
    const QFETCH(int, yellow);

    const Colors::CMY cmy = Colors::RGBtoCMY(rgb);
    QCOMPARE(cmy.cyan, cyan);
    QCOMPARE(cmy.magenta, magenta);
    QCOMPARE(cmy.yellow, yellow);
}

[[maybe_unused]] void ColorsTest::testRGBtoCMYK_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("cyan");
    QTest::addColumn<int>("magenta");
    QTest::addColumn<int>("yellow");
    QTest::addColumn<int>("black");

    QTest::newRow("255,255,255 -> 0,0,0,0")     << qRgb(255, 255, 255) <<   0 <<  0 <<   0 <<   0;
    QTest::newRow("0,0,0 -> 0,0,0,255")         << qRgb(  0,   0,   0) <<   0 <<  0 <<   0 << 255;
    QTest::newRow("127,245,255 -> 128,10,0,0")  << qRgb(127, 245, 255) << 128 << 10 <<   0 <<   0;
    QTest::newRow("113,142,180 -> 95,54,0,75")  << qRgb(113, 142, 180) <<  95<<  54 <<   0 <<  75;
    QTest::newRow("38,235,114 -> 214,0,131,20") << qRgb( 38, 235, 114) << 214 <<  0 << 131 <<  20;
}

[[maybe_unused]] void ColorsTest::testRGBtoCMYK() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, cyan);
    const QFETCH(int, magenta);
    const QFETCH(int, yellow);
    const QFETCH(int, black);

    const Colors::CMYK cmyk = Colors::RGBtoCMYK(rgb);
    QCOMPARE(cmyk.cyan, cyan);
    QCOMPARE(cmyk.magenta, magenta);
    QCOMPARE(cmyk.yellow, yellow);
    QCOMPARE(cmyk.black, black);
}

[[maybe_unused]] void ColorsTest::testRGBtoHSL_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("hue");
    QTest::addColumn<int>("saturation");
    QTest::addColumn<int>("lightness");

    QTest::newRow("0,0,0 -> 0,0,0")          << qRgb(  0,   0,   0) <<   0 <<   0 <<   0;
    QTest::newRow("255,0,0 -> 0,100,50")     << qRgb(255,   0,   0) <<   0 << 100 <<  50;
    QTest::newRow("0,255,0 -> 120,100,50")   << qRgb(  0, 255,   0) << 120 << 100 <<  50;
    QTest::newRow("0,0,255 -> 240,100,50")   << qRgb(  0,   0, 255) << 240 << 100 <<  50;
    QTest::newRow("255,255,255 -> 0,0,100")  << qRgb(255, 255, 255) <<   0 <<   0 << 100;
    QTest::newRow("128,0,0 -> 0,100,25")     << qRgb(128,   0,   0) <<   0 << 100 <<  25;
    QTest::newRow("0,128,0 -> 120,100,25")   << qRgb(  0, 128,   0) << 120 << 100 <<  25;
    QTest::newRow("0,0,128 -> 240,100,25")   << qRgb(  0,   0, 128) << 240 << 100 <<  25;
    QTest::newRow("50,150,200 -> 200,60,49") << qRgb( 50, 150, 200) << 200 <<  60 <<  49;
}

[[maybe_unused]] void ColorsTest::testRGBtoHSL() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, hue);
    const QFETCH(int, saturation);
    const QFETCH(int, lightness);

    const Colors::HSL hsl = Colors::RGBtoHSL(rgb);
    QCOMPARE(hsl.hue, hue);
    QCOMPARE(hsl.saturation, saturation);
    QCOMPARE(hsl.lightness, lightness);
}

[[maybe_unused]] void ColorsTest::testHSLtoRGB_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("hue");
    QTest::addColumn<int>("saturation");
    QTest::addColumn<int>("lightness");

    QTest::newRow("0,0,0 -> 0,0,0")          << qRgb(  0,   0,   0) << 0   << 0   << 0;
    QTest::newRow("255,0,0 -> 0,100,50")     << qRgb(255,   0,   0) << 0   << 100 << 50;
    QTest::newRow("0,255,0 -> 120,100,50")   << qRgb(  0, 255,   0) << 120 << 100 << 50;
    QTest::newRow("0,0,255 -> 240,100,50")   << qRgb(  0,   0, 255) << 240 << 100 << 50;
    QTest::newRow("255,255,255 -> 0,0,100")  << qRgb(255, 255, 255) << 0   << 0   << 100;
    QTest::newRow("128,0,0 -> 0,100,25")     << qRgb(128,   0,   0) << 0   << 100 << 25;
    QTest::newRow("0,128,0 -> 120,100,25")   << qRgb(  0, 128,   0) << 120 << 100 << 25;
    QTest::newRow("0,0,128 -> 240,100,25")   << qRgb(  0,   0, 128) << 240 << 100 << 25;
    QTest::newRow("50,150,200 -> 200,60,49") << qRgb( 50, 150, 200) << 200 << 60  << 49;
}

[[maybe_unused]] void ColorsTest::testHSLtoRGB() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, hue);
    const QFETCH(int, saturation);
    const QFETCH(int, lightness);

    const QRgb actualRgb = Colors::HSLtoRGB(Colors::HSL(hue, saturation, lightness));
    QCOMPARE(actualRgb, rgb);
}

[[maybe_unused]] void ColorsTest::testRGBtoYCbCr_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("cb");
    QTest::addColumn<int>("cr");

    QTest::newRow("0,0,0 -> 16,128,128")        << qRgb(  0,   0,   0) <<  16 << 128 << 128;
    QTest::newRow("255,255,255 -> 235,128,128") << qRgb(255, 255, 255) << 235 << 128 << 128;
    QTest::newRow("128,128,128 -> 126,128,128") << qRgb(128, 128, 128) << 126 << 128 << 128;
    QTest::newRow("255,0,0 -> 82,90,240")       << qRgb(255,   0,   0) <<  82 <<  90 << 240;
    QTest::newRow("0,255,0 -> 145,54,34")       << qRgb(  0, 255,   0) << 145 <<  54 <<  34;
    QTest::newRow("0,0,255 -> 41,240,110")      << qRgb(  0,   0, 255) <<  41 << 240 << 110;
    QTest::newRow("255,255,0 -> 210,16,146")    << qRgb(255, 255,   0) << 210 <<  16 << 146;
    QTest::newRow("0,255,255 -> 170,166,16")    << qRgb(  0, 255, 255) << 170 << 166 <<  16;
    QTest::newRow("255,0,255 -> 107,202,222")   << qRgb(255,   0, 255) << 107 << 202 << 222;
}

[[maybe_unused]] void ColorsTest::testRGBtoYCbCr() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, y);
    const QFETCH(int, cb);
    const QFETCH(int, cr);

    const Colors::YCbCr ycbcr = Colors::RGBtoYCbCr(rgb);
    QCOMPARE(ycbcr.y, y);
    QCOMPARE(ycbcr.cb, cb);
    QCOMPARE(ycbcr.cr, cr);
}

[[maybe_unused]] void ColorsTest::testRGBtoYIQ_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("y");
    QTest::addColumn<int>("i");
    QTest::addColumn<int>("q");

    QTest::newRow("0,0,0 -> 0,0,0")            << qRgb(  0,   0,   0) <<   0 <<    0 <<    0;
    QTest::newRow("255,255,255 -> 255,0,0")    << qRgb(255, 255, 255) << 255 <<    0 <<    0;
    QTest::newRow("128,128,128 -> 128,0,0")    << qRgb(128, 128, 128) << 128 <<    0 <<    0;
    QTest::newRow("255,0,0 -> 76,152,54")      << qRgb(255,   0,   0) <<  76 <<  152 <<   54;
    QTest::newRow("0,255,0 -> 150,-70,-133")   << qRgb(  0, 255,   0) << 150 <<  -70 << -133;
    QTest::newRow("0,0,255 -> 29,-82,79")      << qRgb(  0,   0, 255) <<  29 <<  -82 <<   79;
    QTest::newRow("255,255,0 -> 226,82,-79")   << qRgb(255, 255,   0) << 226 <<   82 <<  -79;
    QTest::newRow("0,255,255 -> 179,-152,-54") << qRgb(  0, 255, 255) << 179 << -152 <<  -54;
    QTest::newRow("255,0,255 -> 105,70,133")   << qRgb(255,   0, 255) << 105 <<   70 <<  133;
}

[[maybe_unused]] void ColorsTest::testRGBtoYIQ() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, y);
    const QFETCH(int, i);
    const QFETCH(int, q);

    const Colors::YIQ yiq = Colors::RGBtoYIQ(rgb);
    QCOMPARE(yiq.y, y);
    QCOMPARE(yiq.i, i);
    QCOMPARE(yiq.q, q);
}

[[maybe_unused]] void ColorsTest::testRGBtoY_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<int>("yExpected");

    QTest::newRow("0,0,0 -> 0")         << qRgb(  0,   0,   0) <<   0;
    QTest::newRow("255,255,255 -> 255") << qRgb(255, 255, 255) << 255;
    QTest::newRow("128,128,128 -> 128") << qRgb(128, 128, 128) << 128;
    QTest::newRow("255,0,0 -> 76")      << qRgb(255,   0,   0) <<  76;
    QTest::newRow("0,255,0 -> 150")     << qRgb(  0, 255,   0) << 150;
    QTest::newRow("0,0,255 -> 29")      << qRgb(  0,   0, 255) <<  29;
    QTest::newRow("255,255,0 -> 226")   << qRgb(255, 255,   0) << 226;
    QTest::newRow("0,255,255 -> 179")   << qRgb(  0, 255, 255) << 179;
    QTest::newRow("255,0,255 -> 105")   << qRgb(255,   0, 255) << 105;
}

[[maybe_unused]] void ColorsTest::testRGBtoY() {
    const QFETCH(QRgb, rgb);
    const QFETCH(int, yExpected);

    const int y = Colors::RGBtoY(rgb);
    QCOMPARE(y, yExpected);
}

[[maybe_unused]] void ColorsTest::testRGBtoXYZ_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<double>("x");
    QTest::addColumn<double>("y");
    QTest::addColumn<double>("z");

    QTest::newRow("0,0,0 -> 0.0000,0.0000,0.0000")            << qRgb(  0,   0,   0) <<  0.0000 <<   0.0000 <<   0.0000;
    QTest::newRow("255,255,255 -> 95.0470,100.0000,108.8830") << qRgb(255, 255, 255) << 95.0470 << 100.0000 << 108.8830;
    QTest::newRow("128,128,128 -> 20.5169,21.5861,23.5035")   << qRgb(128, 128, 128) << 20.5169 <<  21.5861 <<  23.5035;
    QTest::newRow("255,0,0 -> 41.2456,21.2673,1.9334")        << qRgb(255,   0,   0) << 41.2456 <<  21.2673 <<   1.9334;
    QTest::newRow("0,255,0 -> 35.7576,71.5152,11.9192")       << qRgb(  0, 255,   0) << 35.7576 <<  71.5152 <<  11.9192;
    QTest::newRow("0,0,255 -> 18.0438,7.2175,95.0304")        << qRgb(  0,   0, 255) << 18.0438 <<   7.2175 <<  95.0304;
    QTest::newRow("255,255,0 -> 77.0033,92.7825,13.8526")     << qRgb(255, 255,   0) << 77.0033 <<  92.7825 <<  13.8526;
    QTest::newRow("0,255,255 -> 53.8014,78.7327,106.9496")    << qRgb(  0, 255, 255) << 53.8014 <<  78.7327 << 106.9496;
    QTest::newRow("255,0,255 -> 59.2894,28.4848,96.9638")     << qRgb(255,   0, 255) << 59.2894 <<  28.4848 <<  96.9638;
}

[[maybe_unused]] void ColorsTest::testRGBtoXYZ() {
    const QFETCH(QRgb, rgb);
    const QFETCH(double, x);
    const QFETCH(double, y);
    const QFETCH(double, z);

    const Colors::XYZ xyz = Colors::RGBtoXYZ(rgb);
    QCOMPARE(roundf(xyz.x), x);
    QCOMPARE(roundf(xyz.y), y);
    QCOMPARE(roundf(xyz.z), z);
}

[[maybe_unused]] void ColorsTest::testXYZtoLab_data() {
    QTest::addColumn<double>("x");
    QTest::addColumn<double>("y");
    QTest::addColumn<double>("z");
    QTest::addColumn<double>("l");
    QTest::addColumn<double>("a");
    QTest::addColumn<double>("b");

    QTest::newRow("0.0000,0.0000,0.0000 -> 0.0000,0.0000,0.0000")          <<  0.0000 <<   0.0000 <<   0.0000 <<   0.0000 <<   0.0000 <<    0.0000;
    QTest::newRow("95.0470,100.0000,108.8830 -> 100.0000,0.0000,0.0000")   << 95.0470 << 100.0000 << 108.8830 << 100.0000 <<   0.0000 <<    0.0000;
    QTest::newRow("20.5169,21.5861,23.5035 -> 53.5851,-0.0002,0.0002")     << 20.5169 <<  21.5861 <<  23.5035 <<  53.5851 <<  -0.0002 <<    0.0002;
    QTest::newRow("41.2456,21.2673,1.9334 -> 53.2408,80.0923,67.2031")     << 41.2456 <<  21.2673 <<   1.9334 <<  53.2408 <<  80.0923 <<   67.2031;
    QTest::newRow("35.7576,71.5152,11.9192 -> 87.7347,-86.1827,83.1793")   << 35.7576 <<  71.5152 <<  11.9192 <<  87.7347 << -86.1827 <<   83.1793;
    QTest::newRow("18.0438,7.2175,95.0304 -> 32.2970,79.1878,-107.8602")   << 18.0438 <<   7.2175 <<  95.0304 <<  32.2970 <<  79.1878 << -107.8602;
    QTest::newRow("77.0033,92.7825,13.8526 -> 97.1393,-21.5536,94.4779")   << 77.0033 <<  92.7825 <<  13.8526 <<  97.1393 << -21.5536 <<   94.4779;
    QTest::newRow("53.8014,78.7327,106.9496 -> 91.1132,-48.0874,-14.1312") << 53.8014 <<  78.7327 << 106.9496 <<  91.1132 << -48.0874 <<  -14.1312;
    QTest::newRow("59.2894,28.4848,96.9638 -> 60.3242,98.2343,-60.8249")   << 59.2894 <<  28.4848 <<  96.9638 <<  60.3242 <<  98.2343 <<  -60.8249;
    QTest::newRow("0.6096,0.6585,1.3230 -> 5.9482,-0.6671,-8.1376")        <<  0.6096 <<   0.6585 <<   1.3230 <<   5.9482 <<  -0.6671 <<   -8.1376;
    QTest::newRow("25.5444,28.1543,58.7206 -> 60.0279,-5.0385,-31.7121")   << 25.5444 <<  28.1543 <<  58.7206 <<  60.0279 <<  -5.0385 <<  -31.7121;
    QTest::newRow("31.2431,24.1726,12.6913 -> 56.2602,33.6047,26.8901")    << 31.2431 <<  24.1726 <<  12.6913 <<  56.2602 <<  33.6047 <<   26.8901;
    QTest::newRow("25.6330,47.1587,19.8057 -> 74.2912,-66.1455,42.3544")   << 25.6330 <<  47.1587 <<  19.8057 <<  74.2912 << -66.1455 <<   42.3544;
}

[[maybe_unused]] void ColorsTest::testXYZtoLab() {
    const QFETCH(double, x);
    const QFETCH(double, y);
    const QFETCH(double, z);
    const QFETCH(double, l);
    const QFETCH(double, a);
    const QFETCH(double, b);

    const Colors::XYZ xyz(x, y, z);
    const Colors::Lab lab = Colors::XYZtoLab(xyz);
    QCOMPARE(roundf(lab.l), l);
    QCOMPARE(roundf(lab.a), a);
    QCOMPARE(roundf(lab.b), b);
}

[[maybe_unused]] void ColorsTest::testRGBtoLab_data() {
    QTest::addColumn<QRgb>("rgb");
    QTest::addColumn<double>("l");
    QTest::addColumn<double>("a");
    QTest::addColumn<double>("b");

    QTest::newRow("0,0,0 -> 0.0000,0.0000,0.0000")          << qRgb(  0,   0,   0) <<   0.0000 <<   0.0000 <<    0.0000;
    QTest::newRow("255,255,255 -> 100.0000,0.0000,0.0000")  << qRgb(255, 255, 255) << 100.0000 <<   0.0000 <<    0.0000;
    QTest::newRow("128,128,128 -> 53.5850,0.0000,0.0000")   << qRgb(128, 128, 128) <<  53.5850 <<   0.0000 <<    0.0000;
    QTest::newRow("255,0,0 -> 53.2408,80.0925,67.2032")     << qRgb(255,   0,   0) <<  53.2408 <<  80.0925 <<   67.2032;
    QTest::newRow("0,255,0 -> 87.7347,-86.1827,83.1793")    << qRgb(  0, 255,   0) <<  87.7347 << -86.1827 <<   83.1793;
    QTest::newRow("0,0,255 -> 32.2970,79.1875,-107.8602")   << qRgb(  0,   0, 255) <<  32.2970 <<  79.1875 << -107.8602;
    QTest::newRow("255,255,0 -> 97.1393,-21.5537,94.4780")  << qRgb(255, 255,   0) <<  97.1393 << -21.5537 <<   94.4780;
    QTest::newRow("0,255,255 -> 91.1132,-48.0875,-14.1312") << qRgb(  0, 255, 255) <<  91.1132 << -48.0875 <<  -14.1312;
    QTest::newRow("255,0,255 -> 60.3242,98.2343,-60.8249")  << qRgb(255,   0, 255) <<  60.3242 <<  98.2343 <<  -60.8249;
    QTest::newRow("10,20,30 -> 5.9485,-0.6687,-8.1374")     << qRgb( 10,  20,  30) <<   5.9485 <<  -0.6687 <<   -8.1374;
    QTest::newRow("90,150,200 -> 60.0279,-5.0388,-31.7120") << qRgb( 90, 150, 200) <<  60.0279 <<  -5.0388 <<  -31.7120;
    QTest::newRow("200,110,90 -> 56.2602,33.6045,26.8902")  << qRgb(200, 110,  90) <<  56.2602 <<  33.6045 <<   26.8902;
    QTest::newRow("20,210,100 -> 74.2911,-66.1453,42.3544") << qRgb( 20, 210, 100) <<  74.2911 << -66.1453 <<   42.3544;
}

[[maybe_unused]] void ColorsTest::testRGBtoLab() {
    const QFETCH(QRgb, rgb);
    const QFETCH(double, l);
    const QFETCH(double, a);
    const QFETCH(double, b);

    const Colors::Lab lab = Colors::RGBtoLab(rgb);
    QCOMPARE(roundf(lab.l), l);
    QCOMPARE(roundf(lab.a), a);
    QCOMPARE(roundf(lab.b), b);
}

[[maybe_unused]] void ColorsTest::testInterpolateColor() {
    QRgb color = Colors::interpolateColor(qRgb(0, 0, 0), qRgb(255, 255, 255), 50);
    QCOMPARE(color, qRgb(128, 128, 128));

    color = Colors::interpolateColor(qRgb(0, 0, 0), qRgb(255, 255, 255), 0);
    QCOMPARE(color, qRgb(0, 0, 0));

    color = Colors::interpolateColor(qRgb(0, 0, 0), qRgb(255, 255, 255), 100);
    QCOMPARE(color, qRgb(255, 255, 255));

    color = Colors::interpolateColor(qRgb(0, 0, 0), qRgb(255, 255, 255), 10);
    QCOMPARE(color, qRgb(26, 26, 26));

    color = Colors::interpolateColor(qRgb(10, 20, 30), qRgb(200, 150, 100), 50);
    QCOMPARE(color, qRgb(44, 129, 44));
}

// Test data from https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/dataNprograms/ciede2000testdata.txt
[[maybe_unused]] void ColorsTest::testColorDifferenceLab_data() {
    QTest::addColumn<double>("l1");
    QTest::addColumn<double>("a1");
    QTest::addColumn<double>("b1");
    QTest::addColumn<double>("l2");
    QTest::addColumn<double>("a2");
    QTest::addColumn<double>("b2");
    QTest::addColumn<double>("expectedDiff");

    QTest::newRow("(50.0000,2.6772,-79.7751) - (50.0000,0.0000,-82.7485) =  2.0425")   << 50.0000 <<   2.6772 << -79.7751 << 50.0000 <<   0.0000 << -82.7485 <<  2.0425;
    QTest::newRow("(50.0000,3.1571,-77.2803) - (50.0000,0.0000,-82.7485) =  2.8615")   << 50.0000 <<   3.1571 << -77.2803 << 50.0000 <<   0.0000 << -82.7485 <<  2.8615;
    QTest::newRow("(50.0000,2.8361,-74.0200) - (50.0000,0.0000,-82.7485) =  3.4412")   << 50.0000 <<   2.8361 << -74.0200 << 50.0000 <<   0.0000 << -82.7485 <<  3.4412;
    QTest::newRow("(50.0000,-1.3802,-84.2814) - (50.0000,0.0000,-82.7485) =  1.0000")  << 50.0000 <<  -1.3802 << -84.2814 << 50.0000 <<   0.0000 << -82.7485 <<  1.0000;
    QTest::newRow("(50.0000,-1.1848,-84.8006) - (50.0000,0.0000,-82.7485) =  1.0000")  << 50.0000 <<  -1.1848 << -84.8006 << 50.0000 <<   0.0000 << -82.7485 <<  1.0000;
    QTest::newRow("(50.0000,-0.9009,-85.5211) - (50.0000,0.0000,-82.7485) =  1.0000")  << 50.0000 <<  -0.9009 << -85.5211 << 50.0000 <<   0.0000 << -82.7485 <<  1.0000;
    QTest::newRow("(50.0000,0.0000,0.0000) - (50.0000,-1.0000,2.0000) =  2.3669")      << 50.0000 <<   0.0000 <<   0.0000 << 50.0000 <<  -1.0000 <<   2.0000 <<  2.3669;
    QTest::newRow("(50.0000,-1.0000,2.0000) - (50.0000,0.0000,0.0000) =  2.3669")      << 50.0000 <<  -1.0000 <<   2.0000 << 50.0000 <<   0.0000 <<   0.0000 <<  2.3669;
    QTest::newRow("(50.0000,2.4900,-0.0010) - (50.0000,-2.4900,0.0009) =  7.1792")     << 50.0000 <<   2.4900 <<  -0.0010 << 50.0000 <<  -2.4900 <<   0.0009 <<  7.1792;
    QTest::newRow("(50.0000,2.4900,-0.0010) - (50.0000,-2.4900,0.0010) =  7.1792")     << 50.0000 <<   2.4900 <<  -0.0010 << 50.0000 <<  -2.4900 <<   0.0010 <<  7.1792;
    QTest::newRow("(50.0000,2.4900,-0.0010) - (50.0000,-2.4900,0.0011) =  7.2195")     << 50.0000 <<   2.4900 <<  -0.0010 << 50.0000 <<  -2.4900 <<   0.0011 <<  7.2195;
    QTest::newRow("(50.0000,2.4900,-0.0010) - (50.0000,-2.4900,0.0012) =  7.2195")     << 50.0000 <<   2.4900 <<  -0.0010 << 50.0000 <<  -2.4900 <<   0.0012 <<  7.2195;
    QTest::newRow("(50.0000,-0.0010,2.4900) - (50.0000,0.0009,-2.4900) =  4.8045")     << 50.0000 <<  -0.0010 <<   2.4900 << 50.0000 <<   0.0009 <<  -2.4900 <<  4.8045;
    QTest::newRow("(50.0000,-0.0010,2.4900) - (50.0000,0.0010,-2.4900) =  4.8045")     << 50.0000 <<  -0.0010 <<   2.4900 << 50.0000 <<   0.0010 <<  -2.4900 <<  4.8045;
    QTest::newRow("(50.0000,-0.0010,2.4900) - (50.0000,0.0011,-2.4900) =  4.7461")     << 50.0000 <<  -0.0010 <<   2.4900 << 50.0000 <<   0.0011 <<  -2.4900 <<  4.7461;
    QTest::newRow("(50.0000,2.5000,0.0000) - (50.0000,0.0000,-2.5000) =  4.3065")      << 50.0000 <<   2.5000 <<   0.0000 << 50.0000 <<   0.0000 <<  -2.5000 <<  4.3065;
    QTest::newRow("(50.0000,2.5000,0.0000) - (73.0000,25.0000,-18.0000) = 27.1492")    << 50.0000 <<   2.5000 <<   0.0000 << 73.0000 <<  25.0000 << -18.0000 << 27.1492;
    QTest::newRow("(50.0000,2.5000,0.0000) - (61.0000,-5.0000,29.0000) = 22.8977")     << 50.0000 <<   2.5000 <<   0.0000 << 61.0000 <<  -5.0000 <<  29.0000 << 22.8977;
    QTest::newRow("(50.0000,2.5000,0.0000) - (56.0000,-27.0000,-3.0000) = 31.9030")    << 50.0000 <<   2.5000 <<   0.0000 << 56.0000 << -27.0000 <<  -3.0000 << 31.9030;
    QTest::newRow("(50.0000,2.5000,0.0000) - (58.0000,24.0000,15.0000) = 19.4535")     << 50.0000 <<   2.5000 <<   0.0000 << 58.0000 <<  24.0000 <<  15.0000 << 19.4535;
    QTest::newRow("(50.0000,2.5000,0.0000) - (50.0000,3.1736,0.5854) =  1.0000")       << 50.0000 <<   2.5000 <<   0.0000 << 50.0000 <<   3.1736 <<   0.5854 <<  1.0000;
    QTest::newRow("(50.0000,2.5000,0.0000) - (50.0000,3.2972,0.0000) =  1.0000")       << 50.0000 <<   2.5000 <<   0.0000 << 50.0000 <<   3.2972 <<   0.0000 <<  1.0000;
    QTest::newRow("(50.0000,2.5000,0.0000) - (50.0000,1.8634,0.5757) =  1.0000")       << 50.0000 <<   2.5000 <<   0.0000 << 50.0000 <<   1.8634 <<   0.5757 <<  1.0000;
    QTest::newRow("(50.0000,2.5000,0.0000) - (50.0000,3.2592,0.3350) =  1.0000")       << 50.0000 <<   2.5000 <<   0.0000 << 50.0000 <<   3.2592 <<   0.3350 <<  1.0000;
    QTest::newRow("(60.2574,-34.0099,36.2677) - (60.4626,-34.1751,39.4387) =  1.2644") << 60.2574 << -34.0099 <<  36.2677 << 60.4626 << -34.1751 <<  39.4387 <<  1.2644;
    QTest::newRow("(63.0109,-31.0961,-5.8663) - (62.8187,-29.7946,-4.0864) =  1.2630") << 63.0109 << -31.0961 <<  -5.8663 << 62.8187 << -29.7946 <<  -4.0864 <<  1.2630;
    QTest::newRow("(61.2901,3.7196,-5.3901) - (61.4292,2.2480,-4.9620) =  1.8731")     << 61.2901 <<   3.7196 <<  -5.3901 << 61.4292 <<   2.2480 <<  -4.9620 <<  1.8731;
    QTest::newRow("(35.0831,-44.1164,3.7933) - (35.0232,-40.0716,1.5901) =  1.8645")   << 35.0831 << -44.1164 <<   3.7933 << 35.0232 << -40.0716 <<   1.5901 <<  1.8645;
    QTest::newRow("(22.7233,20.0904,-46.6940) - (23.0331,14.9730,-42.5619) =  2.0373") << 22.7233 <<  20.0904 << -46.6940 << 23.0331 <<  14.9730 << -42.5619 <<  2.0373;
    QTest::newRow("(36.4612,47.8580,18.3852) - (36.2715,50.5065,21.2231) =  1.4146")   << 36.4612 <<  47.8580 <<  18.3852 << 36.2715 <<  50.5065 <<  21.2231 <<  1.4146;
    QTest::newRow("(90.8027,-2.0831,1.4410) - (91.1528,-1.6435,0.0447) =  1.4441")     << 90.8027 <<  -2.0831 <<   1.4410 << 91.1528 <<  -1.6435 <<   0.0447 <<  1.4441;
    QTest::newRow("(90.9257,-0.5406,-0.9208) - (88.6381,-0.8985,-0.7239) =  1.5381")   << 90.9257 <<  -0.5406 <<  -0.9208 << 88.6381 <<  -0.8985 <<  -0.7239 <<  1.5381;
    QTest::newRow("(6.7747,-0.2908,-2.4247) - ( 5.8714,-0.0985,-2.2286) =  0.6377")    <<  6.7747 <<  -0.2908 <<  -2.4247 <<  5.8714 <<  -0.0985 <<  -2.2286 <<  0.6377;
    QTest::newRow("(2.0776,0.0795,-1.1350) - ( 0.9033,-0.0636,-0.5514) =  0.9082")     <<  2.0776 <<   0.0795 <<  -1.1350 <<  0.9033 <<  -0.0636 <<  -0.5514 <<  0.9082;
}

[[maybe_unused]] void ColorsTest::testColorDifferenceLab() {
    const QFETCH(double, l1);
    const QFETCH(double, a1);
    const QFETCH(double, b1);
    const QFETCH(double, l2);
    const QFETCH(double, a2);
    const QFETCH(double, b2);
    const QFETCH(double, expectedDiff);

    const Colors::Lab lab1(l1, a1, b1);
    const Colors::Lab lab2(l2, a2, b2);
    const double diff = Colors::colorDifference(lab1, lab2);
    QCOMPARE(roundf(diff), expectedDiff);
}

[[maybe_unused]] void ColorsTest::testMatchBasicColor() {
    QCOMPARE(Colors::matchBasicColor(qRgb(0, 0, 0))->name, "black");
    QCOMPARE(Colors::matchBasicColor(qRgb(255, 0, 0))->name, "red");
    QCOMPARE(Colors::matchBasicColor(qRgb(200, 0, 0))->name, "red");
    QCOMPARE(Colors::matchBasicColor(qRgb(90, 90, 10))->name, "olive");
    QCOMPARE(Colors::matchBasicColor(qRgb(5, 90, 10))->name, "green");
    QCOMPARE(Colors::matchBasicColor(qRgb(195, 17, 210))->name, "fuchsia");
    QCOMPARE(Colors::matchBasicColor(qRgb(195, 200, 230))->name, "silver");
    QCOMPARE(Colors::matchBasicColor(qRgb(240, 215, 230))->name, "white");
}

[[maybe_unused]] void ColorsTest::testMatchExtendedColor() {
    QCOMPARE(Colors::matchExtendedColor(qRgb(0, 0, 0))->name, "black");
    QCOMPARE(Colors::matchExtendedColor(qRgb(255, 0, 0))->name, "red");
    QCOMPARE(Colors::matchExtendedColor(qRgb(135, 40, 230))->name, "blueviolet");
    QCOMPARE(Colors::matchExtendedColor(qRgb(253, 155, 5))->name, "orange");
    QCOMPARE(Colors::matchExtendedColor(qRgb(253, 240, 5))->name, "yellow");
    QCOMPARE(Colors::matchExtendedColor(qRgb(221, 224, 215))->name, "gainsboro");
    QCOMPARE(Colors::matchExtendedColor(qRgb(32, 178, 170))->name, "lightseagreen");
    QCOMPARE(Colors::matchExtendedColor(qRgb(70, 210, 200))->name, "mediumturquoise");
}

[[maybe_unused]] void ColorsTest::testColorItem() {
    Colors::set(Colors::LineFore, qRgb(10, 20, 30));
    QCOMPARE(Colors::get(Colors::LineFore), qRgb(10, 20, 30));
    QCOMPARE(Colors::getDefault(Colors::LineFore), qRgb(255, 0, 0));
    QCOMPARE(Colors::getRed(Colors::LineFore), 10);
    QCOMPARE(Colors::getGreen(Colors::LineFore), 20);
    QCOMPARE(Colors::getBlue(Colors::LineFore), 30);
}

[[maybe_unused]] void ColorsTest::testOpacity() {
    QCOMPARE(Colors::getAlpha(Colors::CrosshairOpacity), 0xE5);

    Colors::setAlpha(Colors::LineFore, 25);
    QCOMPARE(Colors::getAlpha(Colors::LineFore), 25);
}

[[maybe_unused]] void ColorsTest::testOpacityConversion() {
    QCOMPARE(Colors::opacityToFraction(qRgba(0, 0, 0, 0)), 0.0);
    QCOMPARE(Colors::opacityToFraction(qRgba(0, 0, 0, 255)), 1.0);
    QCOMPARE(Colors::opacityToFraction(qRgba(0, 0, 0, 128)), 0.501960784314);

    QCOMPARE(Colors::opacityToPercent(qRgba(0, 0, 0, 0)), 0);
    QCOMPARE(Colors::opacityToPercent(qRgba(0, 0, 0, 255)), 100);
    QCOMPARE(Colors::opacityToPercent(qRgba(0, 0, 0, 128)), 50);

    QCOMPARE(Colors::opacityFromPercent(0), qRgba(0, 0, 0, 0));
    QCOMPARE(Colors::opacityFromPercent(100), qRgba(0, 0, 0, 255));
    QCOMPARE(Colors::opacityFromPercent(50), qRgba(0, 0, 0, 128));
}

QTEST_MAIN(ColorsTest)

#include "ColorsTest.moc"
