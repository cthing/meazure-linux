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

#include "RadioTool.h"
#include "RadioToolTraits.h"
#include <meazure/environment/CursorTracker.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <QObject>
#include <QString>
#include <QPoint>


/// Cursor position measurement tool. This tool measures the position of the mouse pointer.
///
class CursorTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "CursorTool" };

    explicit CursorTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~CursorTool() override;

    CursorTool(const CursorTool&) = delete;
    CursorTool(CursorTool&&) = delete;
    CursorTool& operator=(const CursorTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Cursor hotspot is measurement point");
    }

    void setEnabled(bool enable) override;

    void setDataWinEnabled(bool enable) override;

    void refresh() override;
    void strobe() override;

    [[nodiscard]] bool hasCrosshairs() const override;

signals:
    void activePositionChanged(QPoint rawPos);
    void xy1PositionChanged(QPointF coord, QPoint rawPos);

private slots:
    void cursorMotion(QPoint pos);

private:
    static constexpr RadioToolTraits k_traits { XY1ReadOnly };

    void placeDataWin(const QPoint& position);

    void emitMeasurement(QPoint position);

    CursorTracker* m_pointerTracker { new CursorTracker(this) };
    ToolDataWindow* m_dataWindow;
};
