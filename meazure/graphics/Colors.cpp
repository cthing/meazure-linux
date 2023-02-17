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

#include "Colors.h"
#include <meazure/utils/MathUtils.h>
#include <QApplication>
#include <QColorDialog>
#include <QColor>
#include <map>
#include <cmath>
#include <limits>
#include <QtMath>


using ColorMap = std::map<Colors::Item, QRgb>;        ///< Maps items to their colors.

static ColorMap const defaultColors = {
        { Colors::LineFore,           qRgb(0xFF, 0, 0) },
        { Colors::CrosshairBack,      qRgb(0xFF, 0, 0) },
        { Colors::CrosshairBorder,    qRgb(0x50, 0x50, 0x50) },
        { Colors::CrosshairHighlight, qRgb(0xFF, 0xFF, 0) },
        { Colors::CrosshairOpacity,   qRgba(0, 0, 0, 0xE5) },
        { Colors::RulerBack,          qRgb(0xFF, 0xFF, 0xFF) },
        { Colors::RulerBorder,        qRgb(0, 0, 0) },
        { Colors::RulerOpacity,       qRgba(0, 0, 0, 0xE5) }
};

static ColorMap colors = defaultColors;

using BasicColorTable = Colors::ColorTable<16>;

static BasicColorTable basicWebColors = { {
        { "black",      qRgb(  0,   0,   0), Colors::RGBtoLab(qRgb(  0,   0,   0)) },
        { "silver",     qRgb(192, 192, 192), Colors::RGBtoLab(qRgb(192, 192, 192)) },
        { "gray",       qRgb(128, 128, 128), Colors::RGBtoLab(qRgb(128, 128, 128)) },
        { "white",      qRgb(255, 255, 255), Colors::RGBtoLab(qRgb(255, 255, 255)) },
        { "maroon",     qRgb(128,   0,   0), Colors::RGBtoLab(qRgb(128,   0,   0)) },
        { "red",        qRgb(255,   0,   0), Colors::RGBtoLab(qRgb(255,   0,   0)) },
        { "purple",     qRgb(128,   0, 128), Colors::RGBtoLab(qRgb(128,   0, 128)) },
        { "fuchsia",    qRgb(255,   0, 255), Colors::RGBtoLab(qRgb(255,   0, 255)) },
        { "green",      qRgb(  0, 128,   0), Colors::RGBtoLab(qRgb(  0, 128,   0)) },
        { "lime",       qRgb(  0, 255,   0), Colors::RGBtoLab(qRgb(  0, 255,   0)) },
        { "olive",      qRgb(128, 128,   0), Colors::RGBtoLab(qRgb(128, 128,   0)) },
        { "yellow",     qRgb(255, 255,   0), Colors::RGBtoLab(qRgb(255, 255,   0)) },
        { "navy",       qRgb(  0,   0, 128), Colors::RGBtoLab(qRgb(  0,   0, 128)) },
        { "blue",       qRgb(  0,   0, 255), Colors::RGBtoLab(qRgb(  0,   0, 255)) },
        { "teal",       qRgb(  0, 128, 128), Colors::RGBtoLab(qRgb(  0, 128, 128)) },
        { "aqua",       qRgb(  0, 255, 255), Colors::RGBtoLab(qRgb(  0, 255, 255)) },
} };

using ExtendedColorTable = Colors::ColorTable<141>;

