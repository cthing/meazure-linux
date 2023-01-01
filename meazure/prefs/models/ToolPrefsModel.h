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


/// Tool preferences model.
///
class ToolPrefsModel : public QObject {

    Q_OBJECT

public:
    explicit ToolPrefsModel(QObject* parent);

    void initialize();

    void apply() const;

    [[nodiscard]] bool isDirty() const;

    Preference<QRgb>* m_lineForeColor;
    Preference<QRgb>* m_crosshairBackColor;
    Preference<QRgb>* m_crosshairBorderColor;
    Preference<QRgb>* m_crosshairHighlightColor;
    Preference<int>* m_crosshairOpacity;
    Preference<int>* m_lineWidth;

signals:
    void dirtyChanged(bool dirty);
};
