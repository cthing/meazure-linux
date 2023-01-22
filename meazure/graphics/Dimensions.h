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

#include <meazure/config/Config.h>
#include <QObject>


/// Configurable common dimensions used by various graphical elements.
///
namespace Dimensions {

    class ChangeNotifier : public QObject {
        Q_OBJECT
    signals:
        void lineWidthChanged(int width);
    };


    /// Obtains the notifier that will emit a signal when a dimension changes.
    ///
    /// @return Notifier that emits a signal when a dimension changes.
    ///
    ChangeNotifier* getChangeNotifier();

    /// Resets all dimensions to their default values.
    ///
    void reset();

    /// Persists the dimensions to the specified configuration.
    ///
    /// @param[in] config Configuration into which the dimensions are persisted.
    ///
    void writeConfig(Config& config);

    /// Restores the dimensions from the specified configuration.
    ///
    /// @param[in] config Configuration from which the dimensions are restored.
    ///
    void readConfig(const Config& config);

    /// Sets the width for lines used by tools and the grid.
    ///
    /// @param[in] width Width for lines, in pixels
    ///
    void setLineWidth(int width);

    /// Obtains the width for lines used by tools and the grid.
    ///
    /// @return Width for lines, in pixels.
    ///
    int getLineWidth();

    /// Obtains the default with for lines used by tools and the grid.
    ///
    /// @return Default width for lines, in pixels.
    ///
    int getDefaultLineWidth();
}
