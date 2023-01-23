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

#include "OriginTool.h"
#include <QPoint>

OriginTool::OriginTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent) :
        Tool(screenInfo, unitsProvider, parent),
        m_marker(new OriginMarker(screenInfo, unitsProvider)) {
}

OriginTool::~OriginTool() {
    setEnabled(false);
    delete m_marker;
}

void OriginTool::setEnabled(bool enable) {
    Tool::setEnabled(enable);

    if (enable) {
        m_marker->show();
    } else {
        m_marker->hide();
    }
}

void OriginTool::writeConfig(Config& config) const {
    config.writeBool("OriginMarker", isEnabled());
}

void OriginTool::readConfig(const Config& config) {
    setEnabled(config.readBool("OriginMarker", isEnabled()));

    setPosition();
}

void OriginTool::setPosition() {
    QPoint origin = m_unitsProvider->getOrigin();
    const bool inverted = m_unitsProvider->isInvertY();

    if (inverted && (origin.x() == 0) && (origin.y() == 0)) {
        origin.ry() = m_screenInfo->getVirtualRect().height() - 1;
    }

    m_marker->setPosition(origin, inverted);
}

void OriginTool::refresh() {
    setPosition();
    m_marker->repaint();
}