static ExtendedColorTable extendedWebColors = { {
        { "aliceblue",              qRgb(240, 248, 255), Colors::RGBtoLab(qRgb(240, 248, 255)) },
        { "antiquewhite",           qRgb(250, 235, 215), Colors::RGBtoLab(qRgb(250, 235, 215)) },
        { "aqua",                   qRgb(  0, 255, 255), Colors::RGBtoLab(qRgb(  0, 255, 255)) },
        { "aquamarine",             qRgb(127, 255, 212), Colors::RGBtoLab(qRgb(127, 255, 212)) },
        { "azure",                  qRgb(240, 255, 255), Colors::RGBtoLab(qRgb(240, 255, 255)) },
        { "beige",                  qRgb(245, 245, 220), Colors::RGBtoLab(qRgb(245, 245, 220)) },
        { "bisque",                 qRgb(255, 228, 196), Colors::RGBtoLab(qRgb(255, 228, 196)) },
        { "black",                  qRgb(  0,   0,   0), Colors::RGBtoLab(qRgb(  0,   0,   0)) },
        { "blanchedalmond",         qRgb(255, 235, 205), Colors::RGBtoLab(qRgb(255, 235, 205)) },
        { "blue",                   qRgb(  0,   0, 255), Colors::RGBtoLab(qRgb(  0,   0, 255)) },
        { "blue",                   qRgb(  0,   0, 255), Colors::RGBtoLab(qRgb(  0,   0, 255)) },
        { "blueviolet",             qRgb(138,  43, 226), Colors::RGBtoLab(qRgb(138,  43, 226)) },
        { "brown",                  qRgb(165,  42,  42), Colors::RGBtoLab(qRgb(165,  42,  42)) },
        { "burlywood",              qRgb(222, 184, 135), Colors::RGBtoLab(qRgb(222, 184, 135)) },
        { "cadetblue",              qRgb( 95, 158, 160), Colors::RGBtoLab(qRgb( 95, 158, 160)) },
        { "chartreuse",             qRgb(127, 255,   0), Colors::RGBtoLab(qRgb(127, 255,   0)) },
        { "chocolate",              qRgb(210, 105,  30), Colors::RGBtoLab(qRgb(210, 105,  30)) },
        { "coral",                  qRgb(255, 127,  80), Colors::RGBtoLab(qRgb(255, 127,  80)) },
        { "cornflowerblue",         qRgb(100, 149, 237), Colors::RGBtoLab(qRgb(100, 149, 237)) },
        { "cornsilk",               qRgb(255, 248, 220), Colors::RGBtoLab(qRgb(255, 248, 220)) },
        { "crimson",                qRgb(220,  20,  60), Colors::RGBtoLab(qRgb(220,  20,  60)) },
        { "cyan",                   qRgb(  0, 255, 255), Colors::RGBtoLab(qRgb(  0, 255, 255)) },
        { "darkblue",               qRgb(  0,   0, 139), Colors::RGBtoLab(qRgb(  0,   0, 139)) },
        { "darkcyan",               qRgb(  0, 139, 139), Colors::RGBtoLab(qRgb(  0, 139, 139)) },
        { "darkgoldenrod",          qRgb(184, 134,  11), Colors::RGBtoLab(qRgb(184, 134,  11)) },
        { "darkgray",               qRgb(169, 169, 169), Colors::RGBtoLab(qRgb(169, 169, 169)) },
        { "darkgreen",              qRgb(  0, 100,   0), Colors::RGBtoLab(qRgb(  0, 100,   0)) },
        { "darkkhaki",              qRgb(189, 183, 107), Colors::RGBtoLab(qRgb(189, 183, 107)) },
        { "darkmagenta",            qRgb(139,   0, 139), Colors::RGBtoLab(qRgb(139,   0, 139)) },
        { "darkolivegreen",         qRgb( 85, 107,  47), Colors::RGBtoLab(qRgb( 85, 107,  47)) },
        { "darkorange",             qRgb(255, 140,   0), Colors::RGBtoLab(qRgb(255, 140,   0)) },
        { "darkorchid",             qRgb(153,  50, 204), Colors::RGBtoLab(qRgb(153,  50, 204)) },
        { "darkred",                qRgb(139,   0,   0), Colors::RGBtoLab(qRgb(139,   0,   0)) },
        { "darksalmon",             qRgb(233, 150, 122), Colors::RGBtoLab(qRgb(233, 150, 122)) },
        { "darkseagreen",           qRgb(143, 188, 143), Colors::RGBtoLab(qRgb(143, 188, 143)) },
        { "darkslateblue",          qRgb( 72,  61, 139), Colors::RGBtoLab(qRgb( 72,  61, 139)) },
        { "darkslategray",          qRgb( 47,  79,  79), Colors::RGBtoLab(qRgb( 47,  79,  79)) },
        { "darkturquoise",          qRgb(  0, 206, 209), Colors::RGBtoLab(qRgb(  0, 206, 209)) },
        { "darkviolet",             qRgb(148,   0, 211), Colors::RGBtoLab(qRgb(148,   0, 211)) },
        { "deeppink",               qRgb(255,  20, 147), Colors::RGBtoLab(qRgb(255,  20, 147)) },
        { "deepskyblue",            qRgb(  0, 191, 255), Colors::RGBtoLab(qRgb(  0, 191, 255)) },
        { "dimgray",                qRgb(105, 105, 105), Colors::RGBtoLab(qRgb(105, 105, 105)) },
        { "dodgerblue",             qRgb( 30, 144, 255), Colors::RGBtoLab(qRgb( 30, 144, 255)) },
        { "firebrick",              qRgb(178,  34,  34), Colors::RGBtoLab(qRgb(178,  34,  34)) },
        { "floralwhite",            qRgb(255, 250, 240), Colors::RGBtoLab(qRgb(255, 250, 240)) },
        { "forestgreen",            qRgb( 34, 139,  34), Colors::RGBtoLab(qRgb( 34, 139,  34)) },
        { "fuchsia",                qRgb(255,   0, 255), Colors::RGBtoLab(qRgb(255,   0, 255)) },
        { "gainsboro",              qRgb(220, 220, 220), Colors::RGBtoLab(qRgb(220, 220, 220)) },
        { "ghostwhite",             qRgb(248, 248, 255), Colors::RGBtoLab(qRgb(248, 248, 255)) },
        { "gold",                   qRgb(255, 215,   0), Colors::RGBtoLab(qRgb(255, 215,   0)) },
        { "goldenrod",              qRgb(218, 165,  32), Colors::RGBtoLab(qRgb(218, 165,  32)) },
        { "gray",                   qRgb(128, 128, 128), Colors::RGBtoLab(qRgb(128, 128, 128)) },
        { "green",                  qRgb(  0, 128,   0), Colors::RGBtoLab(qRgb(  0, 128,   0)) },
        { "greenyellow",            qRgb(173, 255,  47), Colors::RGBtoLab(qRgb(173, 255,  47)) },
        { "honeydew",               qRgb(240, 255, 240), Colors::RGBtoLab(qRgb(240, 255, 240)) },
        { "hotpink",                qRgb(255, 105, 180), Colors::RGBtoLab(qRgb(255, 105, 180)) },
        { "indianred",              qRgb(205,  92,  92), Colors::RGBtoLab(qRgb(205,  92,  92)) },
        { "indigo",                 qRgb( 75,   0, 130), Colors::RGBtoLab(qRgb( 75,   0, 130)) },
        { "ivory",                  qRgb(255, 255, 240), Colors::RGBtoLab(qRgb(255, 255, 240)) },
        { "khaki",                  qRgb(240, 230, 140), Colors::RGBtoLab(qRgb(240, 230, 140)) },
        { "lavender",               qRgb(230, 230, 250), Colors::RGBtoLab(qRgb(230, 230, 250)) },
        { "lavenderblush",          qRgb(255, 240, 245), Colors::RGBtoLab(qRgb(255, 240, 245)) },
        { "lawngreen",              qRgb(124, 252,   0), Colors::RGBtoLab(qRgb(124, 252,   0)) },
        { "lemonchiffon",           qRgb(255, 250, 205), Colors::RGBtoLab(qRgb(255, 250, 205)) },
        { "lightblue",              qRgb(173, 216, 230), Colors::RGBtoLab(qRgb(173, 216, 230)) },
        { "lightcoral",             qRgb(240, 128, 128), Colors::RGBtoLab(qRgb(240, 128, 128)) },
        { "lightcyan",              qRgb(224, 255, 255), Colors::RGBtoLab(qRgb(224, 255, 255)) },
        { "lightgoldenrodyellow",   qRgb(250, 250, 210), Colors::RGBtoLab(qRgb(250, 250, 210)) },
        { "lightgray",              qRgb(211, 211, 211), Colors::RGBtoLab(qRgb(211, 211, 211)) },
        { "lightgreen",             qRgb(144, 238, 144), Colors::RGBtoLab(qRgb(144, 238, 144)) },
        { "lightpink",              qRgb(255, 182, 193), Colors::RGBtoLab(qRgb(255, 182, 193)) },
        { "lightsalmon",            qRgb(255, 160, 122), Colors::RGBtoLab(qRgb(255, 160, 122)) },
        { "lightseagreen",          qRgb( 32, 178, 170), Colors::RGBtoLab(qRgb( 32, 178, 170)) },
        { "lightskyblue",           qRgb(135, 206, 250), Colors::RGBtoLab(qRgb(135, 206, 250)) },
        { "lightslategray",         qRgb(119, 136, 153), Colors::RGBtoLab(qRgb(119, 136, 153)) },
        { "lightsteelblue",         qRgb(176, 196, 222), Colors::RGBtoLab(qRgb(176, 196, 222)) },
        { "lightyellow",            qRgb(255, 255, 224), Colors::RGBtoLab(qRgb(255, 255, 224)) },
        { "lime",                   qRgb(  0, 255,   0), Colors::RGBtoLab(qRgb(  0, 255,   0)) },
        { "limegreen",              qRgb( 50, 205,  50), Colors::RGBtoLab(qRgb( 50, 205,  50)) },
        { "linen",                  qRgb(250, 240, 230), Colors::RGBtoLab(qRgb(250, 240, 230)) },
        { "magenta",                qRgb(255,   0, 255), Colors::RGBtoLab(qRgb(255,   0, 255)) },
        { "maroon",                 qRgb(128,   0,   0), Colors::RGBtoLab(qRgb(128,   0,   0)) },
        { "mediumaquamarine",       qRgb(102, 205, 170), Colors::RGBtoLab(qRgb(102, 205, 170)) },
        { "mediumblue",             qRgb(  0,   0, 205), Colors::RGBtoLab(qRgb(  0,   0, 205)) },
        { "mediumorchid",           qRgb(186,  85, 211), Colors::RGBtoLab(qRgb(186,  85, 211)) },
        { "mediumpurple",           qRgb(147, 112, 219), Colors::RGBtoLab(qRgb(147, 112, 219)) },
        { "mediumseagreen",         qRgb( 60, 179, 113), Colors::RGBtoLab(qRgb( 60, 179, 113)) },
        { "mediumslateblue",        qRgb(123, 104, 238), Colors::RGBtoLab(qRgb(123, 104, 238)) },
        { "mediumspringgreen",      qRgb(  0, 250, 154), Colors::RGBtoLab(qRgb(  0, 250, 154)) },
        { "mediumturquoise",        qRgb( 72, 209, 204), Colors::RGBtoLab(qRgb( 72, 209, 204)) },
        { "mediumvioletred",        qRgb(199,  21, 133), Colors::RGBtoLab(qRgb(199,  21, 133)) },
        { "midnightblue",           qRgb( 25,  25, 112), Colors::RGBtoLab(qRgb( 25,  25, 112)) },
        { "mintcream",              qRgb(245, 255, 250), Colors::RGBtoLab(qRgb(245, 255, 250)) },
        { "mistyrose",              qRgb(255, 228, 225), Colors::RGBtoLab(qRgb(255, 228, 225)) },
        { "moccasin",               qRgb(255, 228, 181), Colors::RGBtoLab(qRgb(255, 228, 181)) },
        { "navajowhite",            qRgb(255, 222, 173), Colors::RGBtoLab(qRgb(255, 222, 173)) },
        { "navy",                   qRgb(  0,   0, 128), Colors::RGBtoLab(qRgb(  0,   0, 128)) },
        { "oldlace",                qRgb(253, 245, 230), Colors::RGBtoLab(qRgb(253, 245, 230)) },
        { "olive",                  qRgb(128, 128,   0), Colors::RGBtoLab(qRgb(128, 128,   0)) },
        { "olivedrab",              qRgb(107, 142,  35), Colors::RGBtoLab(qRgb(107, 142,  35)) },
        { "orange",                 qRgb(255, 165,   0), Colors::RGBtoLab(qRgb(255, 165,   0)) },
        { "orangered",              qRgb(255,  69,   0), Colors::RGBtoLab(qRgb(255,  69,   0)) },
        { "orchid",                 qRgb(218, 112, 214), Colors::RGBtoLab(qRgb(218, 112, 214)) },
        { "palegoldenrod",          qRgb(238, 232, 170), Colors::RGBtoLab(qRgb(238, 232, 170)) },
        { "palegreen",              qRgb(152, 251, 152), Colors::RGBtoLab(qRgb(152, 251, 152)) },
        { "paleturquoise",          qRgb(175, 238, 238), Colors::RGBtoLab(qRgb(175, 238, 238)) },
        { "palevioletred",          qRgb(219, 112, 147), Colors::RGBtoLab(qRgb(219, 112, 147)) },
        { "papayawhip",             qRgb(255, 239, 213), Colors::RGBtoLab(qRgb(255, 239, 213)) },
        { "peachpuff",              qRgb(255, 218, 185), Colors::RGBtoLab(qRgb(255, 218, 185)) },
        { "peru",                   qRgb(205, 133,  63), Colors::RGBtoLab(qRgb(205, 133,  63)) },
        { "pink",                   qRgb(255, 192, 203), Colors::RGBtoLab(qRgb(255, 192, 203)) },
        { "plum",                   qRgb(221, 160, 221), Colors::RGBtoLab(qRgb(221, 160, 221)) },
        { "powderblue",             qRgb(176, 224, 230), Colors::RGBtoLab(qRgb(176, 224, 230)) },
        { "purple",                 qRgb(128,   0, 128), Colors::RGBtoLab(qRgb(128,   0, 128)) },
        { "red",                    qRgb(255,   0,   0), Colors::RGBtoLab(qRgb(255,   0,   0)) },
        { "rosybrown",              qRgb(188, 143, 143), Colors::RGBtoLab(qRgb(188, 143, 143)) },
        { "royalblue",              qRgb( 65, 105, 225), Colors::RGBtoLab(qRgb( 65, 105, 225)) },
        { "saddlebrown",            qRgb(139,  69,  19), Colors::RGBtoLab(qRgb(139,  69,  19)) },
        { "salmon",                 qRgb(250, 128, 114), Colors::RGBtoLab(qRgb(250, 128, 114)) },
        { "sandybrown",             qRgb(244, 164,  96), Colors::RGBtoLab(qRgb(244, 164,  96)) },
        { "seagreen",               qRgb( 46, 139,  87), Colors::RGBtoLab(qRgb( 46, 139,  87)) },
        { "seashell",               qRgb(255, 245, 238), Colors::RGBtoLab(qRgb(255, 245, 238)) },
        { "sienna",                 qRgb(160, 82,   45), Colors::RGBtoLab(qRgb(160, 82,   45)) },
        { "silver",                 qRgb(192, 192, 192), Colors::RGBtoLab(qRgb(192, 192, 192)) },
        { "skyblue",                qRgb(135, 206, 235), Colors::RGBtoLab(qRgb(135, 206, 235)) },
        { "slateblue",              qRgb(106,  90, 205), Colors::RGBtoLab(qRgb(106,  90, 205)) },
        { "slategray",              qRgb(112, 128, 144), Colors::RGBtoLab(qRgb(112, 128, 144)) },
        { "snow",                   qRgb(255, 250, 250), Colors::RGBtoLab(qRgb(255, 250, 250)) },
        { "springgreen",            qRgb(  0, 255, 127), Colors::RGBtoLab(qRgb(  0, 255, 127)) },
        { "steelblue",              qRgb( 70, 130, 180), Colors::RGBtoLab(qRgb( 70, 130, 180)) },
        { "tan",                    qRgb(210, 180, 140), Colors::RGBtoLab(qRgb(210, 180, 140)) },
        { "teal",                   qRgb(  0, 128, 128), Colors::RGBtoLab(qRgb(  0, 128, 128)) },
        { "thistle",                qRgb(216, 191, 216), Colors::RGBtoLab(qRgb(216, 191, 216)) },
        { "tomato",                 qRgb(255,  99,  71), Colors::RGBtoLab(qRgb(255,  99,  71)) },
        { "turquoise",              qRgb( 64, 224, 208), Colors::RGBtoLab(qRgb( 64, 224, 208)) },
        { "violet",                 qRgb(238, 130, 238), Colors::RGBtoLab(qRgb(238, 130, 238)) },
        { "wheat",                  qRgb(245, 222, 179), Colors::RGBtoLab(qRgb(245, 222, 179)) },
        { "white",                  qRgb(255, 255, 255), Colors::RGBtoLab(qRgb(255, 255, 255)) },
        { "whitesmoke",             qRgb(245, 245, 245), Colors::RGBtoLab(qRgb(245, 245, 245)) },
        { "yellow",                 qRgb(255, 255,   0), Colors::RGBtoLab(qRgb(255, 255,   0)) },
        { "yellowgreen",            qRgb(154, 205,  50), Colors::RGBtoLab(qRgb(154, 205,  50)) },
} };


