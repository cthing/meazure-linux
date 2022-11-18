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

#include <meazure/profile/Profile.h>


/// Configurable common dimensions used by various graphical elements.
///
namespace Dimensions {

    /// Resets all dimensions to their default values.
    ///
    void reset();

    /// Persists the dimensions to the specified profile.
    ///
    /// @param[in] profile Profile into which the dimensions are persisted.
    ///
    void saveProfile(Profile& profile);

    /// Restores the dimensions from the specified profile.
    ///
    /// @param[in] profile Profile from which the dimensions are restored.
    ///
    void loadProfile(Profile& profile);

    /// Obtains the width for lines used by tools and the grid.
    ///
    /// @return Width for lines, in pixels.
    ///
    int getLineWidth();

    /// Sets the width for lines used by tools and the grid.
    ///
    /// @param[in] width Width for lines, in pixels
    ///
    void setLineWidth(int width);
}