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

#pragma once

#include <QObject>
#include <QVariant>


class PreferenceBase : public QObject {

    Q_OBJECT

public:
    explicit PreferenceBase(QObject* parent) : QObject(parent) {
    }

    [[nodiscard]] bool isDirty() const {
        return m_dirty;
    }

    void clearDirty() {
        m_dirty = false;
        emit dirtyChanged(m_dirty);
    }

signals:
    void valueChanged(QVariant newValue);
    void dirtyChanged(bool dirty);

protected:
    void markDirty() {
        m_dirty = true;
        emit dirtyChanged(m_dirty);
    }

private:
    bool m_dirty { false };
};


/// Represents a preference value. Provides a setter, getter, setToDefaultValue, value change signal, a dirty flag.
///
template<typename T>
class Preference : public PreferenceBase {

public:
    explicit Preference(T defaultValue, QObject* parent = nullptr) :
            PreferenceBase(parent),
            m_defaultValue(defaultValue),
            m_value(defaultValue) {
    }

    void initialize(T value) {
        m_value = value;
        clearDirty();
        emit valueChanged(QVariant::fromValue<T>(m_value));
    }

    void setValue(T value) {
        if (m_value != value) {
            m_value = value;
            markDirty();
            emit valueChanged(QVariant::fromValue<T>(m_value));
        }
    }

    [[nodiscard]] T getValue() const {
        return m_value;
    }

    operator T() const {    // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        return m_value;
    }

    void restore() {
        setValue(m_defaultValue);
    }

    [[nodiscard]] T getDefaultValue() const {
        return m_defaultValue;
    }

private:
    T m_defaultValue;
    T m_value;
};