Colors::ChangeNotifier* Colors::getChangeNotifier() {
    static Colors::ChangeNotifier colorChangeNotifier;

    return &colorChangeNotifier;
}

void Colors::hardReset() {
    for (auto const& [item, color] : defaultColors) {
        set(item, color);
    }
}

void Colors::writeConfig(Config& config) {
    config.writeUInt("LineFore", colors[LineFore]);
    config.writeUInt("CrossHairBack", colors[CrosshairBack]);
    config.writeUInt("CrossHairBorder", colors[CrosshairBorder]);
    config.writeUInt("CrossHairHilite", colors[CrosshairHighlight]);
    config.writeUInt("CrossHairOpacity", colors[CrosshairOpacity]);
    config.writeUInt("RulerBack", colors[RulerBack]);
    config.writeUInt("RulerBorder", colors[RulerBorder]);
    config.writeUInt("RulerOpacity", colors[RulerOpacity]);

    const int numCustomColors = QColorDialog::customCount();
    config.writeInt("CustomColors", numCustomColors);
    for (int idx = 0; idx < numCustomColors; idx++) {
        config.writeUInt(QString("CustomColor%1").arg(idx), QColorDialog::customColor(idx).rgb());
    }
}

void Colors::readConfig(const Config& config) {
    const int availableNumCustomColors = QColorDialog::customCount();
    const int savedNumCustomColors = config.readInt("CustomColors", availableNumCustomColors);
    const int numCustomColors = std::min(availableNumCustomColors, savedNumCustomColors);
    for (int idx = 0; idx < numCustomColors; idx++) {
        const QRgb color = config.readUInt(QString("CustomColor%1").arg(idx), QColorDialog::customColor(idx).rgb());
        QColorDialog::setCustomColor(idx, QColor(color));
    }

    set(LineFore, config.readUInt("LineFore", defaultColors.at(LineFore)));
    set(CrosshairBack, config.readUInt("CrossHairBack", defaultColors.at(CrosshairBack)));
    set(CrosshairBorder, config.readUInt("CrossHairBorder", defaultColors.at(CrosshairBorder)));
    set(CrosshairHighlight, config.readUInt("CrossHairHilite", defaultColors.at(CrosshairHighlight)));
    set(CrosshairOpacity, config.readUInt("CrossHairOpacity", defaultColors.at(CrosshairOpacity)));
    set(RulerBack, config.readUInt("RulerBack", defaultColors.at(RulerBack)));
    set(RulerBorder, config.readUInt("RulerBorder", defaultColors.at(RulerBorder)));
    set(RulerOpacity, config.readUInt("RulerOpacity", defaultColors.at(RulerOpacity)));
}

