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

#include "RadioTool.h"


RadioTool::RadioTool(const ScreenInfoProvider& screenInfo, const UnitsProvider& unitsProvider, QObject *parent) :
        Tool(screenInfo, unitsProvider, parent) {
}

void RadioTool::setCrosshairsEnabled(bool) {
}

void RadioTool::flash() {
}

void RadioTool::strobe() {
}

bool RadioTool::hasCrosshairs() const {
    return true;
}

void RadioTool::setX1Position(double) {
}

void RadioTool::setY1Position(double) {
}

void RadioTool::setX2Position(double) {
}

void RadioTool::setY2Position(double) {
}

void RadioTool::setXVPosition(double) {
}

void RadioTool::setYVPosition(double) {
}

void RadioTool::stepX1Position(int) {
}

void RadioTool::stepY1Position(int) {
}

void RadioTool::stepX2Position(int) {
}

void RadioTool::stepY2Position(int) {
}

void RadioTool::stepXVPosition(int) {
}

void RadioTool::stepYVPosition(int) {
}
