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
#include <QPoint>
#include <QRect>


/// A window finder that does not do anything for use on platforms that do not support this capability (e.g. Wayland).
///
class NoopWindowFinder : public WindowFinder {

public:
    NoopWindowFinder() = default;
    ~NoopWindowFinder() override = default;

    NoopWindowFinder(const NoopWindowFinder&) = delete;
    NoopWindowFinder(NoopWindowFinder&&) = delete;
    NoopWindowFinder& operator=(const NoopWindowFinder&) = delete;

    [[nodiscard]] bool isSupported() const override;

    void refresh() override;

    QRect find(const QPoint& position) override;
};