void Colors::set(Item item, QRgb color) {
    colors[item] = color;
    emit getChangeNotifier()->colorChanged(item, color);
}

void Colors::setAlpha(Item item, int opacity) {
    colors[item] = qRgba(0, 0, 0, opacity);
}

QRgb Colors::get(Item item) {
    return colors.at(item);
}

int Colors::getRed(Item item) {
    return qRed(colors.at(item));
}

int Colors::getGreen(Item item) {
    return qGreen(colors.at(item));
}

int Colors::getBlue(Item item) {
    return qBlue(colors.at(item));
}

int Colors::getAlpha(Item item) {
    return qAlpha(colors.at(item));
}

QRgb Colors::getDefault(Item item) {
    return defaultColors.at(item);
}

bool Colors::isDarkMode() {
    static bool first = true;
    static bool isDark;

    if (first) {
        first = false;
        const int yWindow = Colors::RGBtoY(QApplication::palette().color(QPalette::Window).rgb());
        const int yText = Colors::RGBtoY(QApplication::palette().color(QPalette::Text).rgb());
        isDark = (yText > yWindow);
    }
    return isDark;
}

QRgb Colors::interpolateColor(QRgb startRGB, QRgb endRGB, int percent) {
    if (percent == 0) {
        return startRGB;
    }
    if (percent == 100) {
        return endRGB;
    }

    auto interpolate = [](int start, int end, int percent) {
        return start + qRound((end - start) * percent / 100.0);
    };

    const HSL startHSL = RGBtoHSL(startRGB);
    const HSL endHSL = RGBtoHSL(endRGB);

    const HSL hsl(interpolate(startHSL.hue, endHSL.hue, percent),
                  interpolate(startHSL.saturation, endHSL.saturation, percent),
                  interpolate(startHSL.lightness, endHSL.lightness, percent));

    return HSLtoRGB(hsl);
}

