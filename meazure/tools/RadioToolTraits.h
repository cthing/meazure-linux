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

#include <QFlags>


/// Flags to indicate the measurements provided by a radio tool and whether a given measurement can be set or just
/// displayed. Flags for each type of measurement are OR'd together to describe the set of measurements provided by
/// a radio tool. For example, XY1ReadWrite | XY2ReadWrite | WHReadOnly indicates that a radio tool provides
/// measurement data for X1, Y1, X2, Y2, Width and Height and that the Width and Height as not settable. The *Available
/// flags are a convenience to indicate whether a given measurement is provided by a radio tool regardless of whether
/// it is settable or read only.
///
enum RadioToolTrait {
    XY1ReadOnly        = 0x00000001,
    XY1ReadWrite       = 0x00000002,
    XY1Available       = XY1ReadOnly | XY1ReadWrite,

    XY2ReadOnly        = 0x00000004,
    XY2ReadWrite       = 0x00000008,
    XY2Available       = XY2ReadOnly | XY2ReadWrite,

    XYVReadOnly        = 0x00000010,
    XYVReadWrite       = 0x00000020,
    XYVAvailable       = XYVReadOnly | XYVReadWrite,

    WHReadOnly         = 0x00000040,
    WHAvailable        = WHReadOnly,

    DistReadOnly       = 0x00000080,
    DistAvailable      = DistReadOnly,

    AngleReadOnly      = 0x00000100,
    AngleAvailable     = AngleReadOnly,

    AreaReadOnly       = 0x00000200,
    AreaAvailable      = AreaReadOnly,

    AspectReadOnly     = 0x00000400,
    AspectAvailable    = AspectReadOnly,
};

Q_DECLARE_FLAGS(RadioToolTraits, RadioToolTrait)
Q_DECLARE_OPERATORS_FOR_FLAGS(RadioToolTraits)
