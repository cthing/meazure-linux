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

#include "LineEdit.h"
#include <QStyle>
#include <QStyleOptionFrame>


LineEdit::LineEdit(int charWidth, QWidget* parent) :
        QLineEdit(parent),
        m_widthPadding(2 * style()->pixelMetric(QStyle::PM_TextCursorWidth) + k_extraPadding),
        m_charWidth(charWidth) {
}

QSize LineEdit::sizeHint() const {
    // Ensure that the style is applied to this widget.
    ensurePolished();

    // Determine the pixel dimensions of the field based on the field style, cursor dimensions and text metrics.
    const QString s(m_charWidth, u'm');
    const int h = QLineEdit::sizeHint().height();
    const int w = fontMetrics().horizontalAdvance(s) + m_widthPadding;

    QStyleOptionFrame opt;
    initStyleOption(&opt);
    return style()->sizeFromContents(QStyle::CT_LineEdit, &opt, QSize(w, h), this);
}

QSize LineEdit::minimumSizeHint() const {
    return sizeHint();
}
