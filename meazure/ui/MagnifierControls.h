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

#include "Magnifier.h"
#include <QWidget>
#include <QAction>


/// Provides the zoom and freeze controls for the magnifier.
///
class MagnifierControls : public QWidget {

    Q_OBJECT

public:
    explicit MagnifierControls(Magnifier* magnifier);

    [[nodiscard]] QAction* getFreezeAction() const {
        return m_freezeAction;
    }

private:
    static constexpr int k_freezeButtonSize { 20 };
    static constexpr int k_freezeButtonIconSize { k_freezeButtonSize - 6 };

    QAction* m_freezeAction;
};