double Colors::colorDifference(const Colors::Lab& color1, const Colors::Lab& color2) {
    // This implementation of the CIEDE2000 algorithm follows the implementation notes in
    // https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/ciede2000noteCRNA.pdf. Excerpts from this
    // paper and references to the equation numbers are included in the comments in this implementation.

    // Constants
    constexpr double k_pow25To7 = 6103515625.0;     // 25^7
    constexpr double k_deg360Rad = qDegreesToRadians(360.0);
    constexpr double k_deg180Rad = qDegreesToRadians(180.0);
    constexpr double k_deg30Rad = qDegreesToRadians(30.0);
    constexpr double k_deg63Rad = qDegreesToRadians(63.0);
    constexpr double k_deg6Rad = qDegreesToRadians(6.0);
    constexpr double k_deg25Rad = qDegreesToRadians(25.0);
    constexpr double k_deg275Rad = qDegreesToRadians(275.0);

    //
    // Step 1
    //

    // Equation 2
    const double b1Squared = color1.b * color1.b;
    const double b2Squared = color2.b * color2.b;
    const double c1 = std::sqrt((color1.a * color1.a) + b1Squared);
    const double c2 = std::sqrt((color2.a * color2.a) + b2Squared);

    // Equation 3
    const double cAve = (c1 + c2) / 2.0;

    // Equation 4
    const double cAvePow7 = std::pow(cAve, 7.0);
    const double g = 0.5 * (1.0 - std::sqrt(cAvePow7 / (cAvePow7 + k_pow25To7)));

    // Equation 5
    const double aPrime1 = (1.0 + g) * color1.a;
    const double aPrime2 = (1.0 + g) * color2.a;

    // Equation 6
    const double cPrime1 = std::sqrt((aPrime1 * aPrime1) + b1Squared);
    const double cPrime2 = std::sqrt((aPrime2 * aPrime2) + b2Squared);

    // Equation 7
    auto hueAngle = [](const Lab& color, double aPrime) {
        if (MathUtils::fuzzyEqualZero(color.b) && MathUtils::fuzzyEqualZero(aPrime)) {
            return 0.0;
        }

        double hPrime = std::atan2(color.b, aPrime);

        // Per implementation clarification 1, add 360 degrees to negative hue angles
        if (hPrime < 0.0) {
            hPrime += k_deg360Rad;
        }

        return hPrime;
    };

    const double hPrime1 = hueAngle(color1, aPrime1);
    const double hPrime2 = hueAngle(color2, aPrime2);

    //
    // Step 2
    //

    // Equation 8
    const double deltaLPrime = color2.l - color1.l;

    // Equation 9
    const double deltaCPrime = cPrime2 - cPrime1;

    // Equation 10
    const double cPrimeProduct = cPrime1 * cPrime2;
    double deltahPrime;       // NOLINT(cppcoreguidelines-init-variables)

    if (MathUtils::fuzzyEqualZero(cPrimeProduct)) {
        deltahPrime = 0.0;
    } else {
        deltahPrime = hPrime2 - hPrime1;

        if (deltahPrime > k_deg180Rad) {
            deltahPrime -= k_deg360Rad;
        } else if (deltahPrime < -k_deg180Rad) {
            deltahPrime += k_deg360Rad;
        }
    }

    // Equation 11
    const double deltaHPrime = 2.0 * std::sqrt(cPrimeProduct) * std::sin(deltahPrime / 2.0);

    //
    // Step 3
    //

    // Equation 12
    const double lPrimeAve = (color1.l + color2.l) / 2.0;

    // Equation 13
    const double cPrimeAve = (cPrime1 + cPrime2) / 2.0;

    // Equation 14
    const double hPrimeSum = hPrime1 + hPrime2;
    double hPrimeAve;       // NOLINT(cppcoreguidelines-init-variables)

    if (MathUtils::fuzzyEqualZero(cPrimeProduct)) {
        hPrimeAve = hPrimeSum;
    } else {
        if (std::fabs(hPrime1 - hPrime2) <= k_deg180Rad) {
            hPrimeAve = hPrimeSum / 2.0;
        } else {
            if (hPrimeSum < k_deg360Rad) {
                hPrimeAve = (hPrimeSum + k_deg360Rad) / 2.0;
            } else {
                hPrimeAve = (hPrimeSum - k_deg360Rad) / 2.0;
            }
        }
    }

    // Equation 15
    const double t = 1.0
            - 0.17 * std::cos(hPrimeAve - k_deg30Rad)
            + 0.24 * std::cos(2.0 * hPrimeAve)
            + 0.32 * cos(3.0 * hPrimeAve + k_deg6Rad)
            - 0.20 * std::cos(4.0 * hPrimeAve - k_deg63Rad);

    // Equation 16
    const double deltaTheta = k_deg30Rad * std::exp(-std::pow((hPrimeAve - k_deg275Rad) / k_deg25Rad, 2.0));

    // Equation 17
    const double cPrimeAveTo7 = std::pow(cPrimeAve, 7.0);
    const double rc = 2.0 * std::sqrt(cPrimeAveTo7 / (cPrimeAveTo7 + k_pow25To7));

    // Equation 18
    const double lPrimeAveMinus50Squared = std::pow(lPrimeAve - 50.0, 2.0);
    const double sl = 1.0 + 0.015 * lPrimeAveMinus50Squared / std::sqrt(20.0 + lPrimeAveMinus50Squared);

    // Equation 19
    const double sc = 1.0 + 0.045 * cPrimeAve;

    // Equation 20
    const double sh = 1.0 + 0.015 * cPrimeAve * t;

    // Equation 21
    const double rt = -std::sin(2.0 * deltaTheta) * rc;

    // Equation 22
    // Use the reference conditions for the parametric weighting factors (i.e. kL=kC=kH=1.0).
    return std::sqrt(std::pow(deltaLPrime / sl, 2.0)
                     + std::pow(deltaCPrime / sc, 2.0)
                     + std::pow(deltaHPrime / sh, 2.0)
                     + rt * (deltaCPrime / sc) * (deltaHPrime / sh));
}

