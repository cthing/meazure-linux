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

#include "ui/MainWindow.h"
#include <QApplication>


/// Represents the application.
///
class App : public QApplication {

public:
    /// Constructs the application.
    ///
    /// @param[in] argc Number of command line arguments
    /// @param[in] argv Command line arguments
    ///
    App(int& argc, char** argv);

    ~App() override;

    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;

private:
    MainWindow* m_mainWindow { new MainWindow() };
};