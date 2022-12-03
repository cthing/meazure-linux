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
#include "AngleTool.h"
#include "CircleTool.h"
#include "CursorTool.h"
#include "GridTool.h"
#include "LineTool.h"
#include "PointTool.h"
#include "RadioTool.h"
#include "RectangleTool.h"
#include "WindowTool.h"
#include "RulerTool.h"


ToolMgr::ToolMgr(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider) {
    // Radio tools
    //
    Tool* cursorTool = new CursorTool(screenInfoProvider, unitsProvider, this);
    Tool* pointTool = new PointTool(screenInfoProvider, unitsProvider, this);
    Tool* lineTool = new LineTool(screenInfoProvider, unitsProvider, this);
    Tool* rectangleTool = new RectangleTool(screenInfoProvider, unitsProvider, this);
    Tool* circleTool = new CircleTool(screenInfoProvider, unitsProvider, this);
    Tool* angleTool = new AngleTool(screenInfoProvider, unitsProvider, this);
    Tool* windowTool = new WindowTool(screenInfoProvider, unitsProvider, this);

    // Non-radio tools
    //
    Tool* rulerTool = new RulerTool(screenInfoProvider, unitsProvider, this);
    Tool* gridTool = new GridTool(screenInfoProvider, unitsProvider, this);

    m_tools[cursorTool->getName()] = cursorTool;
    m_tools[pointTool->getName()] = pointTool;
    m_tools[lineTool->getName()] = lineTool;
    m_tools[rectangleTool->getName()] = rectangleTool;
    m_tools[circleTool->getName()] = circleTool;
    m_tools[angleTool->getName()] = angleTool;
    m_tools[windowTool->getName()] = windowTool;
    m_tools[rulerTool->getName()] = rulerTool;
    m_tools[gridTool->getName()] = gridTool;

    connect(cursorTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));

    connect(pointTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));

    connect(lineTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
    connect(lineTool, SIGNAL(xy2PositionChanged(QPointF)), this, SIGNAL(xy2PositionChanged(QPointF)));
    connect(lineTool, SIGNAL(widthHeightChanged(QSizeF)), this, SIGNAL(widthHeightChanged(QSizeF)));
    connect(lineTool, SIGNAL(distanceChanged(double)), this, SIGNAL(distanceChanged(double)));
    connect(lineTool, SIGNAL(angleChanged(double)), this, SIGNAL(angleChanged(double)));
    connect(lineTool, SIGNAL(areaChanged(double)), this, SIGNAL(areaChanged(double)));
    connect(lineTool, SIGNAL(aspectChanged(double)), this, SIGNAL(aspectChanged(double)));

    connect(rectangleTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
    connect(rectangleTool, SIGNAL(xy2PositionChanged(QPointF)), this, SIGNAL(xy2PositionChanged(QPointF)));
    connect(rectangleTool, SIGNAL(widthHeightChanged(QSizeF)), this, SIGNAL(widthHeightChanged(QSizeF)));
    connect(rectangleTool, SIGNAL(distanceChanged(double)), this, SIGNAL(distanceChanged(double)));
    connect(rectangleTool, SIGNAL(angleChanged(double)), this, SIGNAL(angleChanged(double)));
    connect(rectangleTool, SIGNAL(areaChanged(double)), this, SIGNAL(areaChanged(double)));
    connect(rectangleTool, SIGNAL(aspectChanged(double)), this, SIGNAL(aspectChanged(double)));

    connect(circleTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
    connect(circleTool, SIGNAL(xyvPositionChanged(QPointF)), this, SIGNAL(xyvPositionChanged(QPointF)));
    connect(circleTool, SIGNAL(widthHeightChanged(QSizeF)), this, SIGNAL(widthHeightChanged(QSizeF)));
    connect(circleTool, SIGNAL(distanceChanged(double)), this, SIGNAL(distanceChanged(double)));
    connect(circleTool, SIGNAL(angleChanged(double)), this, SIGNAL(angleChanged(double)));
    connect(circleTool, SIGNAL(areaChanged(double)), this, SIGNAL(areaChanged(double)));
    connect(circleTool, SIGNAL(aspectChanged(double)), this, SIGNAL(aspectChanged(double)));

    connect(angleTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
    connect(angleTool, SIGNAL(xy2PositionChanged(QPointF)), this, SIGNAL(xy2PositionChanged(QPointF)));
    connect(angleTool, SIGNAL(xyvPositionChanged(QPointF)), this, SIGNAL(xyvPositionChanged(QPointF)));
    connect(angleTool, SIGNAL(angleChanged(double)), this, SIGNAL(angleChanged(double)));

    connect(windowTool, SIGNAL(xy1PositionChanged(QPointF)), this, SIGNAL(xy1PositionChanged(QPointF)));
    connect(windowTool, SIGNAL(xy2PositionChanged(QPointF)), this, SIGNAL(xy2PositionChanged(QPointF)));
    connect(windowTool, SIGNAL(widthHeightChanged(QSizeF)), this, SIGNAL(widthHeightChanged(QSizeF)));
    connect(windowTool, SIGNAL(distanceChanged(double)), this, SIGNAL(distanceChanged(double)));
    connect(windowTool, SIGNAL(angleChanged(double)), this, SIGNAL(angleChanged(double)));
    connect(windowTool, SIGNAL(areaChanged(double)), this, SIGNAL(areaChanged(double)));
    connect(windowTool, SIGNAL(aspectChanged(double)), this, SIGNAL(aspectChanged(double)));
}

void ToolMgr::saveProfile(Profile& profile) const {
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->saveProfile(profile);
    }

    profile.writeStr("CurrentRadioTool", m_currentRadioTool->getName());
}

void ToolMgr::loadProfile(Profile& profile) {
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->loadProfile(profile);
    }

    const QString currentRadioToolName = profile.readStr("CurrentRadioTool", m_currentRadioTool->getName());
    selectRadioTool(currentRadioToolName.toUtf8().constData());
}

void ToolMgr::masterReset() {
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->masterReset();
    }

    selectRadioTool(CursorTool::k_toolName);
}

Tool* ToolMgr::getTool(const char *toolName) const {
    return m_tools.at(toolName);
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

void ToolMgr::setEnabled(const char *toolName, bool enable) {
    Tool* tool = m_tools.at(toolName);
    tool->setEnabled(enable);

    if (enable) {
        tool->refresh();
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
    for (const auto& toolEntry : m_tools) {
        toolEntry.second->refresh();
    }
}