const Colors::ColorTableEntry* Colors::matchBasicColor(QRgb rgb) {
    // Typically colors are the same over adjacent pixels so remember the last match.
    static const ColorTableEntry* lastEntry = basicWebColors.data();
    static QRgb lastColor = lastEntry->rgb;

    if (lastColor != rgb) {
        lastEntry = matchColor(basicWebColors, rgb);
        lastColor = rgb;
    }

    return lastEntry;
}

const Colors::ColorTableEntry* Colors::matchExtendedColor(QRgb rgb) {
    // Typically colors are the same over adjacent pixels so remember the last match.
    static const ColorTableEntry* lastEntry = extendedWebColors.data();
    static QRgb lastColor = lastEntry->rgb;

    if (lastColor != rgb) {
        lastEntry = matchColor(extendedWebColors, rgb);
        lastColor = rgb;
    }

    return lastEntry;
}

template<std::size_t SIZE>
const Colors::ColorTableEntry* Colors::matchColor(const ColorTable<SIZE>& table, QRgb rgb) {
    const Lab lab = RGBtoLab(rgb);

    double minDiff = std::numeric_limits<double>::max();
    const ColorTableEntry* bestEntry = nullptr;

    for (const ColorTableEntry& entry : table) {
        if (entry.rgb == rgb) {
            return &entry;
        }

        const double diff = colorDifference(entry.lab, lab);
        if (diff < minDiff) {
            minDiff = diff;
            bestEntry = &entry;
        }
    }

    return bestEntry;
}

