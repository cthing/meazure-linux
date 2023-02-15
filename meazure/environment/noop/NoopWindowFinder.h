/*
 * Copyright 2023 C Thing Software
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
