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

#pragma once

#include <QWidget>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPalette>


/// Display a color swatch and the corresponding color value.
///
class ColorDisplay : public QWidget {

    Q_OBJECT

public:
    /// The magnifier displays the color of the pixel at the center of the window. This point typically corresponds
    /// to the hot spot of an active graphic element such as a crosshair. The color can be displayed in a number
    /// of different color spaces. This enumeration indicates which color space is in use.
    ///
    enum ColorFormatId {
        RGBFmt = 0,             ///< Red, green and blue as individual values between 0 and 255 inclusive.
        RGBHexFmt = 1,          ///< Red, green and blue as a composite hex number of the form \#RRGGBB.
        CMYFmt = 2,             ///< Cyan, magenta and yellow as individual values between 0 and 255 inclusive.
        CMYKFmt = 3,            ///< Cyan, magenta, yellow and black as individual values.
        HSLFmt = 4,             ///< Hue, saturation and lightness as individual values.
        YCbCrFmt = 5,           ///< Luminance and chrominance as individual values.
        YIQFmt = 6,             ///< Luminance, and inphase and quadrature chrominance channels.
        BasicNameFmt = 7,       ///< Match to basic web colors and display color name
        BasicHexFmt = 8,        ///< Match to basic web colors and display color in hex
        ExtendedNameFmt = 9,    ///< Match to extended web colors and display color name
        ExtendedHexFmt = 10,    ///< Match to extended web colors and display color in hex
    };


    ColorDisplay();

    void setColorFmt(ColorFormatId colorFmt);

public slots:
    void setColor(QRgb color);

private:
    static constexpr QChar k_fillChar { '0' };

    ColorFormatId m_colorFmt { RGBFmt };
    QLabel* m_colorSpaceLabel;
    QLineEdit* m_colorField;
    QFrame* m_colorSwatch;
    QPalette* m_colorSwatchPalette;
};
