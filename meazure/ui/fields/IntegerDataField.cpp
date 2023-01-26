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

#include "IntegerDataField.h"


IntegerDataField::IntegerDataField(int charWidth, bool showButtons, bool readOnly, bool nativeStepHandling,
                                   QWidget *parent) :
        AbstractDataField(charWidth, showButtons, readOnly, nativeStepHandling, parent) {
}

void IntegerDataField::emitSteps(int numSteps) {
    emit stepRequested(numSteps);
}
