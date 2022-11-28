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
#include <meazure/graphics/CrossHair.h>
#include <meazure/graphics/Line.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <QObject>
#include <QPoint>


/// Line measurement tool. This tool provides a line to measure the distance between two arbitrary points on the
/// screen. Two crosshairs are provided one at the either end of the line (points 1 and 2). Each crosshair can be
/// arbitrarily positioned, which results in moving the line connecting the crosshairs.
///
class LineTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "LineTool" };

    explicit LineTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                      QObject* parent = nullptr);

    ~LineTool() override;

    LineTool(const LineTool&) = delete;
    LineTool(LineTool&&) = delete;
    LineTool& operator=(const LineTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Shift locks to H or V, Ctrl moves line");
    }

    void setEnabled(bool enable) override;

    void saveProfile(Profile& profile) const override;
    void loadProfile(Profile& profile) override;

    void refresh() override;

    void flash() override;
    void strobe() override;

    void setX1Position(double x) override;
    void setY1Position(double y) override;
    void setX2Position(double x) override;
    void setY2Position(double y) override;

    void stepX1Position(int numSteps) override;
    void stepY1Position(int numSteps) override;
    void stepX2Position(int numSteps) override;
    void stepY2Position(int numSteps) override;

signals:
    void xy1PositionChanged(QPointF coord);
    void xy2PositionChanged(QPointF coord);
    void widthHeightChanged(QSizeF widthHeight);
    void distanceChanged(double distance);
    void angleChanged(double angle);
    void aspectChanged(double aspect);
    void areaChanged(double area);

private slots:
    void entered(CrossHair& crosshair, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers);
    void departed(CrossHair& crosshair, int id);
    void dragged(CrossHair& crosshair, int id, QPoint crosshairCenter, Qt::KeyboardModifiers keyboardModifiers);
    void moved(CrossHair& crosshair, int id, QPoint crosshairCenter);

private:
    static constexpr RadioToolTraits k_traits { XY1ReadWrite | XY2ReadWrite | WHReadOnly | DistReadOnly |
                                                AngleReadOnly | AspectReadOnly | AreaReadOnly };
    static constexpr int k_point1Id { 1 };      ///< ID for the point 1 crosshair
    static constexpr int k_point2Id { 2 };      ///< ID for the point 2 crosshair

    void setPosition();


    QPoint m_point1;             ///< Location of one end point of the line
    QPoint m_point2;             ///< Location of one end point of the line
    CrossHair* m_point1CH;       ///< Crosshair for point 1
    CrossHair* m_point2CH;       ///< Crosshair for point 2
    Line* m_line;                ///< Line connecting point 1 and point 2
    ToolDataWindow* m_dataWin1;  ///< Data window tooltip for point 1
    ToolDataWindow* m_dataWin2;  ///< Data window tooltip for point 2
};
