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

#pragma once

#include <QWidget>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>


/// Base class for all graphic elements. Classes derived from this base class are used by the measurement tools
/// to perform their function.
///
class Graphic : public QWidget {

    Q_OBJECT

public:
    explicit Graphic(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QWidget* parent);

    static bool isGraphicWindow(unsigned long windowId);

    bool event(QEvent* ev) override;

protected:
    const ScreenInfo* m_screenInfo;
    const UnitsProvider* m_unitsProvider;
};