Colors::CMYK Colors::RGBtoCMYK(QRgb rgb) {
    const CMY cmy = RGBtoCMY(rgb);
    const int black = std::min({ cmy.cyan, cmy.magenta, cmy.yellow });

    if (black == 255) {
        return { 0, 0, 0, black };
    }

    const double denom = 255.0 - black;
    const int cyan = qRound(255.0 * (cmy.cyan - black) / denom);
    const int magenta = qRound(255.0 * (cmy.magenta - black) / denom);
    const int yellow = qRound(255.0 * (cmy.yellow - black) / denom);

    return { cyan, magenta, yellow, black };
}

Colors::HSL Colors::RGBtoHSL(QRgb rgb) {
    double h;       // NOLINT(cppcoreguidelines-init-variables)
    double s;       // NOLINT(cppcoreguidelines-init-variables)
    double l;       // NOLINT(cppcoreguidelines-init-variables)
    const double r = qRed(rgb) / 255.0;
    const double g = qGreen(rgb) / 255.0;
    const double b = qBlue(rgb) / 255.0;
    const double cmax = std::max({ r, g, b });
    const double cmin = std::min({ r, g, b });
    const double delta = cmax - cmin;

    l = (cmax + cmin) / 2.0;
    if (MathUtils::fuzzyEqualZero(delta)) {      // Gray
        h = 0.0;
        s = 0.0;
    } else {                                // Chroma
        if (l < 0.5) {
            s = delta / (cmax + cmin);
        } else {
            s = delta / (2.0 - cmax - cmin);
        }

        if (MathUtils::fuzzyEqual(r, cmax)) {
            h = (g - b) / delta;
        } else if (MathUtils::fuzzyEqual(g, cmax)) {
            h = 2.0 + (b - r) / delta;
        } else {
            h = 4.0 + (r - g) / delta;
        }
        h /= 6.0;

        if (h < 0.0) {
            h += 1.0;
        } else if (h > 1.0) {
            h -= 1.0;
        }
    }

    return { qRound(h * 360.0), qRound(s * 100.0), qRound(l * 100.0) };
}

