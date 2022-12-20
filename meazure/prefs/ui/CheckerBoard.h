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
#include <QColor>


/// A checkerboard for use a background for widgets whose opacities will be adjusted.
///
class CheckerBoard : public QWidget {

    Q_OBJECT

public:
    enum Theme {
        Light,
        Dark
    };

    explicit CheckerBoard(Theme theme = Light, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static constexpr int k_side { 6 };                 ///< Size of a square, pixels
    static constexpr QColor k_light1 { QColorConstants::White };
    static constexpr QColor k_light2 { 210, 210, 210 };
    static constexpr QColor k_dark1 { QColorConstants::Black };
    static constexpr QColor k_dark2 { 55, 55, 55 };

    const QColor m_color1;
    const QColor m_color2;
};
