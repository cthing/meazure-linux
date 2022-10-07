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

#include <QThread>
#include <cstdint>


/// Tracks the position of the pointer regardless of whether the application has focus. This is accomplished using the
/// XRecord extension to capture motion notify events. The code is based on examples at
/// https://github.com/nibrahim/showkeys/blob/master/tests/record-example.c and
/// https://github.com/KivApple/qvkbd/blob/master/src/x11support.cpp.
///
class PointerTracker : public QThread {

    Q_OBJECT

public:
    explicit PointerTracker(QObject* parent = nullptr);
    ~PointerTracker() override;

    PointerTracker(const PointerTracker&) = delete;
    PointerTracker(PointerTracker&&) = delete;
    PointerTracker& operator=(const PointerTracker&) = delete;

    /// Starts tracking mouse motion and emitting motion signals.
    ///
    void start();

    /// Stops tracking mouse motion.
    ///
    void stop();

signals:
    void error(const char *message);
    void motion(int16_t x, int16_t y);

protected:
    void run() override;

private:
    void handleMotion(int16_t x, int16_t y);

    volatile bool m_run { false };
};
