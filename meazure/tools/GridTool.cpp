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

#include "GridTool.h"
#include <meazure/utils/Geometry.h>
#include <algorithm>

GridTool::GridTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent) :   // NOLINT(cppcoreguidelines-pro-type-member-init)
        Tool(screenInfo, unitsProvider, parent),
        m_grid(new Grid(screenInfo, unitsProvider)) {
    init();
}

GridTool::~GridTool() {
    setEnabled(false);
    delete m_grid;
}

void GridTool::init() {
    const QRect screenRect = m_screenInfo->getVirtualRect();
    m_x = screenRect.left();
    m_y = screenRect.top();
    m_width = screenRect.width();
    m_height = screenRect.height();
    m_angle = 0;
    m_hSpacing = k_defaultSpacing;
    m_vSpacing = k_defaultSpacing;
    m_linkedSpacing = true;
    m_units = k_defaultUnits;
}

void GridTool::setEnabled(bool enable) {
    Tool::setEnabled(enable);

    if (enable) {
        m_grid->show();
    } else {
        m_grid->hide();
    }
}

void GridTool::writeConfig(Config& config) const {
    config.writeBool("ScrnGrid", isEnabled());
    config.writeInt("ScrnGridX", m_x);
    config.writeInt("ScrnGridY", m_y);
    config.writeInt("ScrnGridW", m_width);
    config.writeInt("ScrnGridH", m_height);
    config.writeInt("ScrnGridA", m_angle);
    config.writeDbl("ScrnGridSpaceX", m_hSpacing);
    config.writeDbl("ScrnGridSpaceY", m_vSpacing);
    config.writeBool("ScrnGridLinked", m_linkedSpacing);
    config.writeStr("ScrnGridUnits", m_unitsProvider->getLinearUnits(m_units)->getUnitsStr());
}

void GridTool::readConfig(const Config& config) {
    m_x = config.readInt("ScrnGridX", m_x);
    m_y = config.readInt("ScrnGridY", m_y);
    m_width = config.readInt("ScrnGridW", m_width);
    m_height = config.readInt("ScrnGridH", m_height);
    m_angle = config.readInt("ScrnGridA", m_angle);
    m_hSpacing = config.readDbl("ScrnGridSpaceX", m_hSpacing);
    m_vSpacing = config.readDbl("ScrnGridSpaceY", m_vSpacing);
    m_linkedSpacing = config.readBool("ScrnGridLinked", m_linkedSpacing);

    const QString defaultUnits = m_unitsProvider->getLinearUnits(PixelsId)->getUnitsStr();
    m_units = m_unitsProvider->getLinearUnits(config.readStr("ScrnGridUnits", defaultUnits))->getUnitsId();

    refresh();
    setEnabled(config.readBool("ScrnGrid", false));
}

void GridTool::softReset() {
    init();
    refresh();
}

void GridTool::hardReset() {
    softReset();
    setEnabled(false);
}

void GridTool::refresh() {
    setOrientation();
    setSpacing();
}

void GridTool::setOrientation(int x, int y, int width, int height, int angle) {
    m_x = x;
    m_y = y;
    m_width = std::clamp(width, k_minSize, k_maxSize);
    m_height = std::clamp(height, k_minSize, k_maxSize);
    m_angle = Geometry::normalizeDegrees(angle);
    setOrientation();
}

void GridTool::setXPosition(int x) {
    m_x = x;
    setOrientation();
}

void GridTool::setYPosition(int y) {
    m_y = y;
    setOrientation();
}

void GridTool::setWidth(int width) {
    m_width = std::clamp(width, k_minSize, k_maxSize);
    setOrientation();
}

void GridTool::setHeight(int height) {
    m_height = std::clamp(height, k_minSize, k_maxSize);
    setOrientation();
}

void GridTool::setAngle(int angle) {
    m_angle = Geometry::normalizeDegrees(angle);
    setOrientation();
}

void GridTool::setSpacing(double hspace, double vspace, bool linkSpacing, LinearUnitsId units) {
    if (hspace <= 0.0 || vspace <= 0.0) {
        return;
    }

    m_linkedSpacing = linkSpacing;

    m_hSpacing = hspace;
    if (m_linkedSpacing) {
        m_vSpacing = hspace;
    } else {
        m_vSpacing = vspace;
    }

    m_units = units;

    setSpacing();
}

void GridTool::setHorizontalSpacing(double hspace) {
    if (hspace > 0.0) {
        m_hSpacing = hspace;
        if (m_linkedSpacing) {
            m_vSpacing = hspace;
        }
        setSpacing();
    }
}

void GridTool::setVerticalSpacing(double vspace) {
    if (vspace > 0.0) {
        if (m_linkedSpacing) {
            m_vSpacing = m_hSpacing;
        } else {
            m_vSpacing = vspace;
        }
        setSpacing();
    }
}

void GridTool::setLinkedSpacing(bool linkSpacing) {
    m_linkedSpacing = linkSpacing;
    if (m_linkedSpacing) {
        m_vSpacing = m_hSpacing;
        setSpacing();
    }
}

void GridTool::setSpacing() {
    m_grid->setUnits(m_units);
    m_grid->setSpacing(m_hSpacing, m_vSpacing);

    emit spacingChanged(m_hSpacing, m_vSpacing, m_linkedSpacing, m_units);
}

void GridTool::setOrientation() {
    m_grid->setPosition(m_x, m_y, m_width, m_height, m_angle);
}
