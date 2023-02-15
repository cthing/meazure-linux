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

#include <meazure/environment/WindowTracker.h>
#include <QObject>
#include <cstdint>


/// A window tracker that does not do anything for use on platforms that do not support this capability (e.g. Wayland).
///
class NoopWindowTracker : public QObject, public WindowTracker {

    Q_OBJECT
    Q_INTERFACES(WindowTracker)

public:
    explicit NoopWindowTracker(QObject* parent = nullptr);
    ~NoopWindowTracker() override = default;

    NoopWindowTracker(const NoopWindowTracker&) = delete;
    NoopWindowTracker(NoopWindowTracker&&) = delete;
    NoopWindowTracker& operator=(const NoopWindowTracker&) = delete;

    [[nodiscard]] bool isSupported() const override;

    void start() override;
    void stop() override;

signals:
    void windowChanged(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height) override;
};
