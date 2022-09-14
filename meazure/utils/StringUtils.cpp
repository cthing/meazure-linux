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

#include "StringUtils.h"
#include <cfloat>


QString StringUtils::dblToStr(double value) {
    QString numStr = QString::number(value, 'f', DBL_DIG - 1);

    qsizetype idx = numStr.size();
    while (idx-- > 1) {
        if ((numStr[idx] != '0') || (numStr[idx - 1] == '.')) {
            break;
        }
    }

    return (idx == numStr.size()) ? numStr : numStr.left(idx + 1);
}
