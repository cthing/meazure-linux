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

#include "TextEdit.h"


TextEdit::TextEdit(QWidget* parent) : QPlainTextEdit(parent) {
}

void TextEdit::setRows(int numRows) {
    QTextDocument* doc = document();
    const QFontMetrics fm(doc->defaultFont());
    const QMargins margins = contentsMargins();
    const int fieldHeight = fm.lineSpacing() * numRows + (static_cast<int>(doc->documentMargin()) + frameWidth()) * 2 +
            margins.top() + margins.bottom();
    setFixedHeight(fieldHeight);
}
