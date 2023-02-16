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
#include <QTimer>
#include <QProxyStyle>
#include <QApplication>


/// By default, the spin box control selects the value when the step buttons are pressed. This style proxy disables
/// that behavior so that the value is easier to read. An instance of the proxy is set on the data field class.
///
class DataFieldProxyStyle : public QProxyStyle {

public:
    explicit DataFieldProxyStyle(const QString& name) : QProxyStyle(name) {
    }

    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget,
                  QStyleHintReturn *returnData) const override {
        // Disable the selection when stepping the value.
        return hint == QStyle::SH_SpinBox_SelectOnStep ? 0 : QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};


/// Base class for the integer and double data fields. The base class provides a number of convenience methods
/// and allows the step buttons to emit a step signal without changing the value in the text field. This allows
/// the field to emit a value that is then used in a calculation whose result is then displayed in the text field.
///
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
    /// @return The recommended size for the widget.
    ///
    [[nodiscard]] QSize sizeHint() const override {
        // Ensure that the style is applied to this widget.
        SPIN_TYPE::ensurePolished();

        // Determine the pixel dimensions of the field based on the field style, cursor dimensions and text metrics.
        const QString s(m_charWidth, u'0');
        const int h = SPIN_TYPE::sizeHint().height();
        const int w = SPIN_TYPE::fontMetrics().horizontalAdvance(s) + m_widthPadding;

        QStyleOptionSpinBox opt;
        SPIN_TYPE::initStyleOption(&opt);
        return SPIN_TYPE::style()->sizeFromContents(QStyle::CT_SpinBox, &opt, QSize(w, h), this);
    }

    /// Calculates the minimum size of the field based on the desired character width.
    ///
    /// @return The minimum size for the widget
    ///
    [[nodiscard]] QSize minimumSizeHint() const override {
        return sizeHint();
    }

