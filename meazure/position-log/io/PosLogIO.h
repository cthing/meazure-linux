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
    static constexpr const char* k_dtdUrlOld { "http://www.cthing.com/dtd/PositionLog1.dtd" };

    // Element names
    static constexpr const char* k_angleElem { "angle" };
    static constexpr const char* k_areaElem { "area" };
    static constexpr const char* k_createdElem { "created" };
    static constexpr const char* k_customUnitsElem { "customUnits" };
    static constexpr const char* k_descElem { "desc" };
    static constexpr const char* k_desktopElem { "desktop" };
    static constexpr const char* k_desktopsElem { "desktops" };
    static constexpr const char* k_displayPrecisionElem { "displayPrecision" };
    static constexpr const char* k_displayPrecisionsElem { "displayPrecisions" };
    static constexpr const char* k_distanceElem { "distance" };
    static constexpr const char* k_generatorElem { "generator" };
    static constexpr const char* k_heightElem { "height" };
    static constexpr const char* k_infoElem { "info" };
    static constexpr const char* k_machineElem { "machine" };
    static constexpr const char* k_measurementElem { "measurement" };
    static constexpr const char* k_originElem { "origin" };
    static constexpr const char* k_pointElem { "point" };
    static constexpr const char* k_pointsElem { "points" };
    static constexpr const char* k_positionElem { "position" };
    static constexpr const char* k_positionLogElem { "positionLog"};
    static constexpr const char* k_positionsElem { "positions" };
    static constexpr const char* k_propertiesElem { "properties" };
    static constexpr const char* k_rectElem { "rect" };
    static constexpr const char* k_resolutionElem { "resolution" };
    static constexpr const char* k_screenElem { "screen" };
    static constexpr const char* k_screensElem { "screens" };
    static constexpr const char* k_sizeElem { "size" };
    static constexpr const char* k_titleElem { "title" };
    static constexpr const char* k_unitsElem { "units" };
    static constexpr const char* k_widthElem { "width" };

    // Attribute names
    static constexpr const char* k_abbrevAttr { "abbrev" };
    static constexpr const char* k_angleAttr { "angle" };
    static constexpr const char* k_bottomAttr { "bottom" };
    static constexpr const char* k_buildAttr { "build" };
    static constexpr const char* k_dateAttr { "date" };
    static constexpr const char* k_decimalPlacesAttr { "decimalPlaces" };
    static constexpr const char* k_descAttr { "desc" };
    static constexpr const char* k_desktopRefAttr { "desktopRef" };
    static constexpr const char* k_idAttr { "id" };
    static constexpr const char* k_invertYAttr { "invertY" };
    static constexpr const char* k_leftAttr { "left" };
    static constexpr const char* k_lengthAttr { "length" };
    static constexpr const char* k_manualAttr { "manual" };
    static constexpr const char* k_nameAttr { "name" };
    static constexpr const char* k_oneAttr { "1" };
    static constexpr const char* k_primaryAttr { "primary" };
    static constexpr const char* k_rightAttr { "right" };
    static constexpr const char* k_scaleBasisAttr { "scaleBasis" };
    static constexpr const char* k_scaleFactorAttr { "scaleFactor" };
    static constexpr const char* k_toolAttr { "tool" };
    static constexpr const char* k_topAttr { "top" };
    static constexpr const char* k_twoAttr { "2" };
    static constexpr const char* k_unitsAttr { "units" };
    static constexpr const char* k_vAttr { "v" };
    static constexpr const char* k_valueAttr { "value" };
    static constexpr const char* k_versionAttr { "version" };
    static constexpr const char* k_xAttr { "x" };
    static constexpr const char* k_xoffsetAttr { "xoffset" };
    static constexpr const char* k_yAttr { "y" };
    static constexpr const char* k_yoffsetAttr { "yoffset" };

    explicit PosLogIO(const UnitsProvider& unitsProvider) : m_units(unitsProvider) {
    }

    const UnitsProvider& m_units;
};
