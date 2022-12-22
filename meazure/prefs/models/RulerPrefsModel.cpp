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

#include "RulerPrefsModel.h"
#include <meazure/graphics/Colors.h>


RulerPrefsModel::RulerPrefsModel(QObject* parent) :
        QObject(parent),
        m_rulerBackColor(new Preference(Colors::getDefault(Colors::RulerBack), this)),
        m_rulerBorderColor(new Preference(Colors::getDefault(Colors::RulerBorder), this)),
        m_rulerOpacity(new Preference(Colors::opacityToPercent(Colors::getDefault(Colors::RulerOpacity)), this)) {
    connect(m_rulerBackColor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_rulerBorderColor, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
    connect(m_rulerOpacity, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));
}

void RulerPrefsModel::initialize() {     // NOLINT(readability-make-member-function-const)
    m_rulerBackColor->initialize(Colors::get(Colors::RulerBack));
    m_rulerBorderColor->initialize(Colors::get(Colors::RulerBorder));
    m_rulerOpacity->initialize(Colors::opacityToPercent(Colors::get(Colors::RulerOpacity)));
}

void RulerPrefsModel::apply() const {
    if (m_rulerBackColor->isDirty()) {
        Colors::set(Colors::RulerBack, *m_rulerBackColor);
        m_rulerBackColor->clearDirty();
    }
    if (m_rulerBorderColor->isDirty()) {
        Colors::set(Colors::RulerBorder, *m_rulerBorderColor);
        m_rulerBorderColor->clearDirty();
    }
    if (m_rulerOpacity->isDirty()) {
        Colors::set(Colors::RulerOpacity, Colors::opacityFromPercent(*m_rulerOpacity));
        m_rulerOpacity->clearDirty();
    }
}

bool RulerPrefsModel::isDirty() const { // NOLINT(readability-convert-member-functions-to-static)
    return m_rulerBackColor->isDirty()
        || m_rulerBorderColor->isDirty()
        || m_rulerOpacity->isDirty();
}
