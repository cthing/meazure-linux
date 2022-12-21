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
        m_crosshairHighlightColor(new Preference(Colors::getDefault(Colors::CrossHairHighlight), this)),
        m_crosshairOpacity(new Preference(Colors::opacityToPercent(Colors::getDefault(Colors::CrossHairOpacity)), this)),
        m_lineWidth(new Preference(Dimensions::getDefaultLineWidth(), this)) {
    connect(m_lineForeColor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_crosshairBackColor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_crosshairBorderColor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_crosshairHighlightColor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_crosshairOpacity, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_lineWidth, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
}

void ToolPrefsModel::initialize() {     // NOLINT(readability-make-member-function-const)
    m_lineForeColor->initialize(Colors::get(Colors::LineFore));
    m_crosshairBackColor->initialize(Colors::get(Colors::CrossHairBack));
    m_crosshairBorderColor->initialize(Colors::get(Colors::CrossHairBorder));
    m_crosshairHighlightColor->initialize(Colors::get(Colors::CrossHairHighlight));
    m_crosshairOpacity->initialize(Colors::opacityToPercent(Colors::get(Colors::CrossHairOpacity)));
    m_lineWidth->initialize(Dimensions::getLineWidth());
}

void ToolPrefsModel::apply() const {
    if (m_lineForeColor->isDirty()) {
        Colors::set(Colors::LineFore, *m_lineForeColor);
        m_lineForeColor->clearDirty();
    }
    if (m_crosshairBackColor->isDirty()) {
        Colors::set(Colors::CrossHairBack, *m_crosshairBackColor);
        m_crosshairBackColor->clearDirty();
    }
    if (m_crosshairBorderColor->isDirty()) {
        Colors::set(Colors::CrossHairBorder, *m_crosshairBorderColor);
        m_crosshairBorderColor->clearDirty();
    }
    if (m_crosshairHighlightColor->isDirty()) {
        Colors::set(Colors::CrossHairHighlight, *m_crosshairHighlightColor);
        m_crosshairHighlightColor->clearDirty();
    }
    if (m_crosshairOpacity->isDirty()) {
        Colors::set(Colors::CrossHairOpacity, Colors::opacityFromPercent(*m_crosshairOpacity));
        m_crosshairOpacity->clearDirty();
    }
    if (m_lineWidth->isDirty()) {
        Dimensions::setLineWidth(*m_lineWidth);
        m_lineWidth->clearDirty();
    }
}

bool ToolPrefsModel::isDirty() const {
    return m_lineForeColor->isDirty()
           || m_crosshairBackColor->isDirty()
           || m_crosshairBorderColor->isDirty()
           || m_crosshairHighlightColor->isDirty()
           || m_crosshairOpacity->isDirty()
        || m_lineWidth->isDirty();
}
