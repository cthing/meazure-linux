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

#include "CheckerBoard.h"
#include <QPainter>

CheckerBoard::CheckerBoard(Theme theme, QWidget *parent) :
        QWidget(parent),
        m_color1(theme == Light ? k_light1 : k_dark1),
        m_color2(theme == Light ? k_light2 : k_dark2) {
}

void CheckerBoard::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    const int w = width();
    const int h = height();

    for (auto [x, altCol] = std::pair(0, false); x < w; x += k_side, altCol = !altCol) {      // NOLINT(bugprone-infinite-loop)
        for (auto [y, altRow] = std::pair(0, false); y < h; y += k_side, altRow = !altRow) {  // NOLINT(bugprone-infinite-loop)
            painter.fillRect(x, y, k_side, k_side, (altCol != altRow) ? m_color1 : m_color2);
        }
    }
}
