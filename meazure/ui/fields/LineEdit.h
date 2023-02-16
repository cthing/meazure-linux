/*
 * Copyright 2023 C Thing Software
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

#include <QLineEdit>


/// A QLineEdit whose width can be specified in terms of character. When proportional fonts are used, the
/// width is based off the 'm' character.
///
class LineEdit : public QLineEdit {

    Q_OBJECT

public:
    explicit LineEdit(int charWidth, QWidget *parent = nullptr);

    [[nodiscard]] QSize sizeHint() const override;

    [[nodiscard]] QSize minimumSizeHint() const override;

private:
    static constexpr int k_extraPadding { 3 };

    int m_widthPadding;
    int m_charWidth;
};
