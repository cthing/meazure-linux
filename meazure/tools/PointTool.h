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
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <QObject>
#include <QPoint>


/// Point measurement tool. This tool provides a crosshair that can be placed arbitrarily on the screen to measure
/// a location.
///
class PointTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName {"PointTool" };

    explicit PointTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                       QObject* parent = nullptr);

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

    void saveProfile(Profile& profile) const override;
    void loadProfile(Profile& profile) override;

    void refresh() override;

    void flash() override;
    void strobe() override;

    void setX1Position(double x) override;
    void setY1Position(double y) override;

    void stepX1Position(int numSteps) override;
    void stepY1Position(int numSteps) override;

signals:
    void xy1PositionChanged(QPointF coord, QPoint rawPos);

private slots:
    void entered(CrossHair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void departed(CrossHair& crosshair, int id);
    void dragged(CrossHair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void moved(CrossHair& crosshair, int id, QPoint center);

private:
    static constexpr RadioToolTraits k_traits { XY1ReadWrite };

    void setPosition();

    QPoint m_center;            ///< Position of the crosshair
    QPoint m_anchorPoint;       ///< Location for vertical / horizontal lock when Shift is held while dragging
    CrossHair* m_crosshair;
    ToolDataWindow* m_dataWindow;
};
