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

#include <QObject>


/// Tracks the position of the cursor regardless of whether the application has focus. This is accomplished by
/// using a timer to periodically read the cursor location.
///
class CursorTracker : public QObject {

    Q_OBJECT

public:
    explicit CursorTracker(QObject* parent = nullptr);

    /// Starts tracking cursor motion and emitting motion signals.
    ///
    void start();

    /// Stops tracking cursor motion.
    ///
    void stop();

signals:
    /// Emitted when the cursor is moved.
    ///
    /// @param pos Location of the cursor, in pixels
    ///
    void motion(QPoint pos);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    static constexpr int k_updateRate { 30 };       // Milliseconds

    int m_timerId { 0 };
};
