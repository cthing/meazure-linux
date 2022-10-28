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

#include "PointTool.h"


PointTool::PointTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                     QObject *parent) : RadioTool(parent) {
    m_crosshair = new CrossHair(screenInfoProvider, unitsProvider, nullptr, tr("Point 1"));
}

PointTool::~PointTool() {
    delete m_crosshair;
}

void PointTool::setEnabled(bool enable) {
    RadioTool::setEnabled(enable);

    if (enable) {
        m_crosshair->show();
    } else {
        m_crosshair->hide();
    }
}

void PointTool::flash() {
    m_crosshair->flash();
}
