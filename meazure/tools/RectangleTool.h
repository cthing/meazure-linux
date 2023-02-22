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
#include <meazure/graphics/Rectangle.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <meazure/config/Config.h>
#include <QObject>
#include <QPoint>
#include <QPointF>


/// Rectangle measurement tool. This tool provides a rectangle to measure an arbitrary rectangular area of the screen.
/// Two crosshairs are provided one at the either end of the rectangle's hypot (points 1 and 2). Each crosshair
/// can be arbitrarily positioned, which results in moving the rectangle.
///
class RectangleTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName {"RectTool" };

    explicit RectangleTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~RectangleTool() override;

    RectangleTool(const RectangleTool&) = delete;
    RectangleTool(RectangleTool&&) = delete;
    RectangleTool& operator=(const RectangleTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Ctrl moves rect, Ctrl+R captures region");
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
    void setXY2Position(const QPointF& position) override;
    void setX2Position(double x) override;
    void setY2Position(double y) override;

    void stepX1Position(int numSteps) override;
    void stepY1Position(int numSteps) override;
    void stepX2Position(int numSteps) override;
    void stepY2Position(int numSteps) override;

signals:
    void activePositionChanged(QPoint rawPos);
    void xy1PositionChanged(QPointF coord, QPoint rawPos);
    void xy2PositionChanged(QPointF coord, QPoint rawPos);
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
    static constexpr RadioToolTraits k_traits { XY1ReadWrite | XY2ReadWrite | WHReadOnly | DistReadOnly |
                                                AngleReadOnly | AspectReadOnly | AreaReadOnly };
    static constexpr int k_point1Id { 1 };      ///< ID for the point 1 crosshair
    static constexpr int k_point2Id { 2 };      ///< ID for the point 2 crosshair

    void setPosition();

    void emitActivePosition();


    QPoint m_point1;                     ///< Location of one end point of the line
    QPoint m_point2;                     ///< Location of one end point of the line
    int m_activePointId { k_point1Id };  ///< ID of point being changed
    QPoint m_anchorPoint1;               ///< Location for vertical / horizontal Shift lock while dragging point 1 crosshair
    QPoint m_anchorPoint2;               ///< Location for vertical / horizontal Shift lock while dragging point 2 crosshair
    Crosshair* m_point1CH;               ///< Crosshair for point 1
    Crosshair* m_point2CH;               ///< Crosshair for point 2
    Rectangle* m_rectangle;              ///< Rectangle from point 1 in the top left to point 2 in the bottom right
    ToolDataWindow* m_dataWin1;          ///< Data window tooltip for point 1
    ToolDataWindow* m_dataWin2;          ///< Data window tooltip for point 2
};
