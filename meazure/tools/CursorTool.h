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

#include "RadioTool.h"
#include "RadioToolTraits.h"
#include <QObject>
#include <QString>


/// Cursor position measurement tool. This tool measures the position of the mouse pointer.
///
class CursorTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* toolName { "CursorTool" };

    explicit CursorTool(QObject* parent = nullptr);

    [[nodiscard]] const char* getName() const override {
        return toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Cursor hotspot is measurement point");
    }

    void setEnabled(bool enable) override;

private:
    static constexpr RadioToolTraits traits { XY1ReadOnly };
};
