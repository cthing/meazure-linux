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
#include <meazure/graphics/Crosshair.h>
#include <meazure/graphics/Line.h>
#include <meazure/graphics/Circle.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <meazure/config/Config.h>
#include <QObject>
#include <QPoint>
#include <QPointF>


/// Circle measurement tool. This tool provides a circle and its radius. Two crosshairs are provided: one at the center
/// of the circle (vertex) and one where the radius intersects the perimeter (point 1). Each crosshair can be
/// arbitrarily positioned, which will result in moving and resizing the circle.
///
class CircleTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName {"CircleTool" };

    explicit CircleTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~CircleTool() override;

    CircleTool(const CircleTool&) = delete;
    CircleTool(CircleTool&&) = delete;
    CircleTool& operator=(const CircleTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Ctrl moves circle, Ctrl+R captures region");
    }

    [[nodiscard]] QImage grabRegion() const override;

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
    void setXYVPosition(const QPointF& position) override;
    void setXVPosition(double x) override;
    void setYVPosition(double y) override;

    void stepX1Position(int numSteps) override;
    void stepY1Position(int numSteps) override;
    void stepXVPosition(int numSteps) override;
    void stepYVPosition(int numSteps) override;

signals:
    void activePositionChanged(QPoint rawPos);
    void xy1PositionChanged(QPointF coord, QPoint rawPos);
    void xyvPositionChanged(QPointF coord, QPoint rawPos);
    void widthHeightChanged(QSizeF widthHeight);
    void distanceChanged(double distance);
    void angleChanged(double angle);
    void aspectChanged(double aspect);
    void areaChanged(double area);

private slots:
    void entered(Crosshair& crosshair, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers);
    void departed(Crosshair& crosshair, int id);
    void dragged(Crosshair& crosshair, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers);
    void moved(Crosshair& crosshair, int id, QPoint crosshairCenter);

private:
    /// Similar to k_crosshairOffset but for use with the perimeter crosshair to provide a bit more spacing.
    ///
    static constexpr double k_crosshairRadialOffset {Crosshair::getDefaultSize() / 2.0 + 0.08 };
    static constexpr RadioToolTraits k_traits { XY1ReadWrite | XYVReadWrite | WHReadOnly | DistReadOnly |
                                                AngleReadOnly | AspectReadOnly | AreaReadOnly };
    static constexpr int k_perimeterId { 1 };   ///< ID for the perimeter crosshair
    static constexpr int k_centerId { 2 };      ///< ID for the center crosshair

    void setPosition();

    void emitActivePosition();


    QPoint m_center;                         ///< Location of the center of the circle
    QPoint m_perimeter;                      ///< A point on the perimeter of the circle
    int m_activePointId { k_perimeterId };   ///< ID of point being changed
    Crosshair* m_centerCH;                   ///< Crosshair for the center point
    Crosshair* m_perimeterCH;                ///< Crosshair for the perimeter point
    Circle* m_circle;                        ///< Circle graphic object
    Line* m_line;                            ///< Radial line
    ToolDataWindow* m_dataWinCenter;         ///< Data window tooltip for the center point
    ToolDataWindow* m_dataWinPerimeter;      ///< Data window tooltip for the perimeter point
};
