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
#include <meazure/graphics/Line.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <meazure/config/Config.h>
#include <QObject>
#include <QPoint>
#include <QPointF>


/// Angle measurement tool. This tool measures the acute or obtuse angle between two intersecting lines. There are
/// three crosshairs associated with the tool: one at the intersection of the two lines (vertex), and one at the end
/// of each line (point 1 and point 2). Each crosshair can be arbitrarily positioned. An angle bisector is displayed
/// to indicate on which side of the intersection the angle is being measured.
///
class AngleTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName {"AngleTool" };

    explicit AngleTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~AngleTool() override;

    AngleTool(const AngleTool&) = delete;
    AngleTool(AngleTool&&) = delete;
    AngleTool& operator=(const AngleTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Shift locks to H or V, Ctrl moves angle");
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
    void setXY2Position(const QPointF& position) override;
    void setX2Position(double x) override;
    void setY2Position(double y) override;
    void setXYVPosition(const QPointF& position) override;
    void setXVPosition(double x) override;
    void setYVPosition(double y) override;

    void stepX1Position(int numSteps) override;
    void stepY1Position(int numSteps) override;
    void stepX2Position(int numSteps) override;
    void stepY2Position(int numSteps) override;
    void stepXVPosition(int numSteps) override;
    void stepYVPosition(int numSteps) override;

signals:
    void activePositionChanged(QPoint rawPos);
    void xy1PositionChanged(QPointF coord, QPoint rawPos);
    void xy2PositionChanged(QPointF coord, QPoint rawPos);
    void xyvPositionChanged(QPointF coord, QPoint rawPos);
    void angleChanged(double angle);

private slots:
    void entered(Crosshair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void departed(Crosshair& crosshair, int id);
    void dragged(Crosshair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void moved(Crosshair& crosshair, int id, QPoint center);

private:
    static constexpr RadioToolTraits k_traits { XY1ReadWrite | XY2ReadWrite | XYVReadWrite | AngleReadOnly };
    static constexpr double k_lengthB { 0.75 };     ///< Length of the angle bisector line in inches
    static constexpr double k_minLengthB { 65 };    ///< Minimum length of the angle bisector line in pixels
    static constexpr int k_point1Id { 1 };          ///< ID for the point 1 crosshair
    static constexpr int k_point2Id { 2 };          ///< ID for the point 2 crosshair
    static constexpr int k_vertexId { 3 };          ///< ID for the vertex crosshair

    void setPosition();
    void setBisectorPosition();

    void emitActivePosition();


    QPoint m_vertex;                     ///< Location of the intersection of line 1 and line 2
    QPoint m_point1;                     ///< Location of the line 1 end point
    QPoint m_point2;                     ///< Location of the line 2 end point
    QPoint m_vertexAnchor;               ///< Location for vertical / horizontal lock when Shift is held while dragging
    int m_activePointId { k_point1Id};   ///< ID of point being changed
    Crosshair* m_vertexCH;               ///< Crosshair for the line 1 / line 2 intersection point
    Crosshair* m_point1CH;               ///< Crosshair for the line 1 end point
    Crosshair* m_point2CH;               ///< Crosshair for the line 2 end point
    Line* m_lineB;                       ///< Line forming the angle bisector
    Line* m_line1;                       ///< Line from vertex to point 1
    Line* m_line2;                       ///< Line from vertex to point 2
    ToolDataWindow* m_dataWinV;          ///< Data window tooltip for the vertex
    ToolDataWindow* m_dataWin1;          ///< Data window tooltip for point 1
    ToolDataWindow* m_dataWin2;          ///< Data window tooltip for point 2
};
