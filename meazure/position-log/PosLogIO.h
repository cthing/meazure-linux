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

#include <meazure/units/UnitsProvider.h>


class PosLogIO {

protected:
    static constexpr const char* k_dtdUrl { "https://www.cthing.com/dtd/PositionLog1.dtd" };
    static constexpr int k_majorVersion { 1 };

    explicit PosLogIO(const UnitsProvider& unitsProvider) : m_units(unitsProvider) {
    }

    const UnitsProvider& m_units;
};
