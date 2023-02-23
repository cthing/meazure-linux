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


/// X11 implementation of the Meazure graphic window identifier tag.
///
namespace X11GraphicTag {

    /// Indicates there the specified window is a Meazure graphic window.
    ///
    /// @param[in] windowId Native window identifier
    /// @return true if the specified window is a Meazure graphic window.
    ///
    [[nodiscard]] bool isGraphicWindow(unsigned long windowId);

    /// Checks for and processes window identifier change events.
    ///
    /// @param[in] target Widget whose window events are to be processed
    /// @param[in] event Event to process
    ///
    void processEvents(const QWidget* target, const QEvent* event);
};
