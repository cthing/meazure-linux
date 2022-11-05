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

#include "DataField.h"
#include <QStyle>
#include <QStyleOptionSpinBox>
#include <QFontMetrics>
#include <QString>
#include <QEvent>
#include <QMouseEvent>
#include <limits>


DataField::DataField(int charWidth, bool showButtons, bool readOnly, QWidget *parent) :
        QDoubleSpinBox(parent),
        m_charWidth(charWidth),
        m_defaultBackground(palette().color(QPalette::Base)) {
    setButtonSymbols(showButtons ? UpDownArrows : NoButtons);
    setRange(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
    setReadOnly(readOnly);
}

QSize DataField::minimumSizeHint() const {
    return sizeHint();
}

QSize DataField::sizeHint() const {
    ensurePolished();

    const QString s(m_charWidth, u'0');
    const int h = QDoubleSpinBox::sizeHint().height();
    const int w = fontMetrics().horizontalAdvance(s) + k_cursorBlinkingSpace;

    QStyleOptionSpinBox opt;
    initStyleOption(&opt);

    return style()->sizeFromContents(QStyle::CT_SpinBox, &opt, QSize(w, h), this);
}

bool DataField::event(QEvent* ev)  {
    if (ev->type() == QEvent::ReadOnlyChange) {
        QPalette palette;
        palette.setColor(QPalette::Base, isReadOnly() ? m_readOnlyBackground : m_defaultBackground);
        setPalette(palette);
    }

    return QDoubleSpinBox::event(ev);
}

void DataField::mousePressEvent(QMouseEvent *event) {
    QStyleOptionSpinBox opt;
    this->initStyleOption(&opt);
    const QRect upRect = this->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxUp);
    const QRect downRect = this->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxDown);
    if (upRect.contains(event->pos())) {
        emitSteps(1);
    } else if (downRect.contains(event->pos())) {
        emitSteps(-1);
    } else {
        QDoubleSpinBox::mousePressEvent(event);
    }
}

void DataField::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_PageUp:
            emitSteps(10);
            break;
        case Qt::Key_PageDown:
            emitSteps(-10);
            break;
        case Qt::Key_Up:
            emitSteps(1);
            break;
        case Qt::Key_Down:
            emitSteps(-1);
            break;
        default:
            QDoubleSpinBox::keyPressEvent(event);
            break;
   }
}

void DataField::emitSteps(int numSteps) {
    if (isEnabled() && !isReadOnly() && buttonSymbols() != NoButtons) {
        emit stepRequested(numSteps);
    }
}
