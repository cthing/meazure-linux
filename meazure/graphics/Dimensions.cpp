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

#include "Dimensions.h"


static constexpr int k_defaultLineWidth { 1 };     ///< Pixels

static int lineWidth = k_defaultLineWidth;


Dimensions::ChangeNotifier* Dimensions::getChangeNotifier() {
    static Dimensions::ChangeNotifier changeNotifier;

    return &changeNotifier;
}

void Dimensions::hardReset() {
    setLineWidth(k_defaultLineWidth);
}

void Dimensions::writeConfig(Config& config) {
    config.writeInt("LineWidth", lineWidth);
}

void Dimensions::readConfig(const Config& config) {
    lineWidth = config.readInt("LineWidth", k_defaultLineWidth);
    emit getChangeNotifier()->lineWidthChanged(lineWidth);
}

void Dimensions::setLineWidth(int width) {
    lineWidth = width;
    emit getChangeNotifier()->lineWidthChanged(lineWidth);
}

int Dimensions::getLineWidth() {
    return lineWidth;
}

int Dimensions::getDefaultLineWidth() {
    return k_defaultLineWidth;
}
