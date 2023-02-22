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

#include <meazure/environment/WindowFinder.h>
#include <QObject>
#include <QPoint>
#include <QRect>
#include <vector>


class Finder;


/// Finds the geometry of X11 top level client windows on all screens. Windows that are input-only, minimized or are
/// Meazure graphic elements are not included.
///
class X11WindowFinder : public WindowFinder {

public:
    X11WindowFinder();
    ~X11WindowFinder() override;

    X11WindowFinder(const X11WindowFinder&) = delete;
    X11WindowFinder(X11WindowFinder&&) = delete;
    X11WindowFinder& operator=(const X11WindowFinder&) = delete;

    [[nodiscard]] bool isSupported() const override;

    void refresh() override;

    QRect find(const QPoint& position) override;

private:
    Finder* m_updater;
    bool m_firstUpdate { false };
    std::vector<QRect> m_windows;
};
