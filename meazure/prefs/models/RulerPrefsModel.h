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

#pragma once

#include "Preference.h"
#include <QObject>
#include <QColor>


/// Ruler preferences model.
///
class RulerPrefsModel : public QObject {

    Q_OBJECT

public:
    explicit RulerPrefsModel(QObject* parent);

    void initialize();

    void apply() const;

    [[nodiscard]] bool isDirty() const;

    Preference<QRgb>* m_rulerBackColor;         // NOLINT(misc-non-private-member-variables-in-classes)
    Preference<QRgb>* m_rulerBorderColor;       // NOLINT(misc-non-private-member-variables-in-classes)
    Preference<int>* m_rulerOpacity;            // NOLINT(misc-non-private-member-variables-in-classes)

signals:
    void dirtyChanged(bool dirty);
};
