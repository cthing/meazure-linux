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
#include <QString>
#include <QSignalBlocker>
#include <QStyleOptionSpinBox>
#include <QFontMetrics>
#include <QEvent>
#include <QMouseEvent>


template <typename SPIN_TYPE, typename DATA_TYPE>
class AbstractDataField: public SPIN_TYPE {

public:
    /// Sets the value in the spin box without emitting any signals.
    ///
    /// @param[in] value Spin box value
    ///
    void setValueQuietly(DATA_TYPE value) {
        const QSignalBlocker blocker(this);
        SPIN_TYPE::setValue(value);
    }

    /// Sets the minimum and maximum values for the field without emitting any signals.
    ///
    /// @param[in] minimum Minimum field value
    /// @param[in] maximum Maximum field value
    ///
    void setRangeQuietly(DATA_TYPE minimum, DATA_TYPE maximum) {
        const QSignalBlocker blocker(this);
        SPIN_TYPE::setRange(minimum, maximum);
    }

    /// Sets the minimum value for the field without emitting any signals.
    ///
    /// @param[in] minimum Minimum field value
    ///
    void setMinimumQuietly(DATA_TYPE minimum) {
        const QSignalBlocker blocker(this);
        SPIN_TYPE::setMinimum(minimum);
    }

    /// Calculates the size of the field based on the desired character width.
    ///
    [[nodiscard]] QSize sizeHint() const override {
        SPIN_TYPE::ensurePolished();

        const QString s(m_charWidth, u'0');
        const int h = SPIN_TYPE::sizeHint().height();
        const int w = SPIN_TYPE::fontMetrics().horizontalAdvance(s) + k_cursorBlinkingSpace;

        QStyleOptionSpinBox opt;
        SPIN_TYPE::initStyleOption(&opt);

        return SPIN_TYPE::style()->sizeFromContents(QStyle::CT_SpinBox, &opt, QSize(w, h), this);
    }

    /// Calculates the minimum size of the field based on the desired character width.
    ///
    [[nodiscard]] QSize minimumSizeHint() const override {
        return sizeHint();
    }

protected:
    explicit AbstractDataField(int charWidth, bool showButtons, bool readOnly, bool nativeStepHandling,
                               QWidget *parent) :
            SPIN_TYPE(parent),
            m_charWidth(charWidth),
            m_nativeStepHandling(nativeStepHandling),
            m_defaultBackground(SPIN_TYPE::palette().color(QPalette::Base)) {
        SPIN_TYPE::setAutoFillBackground(true);
        SPIN_TYPE::setButtonSymbols(showButtons ? SPIN_TYPE::UpDownArrows : SPIN_TYPE::NoButtons);
        setRangeQuietly(-std::numeric_limits<DATA_TYPE>::max(), std::numeric_limits<DATA_TYPE>::max());
        SPIN_TYPE::setReadOnly(readOnly);
    }

    void changeEvent(QEvent *ev) override {
        if (ev->type() == QEvent::ReadOnlyChange) {
            QPalette palette;
            palette.setColor(QPalette::Base, SPIN_TYPE::isReadOnly() ? m_readOnlyBackground : m_defaultBackground);
            SPIN_TYPE::setPalette(palette);
        } else if (ev->type() == QEvent::EnabledChange) {
            QPalette palette;
            palette.setColor(QPalette::Base, SPIN_TYPE::isEnabled() ? m_defaultBackground : m_readOnlyBackground);
            SPIN_TYPE::setPalette(palette);
            if (!SPIN_TYPE::isEnabled()) {
                setValueQuietly(0);
            }
        }

        SPIN_TYPE::changeEvent(ev);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (m_nativeStepHandling) {
            SPIN_TYPE::mousePressEvent(event);
        } else {
            QStyleOptionSpinBox opt;
            this->initStyleOption(&opt);
            const QRect upRect = this->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxUp);
            const QRect downRect = this->style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxDown);
            if (upRect.contains(event->pos())) {
                emitSteps(1);
            } else if (downRect.contains(event->pos())) {
                emitSteps(-1);
            } else {
                SPIN_TYPE::mousePressEvent(event);
            }
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (m_nativeStepHandling) {
            SPIN_TYPE::keyPressEvent(event);
        } else {
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
                    SPIN_TYPE::keyPressEvent(event);
                    break;
            }
        }
    }

    virtual void emitSteps(int numSteps) = 0;

private:
    static constexpr int k_cursorBlinkingSpace { 2 };

    int m_charWidth;
    bool m_nativeStepHandling;
    QColor m_defaultBackground;
    QColor m_readOnlyBackground { 240, 240, 240 };
};
