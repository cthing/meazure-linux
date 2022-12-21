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

void Dimensions::reset() {
    lineWidth = k_defaultLineWidth;
}

void Dimensions::saveProfile(Profile& profile) {
    if (!profile.userInitiated()) {
        profile.writeInt("LineWidth", lineWidth);
    }
}

void Dimensions::loadProfile(Profile& profile) {
    if (!profile.userInitiated()) {
        lineWidth = profile.readInt("LineWidth", k_defaultLineWidth);
    }
}

void Dimensions::setLineWidth(int width) {
    lineWidth = width;
    emit getChangeNotifier()->lineWidthChanged(width);
}

int Dimensions::getLineWidth() {
    return lineWidth;
}

int Dimensions::getDefaultLineWidth() {
    return k_defaultLineWidth;
}
