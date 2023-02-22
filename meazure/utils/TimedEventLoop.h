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

#include <QEventLoop>


/// Provides a timed delay that processes events.
///
class TimedEventLoop : public QEventLoop {

public:
    /// Blocks for the specified amount of time but continues to process Qt events.
    ///
    /// @param milliseconds Delay in milliseconds.
    ///
    explicit TimedEventLoop(int milliseconds);
};