/// Converts a hue to an RGB component value based on the specified weighting factors. See conversion of HSL to RGB at
/// http://www.easyrgb.com/en/math.php.
///
/// @param[in] m1 Weighting factor between lightness and saturation.
/// @param[in] m2 Weighting factor between lightness and saturation.
/// @param[in] h Hue value.
/// @return Either R, G, or B between 0.0 and 1.0. The component returned depends on the weighting factors specified.
///
static double huetoRGB(double m1, double m2, double h) {
    if (h < 0.0) {
        h += 1.0;
    }
    if (h > 1.0) {
        h -= 1.0;
    }
    if ((6.0 * h) < 1.0) {
        return (m1 + (m2 - m1) * h * 6.0);
    }
    if ((2.0 * h) < 1.0) {
        return m2;
    }
    if ((3.0 * h) < 2.0) {
        return (m1 + (m2 - m1) * ((2.0 / 3.0) - h) * 6.0);
    }
    return m1;
}

QRgb Colors::HSLtoRGB(const HSL& hsl) {
    double r;       // NOLINT(cppcoreguidelines-init-variables)
    double g;       // NOLINT(cppcoreguidelines-init-variables)
    double b;       // NOLINT(cppcoreguidelines-init-variables)
    const double l = hsl.lightness / 100.0;

    if (hsl.saturation == 0) {
        r = g = b = l;
    } else {
        const double h = hsl.hue / 360.0;
        const double s = hsl.saturation / 100.0;

        const double m2 = (l <= 0.5) ? l * (1.0 + s) : l + s - l * s;
        const double m1 = 2.0 * l - m2;

        r = huetoRGB(m1, m2, h + 1.0 / 3.0);
        g = huetoRGB(m1, m2, h);
        b = huetoRGB(m1, m2, h - 1.0 / 3.0);
    }

    return qRgb(qRound(r * 255.0), qRound(g * 255.0), qRound(b * 255.0));
}

Colors::YCbCr Colors::RGBtoYCbCr(QRgb rgb) {
    const double r = qRed(rgb);
    const double g = qGreen(rgb);
    const double b = qBlue(rgb);
    const int y = qRound(0.257 * r + 0.504 * g + 0.098 * b + 16.0);
    const int cb = qRound(-0.148 * r - 0.291 * g + 0.439 * b + 128.0);
    const int cr = qRound(0.439 * r - 0.368 * g - 0.071 * b + 128.0);
    return { y, cb, cr };
}

Colors::YIQ Colors::RGBtoYIQ(QRgb rgb) {
    const double r = qRed(rgb);
    const double g = qGreen(rgb);
    const double b = qBlue(rgb);
    const int y = qRound(0.299 * r + 0.587 * g + 0.114 * b);
    const int i = qRound(0.596 * r - 0.275 * g - 0.321 * b);
    const int q = qRound(0.212 * r - 0.523 * g + 0.311 * b);
    return { y, i, q };
}

int Colors::RGBtoY(QRgb rgb) {
    return qRound(0.299 * qRed(rgb) + 0.587 * qGreen(rgb) + 0.114 * qBlue(rgb));
}


Colors::XYZ Colors::RGBtoXYZ(QRgb rgb) {
    double r = qRed(rgb) / 255.0;
    double g = qGreen(rgb) / 255.0;
    double b = qBlue(rgb) / 255.0;

    auto inverseCompanding = [](double value) {
        return (value > 0.04045) ? std::pow((value + 0.055) / 1.055, 2.4) : value / 12.92;
    };

    r = 100.0 * inverseCompanding(r);
    g = 100.0 * inverseCompanding(g);
    b = 100.0 * inverseCompanding(b);

    const double x = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
    const double y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
    const double z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
    return { x, y, z };
}

Colors::Lab Colors::XYZtoLab(const Colors::XYZ& xyz) {
    // 1931 D65 2 degree illuminant reference.
    //
    constexpr double k_xref = 95.047;
    constexpr double k_yref = 100.000;
    constexpr double k_zref = 108.883;

    constexpr double k_k = 903.3 / 116.0;
    constexpr double k_m = 16.0 / 116.0;

    double x = xyz.x / k_xref;
    double y = xyz.y / k_yref;
    double z = xyz.z / k_zref;

    auto func = [](double value) {
        return (value > 0.008856) ? std::cbrt(value) : k_k * value + k_m;
    };

    x = func(x);
    y = func(y);
    z = func(z);

    const double l = (116.0 * y) - 16.0;
    const double a = 500.0 * (x - y);
    const double b = 200.0 * (y - z);
    return { l, a, b };
}

Colors::Lab Colors::RGBtoLab(QRgb rgb) {
    return XYZtoLab(RGBtoXYZ(rgb));
}
