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


/// Supervises the saving and loading of application profiles. The profile represents the state of the application
/// (e.g. currently selected tools, color preferences, etc.) so that it can be saved and restored.
///
class ProfileMgr : public QObject {

    Q_OBJECT

public slots:
    void saveProfile();
    void loadProfile();

    void saveSettings();
    void loadSettings();
};
