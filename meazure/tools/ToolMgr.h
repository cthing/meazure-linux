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
#include <meazure/profile/Profile.h>
#include <QObject>
#include <QString>
#include <QPointF>
#include <QPoint>
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

    /// Persists the state of the manager to the specified profile object.
    ///
    /// @param[in] profile Profile object into which the state is to be persisted.
    ///
    void saveProfile(Profile& profile) const;

    /// Restores the state of the manager from the specified profile object.
    ///
    /// @param[in] profile Profile object from which the state is to be restored.
    ///
    void loadProfile(Profile& profile);

    /// Resets the tool manager to its default state.
    ///
    void masterReset();

    /// Causes the current tool to remeasure thereby emitting any measurement related signals. Typically, this method
    /// is called when the measurement units are changed or when the tool is first selected. Does nothing if the tool
    /// is not enabled. The base class implementation does nothing.
    ///
    void refresh();

    /// Obtains the specified tool. Note that this method does not activate the requested tool.
    ///
    /// @param[in] toolName Name of the tool to obtain
    /// @return The requested tool.
    /// @throws std::out_of_range if the requested tool is not found
    ///
    Tool* getTool(const char* toolName) const;

public slots:
    /// Selects the specified radio tool making it visible and ready for providing measurements. The radioToolSelected
    /// signal is emitted.
    ///
    /// @param[in] toolName Name of the radio tool to select
    ///
    void selectRadioTool(const char* toolName);

    /// Enables or disabled the specified tool. This method should only be called on non-radio tool.
    ///
    /// @param[in] toolName Name of the tool to enable or disable
    /// @param[in] enable true to enable the tool or false to disable it
    ///
    void setEnabled(const char* toolName, bool enable);

    /// Sets the current radio tool's position 1 x coordinate.
    ///
    /// @param[in] x X coordinate in the current units, origin and y-axis direction.
    ///
    void setX1Position(double x);

    /// Sets the current radio tool's position 1 y coordinate.
    ///
    /// @param[in] y Y coordinate in the current units, origin and y-axis direction.
    ///
    void setY1Position(double y);

    /// Sets the current radio tool's position 2 x coordinate.
    ///
    /// @param[in] x X coordinate in the current units, origin and y-axis direction.
    ///
    void setX2Position(double x);

    /// Sets the current radio tool's position 2 y coordinate.
    ///
    /// @param[in] y Y coordinate in the current units, origin and y-axis direction.
    ///
    void setY2Position(double y);

    /// Sets the current radio tool's vertex position x coordinate.
    ///
    /// @param[in] x X coordinate in the current units, origin and y-axis direction.
    ///
    void setXVPosition(double x);

    /// Sets the current radio tool's vertex position y coordinate.
    ///
    /// @param[in] y Y coordinate in the current units, origin and y-axis direction.
    ///
    void setYVPosition(double y);

    /// Increments or decrements the current radio tool's position 1 x coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the x coordinate
    ///
    void stepX1Position(int numSteps);

    /// Increments or decrements the current radio tool's position 1 y coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the y coordinate
    ///
    void stepY1Position(int numSteps);

    /// Increments or decrements the current radio tool's position 2 x coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the x coordinate
    ///
    void stepX2Position(int numSteps);

    /// Increments or decrements the current radio tool's position 2 y coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the y coordinate
    ///
    void stepY2Position(int numSteps);

    /// Increments or decrements the current radio tool's vertex position x coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the x coordinate
    ///
    void stepXVPosition(int numSteps);

    /// Increments or decrements the current radio tool's vertex position y coordinate.
    ///
    /// @param[in] numSteps Number of pixels to increment or decrement the y coordinate
    ///
    void stepYVPosition(int numSteps);

signals:
    /// Called when a radio tool has been selected by a call to selectRadioTool.
    ///
    void radioToolSelected(RadioTool& tool);

    void activePositionChanged(QPoint rawPos);

    void xy1PositionChanged(QPointF coord, QPoint rawPos);
    void xy2PositionChanged(QPointF coord, QPoint rawPos);
    void xyvPositionChanged(QPointF coord, QPoint rawPos);
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
