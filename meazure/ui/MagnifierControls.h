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

#include "Magnifier.h"
#include <meazure/environment/ScreenInfoProvider.h>
#include <QWidget>
#include <QAction>


/// Provides the zoom and freeze controls for the magnifier.
///
class MagnifierControls : public QWidget {

    Q_OBJECT

public:
    explicit MagnifierControls(Magnifier* magnifier, const ScreenInfoProvider* screenInfo);

    [[nodiscard]] QAction* getFreezeAction() const {
        return m_freezeAction;
    }

private:
    static constexpr int k_freezeButtonSize { 20 };
    static constexpr int k_freezeButtonIconSize { k_freezeButtonSize - 6 };

    QAction* m_freezeAction;
};
