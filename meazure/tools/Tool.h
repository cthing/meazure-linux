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

#include <meazure/units/UnitsProvider.h>
#include <QObject>


/// Abstract base class for all measurement tools. Both radio and non-radio measurement tools inherit from this
/// base class.
///
class Tool : public QObject {

    Q_OBJECT

public:
    explicit Tool(const UnitsProvider& unitsProvider, QObject* parent = nullptr);

    ~Tool() override = default;

    Tool(const Tool&) = delete;
    Tool(Tool&&) = delete;
    Tool& operator=(const Tool&) = delete;

    /// Returns the name of the tool. Each tool has a unique name which is used to identify the tool in profiles and
    /// position logs. Because the names are persisted, they should not be changed without support for the old name
    /// to maintain backwards compatibility.
    ///
    /// @return Name of the tool. Will never be nullptr and the memory for the string is owned by the tool.
    ///
    [[nodiscard]] virtual const char* getName() const = 0;

    /// Indicates whether the derived tool is a radio tool and cannot be enabled until any other enabled Radio tool
    /// is disabled.
    ///
    /// @return Always false indicating that this class and its derived classes are not radio tools unless they
    ///     have overridden this method.
    ///
    [[nodiscard]] virtual bool isRadioTool() {
        return false;
    }

    /// Sets whether the tool is enabled for use. Typically, enabling a tool makes it visible.
    ///
    /// @param[in] enable true to enable the tool for use
    ///
    virtual void setEnabled(bool enable);

    /// Indicates whether the tool is enabled for use.
    ///
    /// @return true if the tool is enabled for use.
    ///
    [[nodiscard]] bool isEnabled() const {
        return m_enabled;
    }

    /// Causes the tool to remeasure thereby emitting any measurement related signals. Typically, this method is
    /// called when the measurement units are changed or when the tool is first selected. Does nothing if the tool
    /// is not enabled. The base class implementation does nothing.
    ///
    virtual void refresh();

protected:
    [[nodiscard]] const UnitsProvider& getUnitsProvider() const {
        return m_unitsProvider;
    }

private:
    const UnitsProvider& m_unitsProvider;
    bool m_enabled { false };
};
