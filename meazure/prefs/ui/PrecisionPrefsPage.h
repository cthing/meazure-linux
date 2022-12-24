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

#include "PrefsPage.h"
#include "PrefsPageId.h"
#include <meazure/prefs/models/PrecisionPrefsModel.h>


/// User interface for measurement units display precisions. Each measurement unit has associated with it a set of
/// display precisions, one for each measurement type (e.g. width, height, x, y).
///
class PrecisionPrefsPage : public PrefsPage {

public:
    PrecisionPrefsPage();

    PrefsPageId getId() override {
        return PrefsPageId::PrecisionPage;
    }

    QString getName() override {
        return tr("Precision");
    }

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

private:
    PrecisionPrefsModel* m_model;
};
