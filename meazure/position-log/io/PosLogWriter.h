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

#include "PosLogIO.h"
#include <meazure/position-log/model/PosLogArchive.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/xml/XMLWriter.h>
#include <iostream>


/// Writes the position log as an XML file.
///
class PosLogWriter : public PosLogIO {

public:
    explicit PosLogWriter(const UnitsProvider* unitsProvider);

    void write(std::ostream& out, const PosLogArchive& archive);

private:
    static void writeInfoSection(XMLWriter& writer, const PosLogArchive& archive);
    void writeDesktopsSection(XMLWriter& writer, const PosLogArchive& archive);
    static void writePositionsSection(XMLWriter& writer, const PosLogArchive& archive);
};
