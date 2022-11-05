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

#include "ToolMgr.h"
#include "CursorTool.h"
#include "PointTool.h"
#include "RadioTool.h"


ToolMgr::ToolMgr(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider) {
    // Radio tools
    //
    Tool* cursorTool = new CursorTool(screenInfoProvider, unitsProvider, this);
    Tool* pointTool = new PointTool(screenInfoProvider, unitsProvider, this);

    m_tools[cursorTool->getName()] = cursorTool;
    m_tools[pointTool->getName()] = pointTool;

    connect(cursorTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
    connect(pointTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
}

void ToolMgr::selectRadioTool(const char *toolName) {
    Tool* tool = m_tools.at(toolName);
    if (tool->isRadioTool()) {
        for (auto entry : m_tools) {
            if (entry.second->isRadioTool()) {
                entry.second->setEnabled(false);
            }
        }

        tool->setEnabled(true);

        m_currentRadioTool = dynamic_cast<RadioTool*>(tool);
        m_currentRadioTool->flash();

        emit radioToolSelected(*m_currentRadioTool);

        refresh();
    }
}

void ToolMgr::setX1Position(double x) {
    m_currentRadioTool->setX1Position(x);
}

void ToolMgr::setY1Position(double y) {
    m_currentRadioTool->setY1Position(y);
}

void ToolMgr::setX2Position(double x) {
    m_currentRadioTool->setX2Position(x);
}

void ToolMgr::setY2Position(double y) {
    m_currentRadioTool->setY2Position(y);
}

void ToolMgr::setXVPosition(double x) {
    m_currentRadioTool->setXVPosition(x);
}

void ToolMgr::setYVPosition(double y) {
    m_currentRadioTool->setYVPosition(y);
}

void ToolMgr::stepX1Position(int numSteps) {
    m_currentRadioTool->stepX1Position(numSteps);
}

void ToolMgr::stepY1Position(int numSteps) {
    m_currentRadioTool->stepY1Position(numSteps);
}

void ToolMgr::stepX2Position(int numSteps) {
    m_currentRadioTool->stepX2Position(numSteps);
}

void ToolMgr::stepY2Position(int numSteps) {
    m_currentRadioTool->stepY2Position(numSteps);
}

void ToolMgr::stepXVPosition(int numSteps) {
    m_currentRadioTool->stepXVPosition(numSteps);
}

void ToolMgr::stepYVPosition(int numSteps) {
    m_currentRadioTool->stepYVPosition(numSteps);
}

void ToolMgr::refresh() {
    m_currentRadioTool->refresh();
}