protected:
    /// Constructs a data field.
    ///
    /// @param[in] charWidth Width of the field in number of visible characters
    /// @param[in] showButtons Specify true to show the up/down arrow buttons
    /// @param[in] readOnly Configures the field to be read only
    /// @param[in] nativeStepHandling Specifies whether the data field should use its internal step handling for
    ///     arrow buttons and arrow keys
    /// @param[in] parent Parent for the widget
    ///
    AbstractDataField(int charWidth, bool showButtons, bool readOnly, bool nativeStepHandling, QWidget *parent) :
            SPIN_TYPE(parent),
            m_widthPadding(2 * SPIN_TYPE::style()->pixelMetric(QStyle::PM_TextCursorWidth) + k_extraPadding),
            m_charWidth(charWidth),
            m_nativeStepHandling(nativeStepHandling),
            m_defaultBackground(SPIN_TYPE::palette().color(QPalette::Base)),
            m_readOnlyBackground(SPIN_TYPE::palette().color(QPalette::Window)),
            m_thresholdInterval(SPIN_TYPE::style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatThreshold,
                                                              nullptr, this)),
            m_repeatInterval(SPIN_TYPE::style()->styleHint(QStyle::SH_SpinBox_ClickAutoRepeatRate, nullptr, this)),
            m_stepModifier(static_cast<Qt::KeyboardModifier>(SPIN_TYPE::style()->styleHint(QStyle::SH_SpinBox_StepModifier,
                                                                                           nullptr, this))) {
        // Install the proxy style so that the value is not selected when stepping.
        SPIN_TYPE::setStyle(new DataFieldProxyStyle(QApplication::style()->name()));

        // Configure the spin box
        SPIN_TYPE::setAutoFillBackground(true);
        SPIN_TYPE::setButtonSymbols(showButtons ? SPIN_TYPE::UpDownArrows : SPIN_TYPE::NoButtons);
        setRangeQuietly(-std::numeric_limits<DATA_TYPE>::max(), std::numeric_limits<DATA_TYPE>::max());
        SPIN_TYPE::setReadOnly(readOnly);

        if (!m_nativeStepHandling) {
            // Fire the threshold timer only once.
            m_thresholdTimer.setSingleShot(true);
            m_thresholdTimer.callOnTimeout(this, &AbstractDataField::thresholdTimeout);

            // Fire the repeat timer continually.
            m_repeatTimer.setSingleShot(false);
            m_repeatTimer.callOnTimeout(this, &AbstractDataField::repeatTimeout);

            // Disable the native spin box from incrementing the value by setting its step to 0. The actual stepping
            // is handled by this class.
            SPIN_TYPE::setSingleStep(0);
        }
    }

    void changeEvent(QEvent* ev) override {
        if (ev->type() == QEvent::ReadOnlyChange) {
            QPalette palette;
            palette.setColor(QPalette::Base, SPIN_TYPE::isReadOnly() ? m_readOnlyBackground : m_defaultBackground);
            SPIN_TYPE::setPalette(palette);
        } else if (ev->type() == QEvent::EnabledChange) {
            QPalette palette;
            palette.setColor(QPalette::Base, SPIN_TYPE::isEnabled() ? m_defaultBackground : m_readOnlyBackground);
            SPIN_TYPE::setPalette(palette);
        }

        SPIN_TYPE::changeEvent(ev);
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (!m_nativeStepHandling) {
            // Disable native stepping so that the value is not changed.
            SPIN_TYPE::setSingleStep(0);
        }

        // Always call the native handler to ensure the proper button pixmap changes
        SPIN_TYPE::mousePressEvent(event);

        if (!m_nativeStepHandling && (event->button() == Qt::LeftButton)) {
            m_keyboardModifiers = event->modifiers();

            const QStyle::SubControl arrowButton = hitTestArrowButtons(event);
            if (arrowButton == QStyle::SC_SpinBoxUp) {
                emitStepsFiltered(getStep());
                m_down = false;
                m_thresholdTimer.start(m_thresholdInterval);
            } else if (arrowButton == QStyle::SC_SpinBoxDown) {
                emitStepsFiltered(-getStep());
                m_down = true;
                m_thresholdTimer.start(m_thresholdInterval);
            }
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        // Always call the native handler to ensure the proper button pixmap changes
        SPIN_TYPE::mouseReleaseEvent(event);

        if (!m_nativeStepHandling) {
            m_keyboardModifiers = event->modifiers();

            stopRepeat();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        // Always call the native handler to ensure the proper button pixmap changes
        SPIN_TYPE::mouseMoveEvent(event);

        if (!m_nativeStepHandling) {
            m_keyboardModifiers = event->modifiers();

            // If the mouse is no longer over the step buttons, stop the timers.
            if (hitTestArrowButtons(event) == QStyle::SC_None) {
                stopRepeat();
            }
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (m_nativeStepHandling) {
            SPIN_TYPE::keyPressEvent(event);
        } else {
            m_keyboardModifiers = event->modifiers();

            switch (event->key()) {
                case Qt::Key_PageUp:
                    emitStepsFiltered(k_increasedStep);
                    break;
                case Qt::Key_PageDown:
                    emitStepsFiltered(-k_increasedStep);
                    break;
                case Qt::Key_Up:
                    emitStepsFiltered(k_step);
                    break;
                case Qt::Key_Down:
                    emitStepsFiltered(-k_step);
                    break;
                default:
                    SPIN_TYPE::keyPressEvent(event);
                    break;
            }
        }
    }

    void keyReleaseEvent(QKeyEvent *event) override {
        if (!m_nativeStepHandling) {
            m_keyboardModifiers = event->modifiers();
        }

        SPIN_TYPE::keyReleaseEvent(event);
    }

    /// Ensures that the step signal is emitted if the spin box is enabled, is not readonly and step buttons are
    /// shown.
    ///
    /// @param[in] numSteps Step value to send
    ///
    void emitStepsFiltered(int numSteps) {
        if (SPIN_TYPE::isEnabled() && !SPIN_TYPE::isReadOnly()
                && SPIN_TYPE::buttonSymbols() != QAbstractSpinBox::NoButtons) {
            emitSteps(numSteps);
        }
    }

    /// Method implemented by the derived classes to actually emit the step signal. Because this is a templated
    /// base class, it cannot emit signals.
    ///
    /// @param[in] numSteps Step value to send
    ///
    virtual void emitSteps(int numSteps) = 0;

private slots:
    void thresholdTimeout() {
        emitStepsFiltered(m_down ? -getStep() : getStep());

        m_repeatTimer.start(m_repeatInterval);
    }

    void repeatTimeout() {
        emitStepsFiltered(m_down ? -getStep() : getStep());
    }

private:
    static constexpr int k_step { 1 };              ///< Normal step value.
    static constexpr int k_increasedStep { 10 };    ///< Step value when the step modifier key is pressed.
    static constexpr int k_extraPadding { 1 };

    /// Determines the step value based on whether the step modifier button is pressed.
    ///
    /// @return Step value
    ///
    int getStep() {
        return (m_keyboardModifiers & m_stepModifier) == 0 ? k_step : k_increasedStep;
    }

    /// Stops the repeat timers.
    ///
    void stopRepeat() {
        m_thresholdTimer.stop();
        m_repeatTimer.stop();
    }

    /// Tests whether the mouse is over one of the step buttons.
    ///
    /// @param[in] event Mouse event to test
    /// @return Returns QStyle::SC_SpinBoxUp or QStyle::SC_SpinBoxDown if the mouse is over one of these buttons.
    ///     Returns QStyle::SC_None if the mouse is not over either step button.
    ///
    QStyle::SubControl hitTestArrowButtons(QMouseEvent* event) {
        QStyleOptionSpinBox opt;
        SPIN_TYPE::initStyleOption(&opt);
        opt.subControls = QStyle::SC_SpinBoxUp | QStyle::SC_SpinBoxDown;
        return SPIN_TYPE::style()->hitTestComplexControl(QStyle::CC_SpinBox, &opt, event->pos(), this);
    }

    int m_widthPadding;                                 ///< Padding around around the field text, pixels
    int m_charWidth;                                    ///< Number of characters to show in the text field
    bool m_nativeStepHandling;                          ///< Should the native spin box be used
    QColor m_defaultBackground;                         ///< Enabled and writable background color
    QColor m_readOnlyBackground;                        ///< Enabled and readonly background color
    QTimer m_thresholdTimer;                            ///< Timer for initiating the repeat
    QTimer m_repeatTimer;                               ///< Timer for the repeat
    int m_thresholdInterval;                            ///< Delay before repeating, in milliseconds
    int m_repeatInterval;                               ///< Repeat interval, in milliseconds
    Qt::KeyboardModifier m_stepModifier;                ///< Key used to increase the step value
    Qt::KeyboardModifiers m_keyboardModifiers { Qt::NoModifier };    ///< Modifiers in effect during a step and repeat
    bool m_down { false };                              ///< Indicates if stepping up (false) or down (true)
};
