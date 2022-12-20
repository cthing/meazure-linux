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

#include "ToolPrefsModel.h"
#include <meazure/graphics/Colors.h>
#include <meazure/graphics/Dimensions.h>


ToolPrefsModel::ToolPrefsModel(QObject* parent) :
        QObject(parent),
        m_lineForeColor(new Preference(Colors::getDefault(Colors::LineFore), this)),
        m_crosshairBackColor(new Preference(Colors::getDefault(Colors::CrossHairBack), this)),
        m_crosshairBorderColor(new Preference(Colors::getDefault(Colors::CrossHairBorder), this)),
        m_crosshairHiliteColor(new Preference(Colors::getDefault(Colors::CrossHairHilite), this)),
        m_crosshairOpacity(new Preference(Colors::opacityToPercent(Colors::getDefault(Colors::CrossHairOpacity)), this)),
        m_lineWidth(new Preference(Dimensions::getDefaultLineWidth(), this)) {
}

void ToolPrefsModel::update() {     // NOLINT(readability-make-member-function-const)
    m_lineForeColor->update(Colors::get(Colors::LineFore));
    m_crosshairBackColor->update(Colors::get(Colors::CrossHairBack));
    m_crosshairBorderColor->update(Colors::get(Colors::CrossHairBorder));
    m_crosshairHiliteColor->update(Colors::get(Colors::CrossHairHilite));
    m_crosshairOpacity->update(Colors::opacityToPercent(Colors::get(Colors::CrossHairOpacity)));
    m_lineWidth->update(Dimensions::getLineWidth());
}
