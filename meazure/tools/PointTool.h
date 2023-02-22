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
#include <meazure/graphics/Crosshair.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <meazure/config/Config.h>
#include <QObject>
#include <QPoint>
#include <QPointF>


/// Point measurement tool. This tool provides a crosshair that can be placed arbitrarily on the screen to measure
/// a location.
///
class PointTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName {"PointTool" };

    explicit PointTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~PointTool() override;

    PointTool(const PointTool&) = delete;
    PointTool(PointTool&&) = delete;
    PointTool& operator=(const PointTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Shift locks to H or V");
    }

    void setEnabled(bool enable) override;

    void setCrosshairsEnabled(bool enable) override;
    void setDataWinEnabled(bool enable) override;

    void writeConfig(Config& config) const override;
    void readConfig(const Config& config) override;

    void refresh() override;

    void flash() override;
    void strobe() override;

    void setXY1Position(const QPointF& position) override;
    void setX1Position(double x) override;
    void setY1Position(double y) override;

    void stepX1Position(int numSteps) override;
    void stepY1Position(int numSteps) override;

signals:
    void activePositionChanged(QPoint rawPos);
    void xy1PositionChanged(QPointF coord, QPoint rawPos);

private slots:
    void entered(Crosshair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void departed(Crosshair& crosshair, int id);
    void dragged(Crosshair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void moved(Crosshair& crosshair, int id, QPoint center);

private:
    static constexpr RadioToolTraits k_traits { XY1ReadWrite };

    void setPosition();

    QPoint m_center;            ///< Position of the crosshair
    QPoint m_anchorPoint;       ///< Location for vertical / horizontal lock when Shift is held while dragging
    Crosshair* m_crosshair;
    ToolDataWindow* m_dataWindow;
};
