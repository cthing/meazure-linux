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

#include "Tool.h"
#include "RadioTool.h"
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <QObject>
#include <QString>
#include <QPointF>
#include <QSizeF>
#include <map>


/// Manages the measurement tools including selection, enabling, disabling, and communicating various messages
/// to the tools.
///
/// The term "radio tool" refers to those measurement tools that are mutually exclusive (i.e. only one radio tool can
/// be used at a time).
///
class ToolMgr : public QObject {

    Q_OBJECT

public:
    ~ToolMgr() override = default;

    ToolMgr(const ToolMgr&) = delete;
    ToolMgr(ToolMgr&&) = delete;
    ToolMgr& operator=(const ToolMgr&) = delete;

    /// Causes the current tool to remeasure thereby emitting any measurement related signals. Typically, this method
    /// is called when the measurement units are changed or when the tool is first selected. Does nothing if the tool
    /// is not enabled. The base class implementation does nothing.
    ///
    void refresh();

public slots:
    /// Selects the specified radio tool making it visible and ready for providing measurements. The radioToolSelected
    /// signal is emitted.
    ///
    /// @param[in] toolName Name of the radio tool to select
    ///
    void selectRadioTool(const char* toolName);

signals:
    /// Called when a radio tool has been selected by a call to selectRadioTool.
    ///
    void radioToolSelected(RadioTool& tool);

    void xy1PositionChanged(QPointF coord);
    void xy2PositionChanged(QPointF coord);
    void xyvPositionChanged(QPointF coord);
    void widthHeightChanged(QSizeF widthHeight);
    void distanceChanged(double distance);
    void angleChanged(double angle);
    void aspectChanged(double aspect);
    void areaChanged(double area);

private:
    using ToolsMap = std::map<QString, Tool*>;   ///< Maps a tool name to the tool object.


    explicit ToolMgr(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider);

    ToolsMap m_tools;
    RadioTool* m_currentRadioTool { nullptr };

    friend class App;
};
