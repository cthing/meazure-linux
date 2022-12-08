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
#include <meazure/environment/WindowFinder.h>
#include <meazure/environment/PointerTracker.h>
#include <meazure/environment/WindowTracker.h>
#include <meazure/graphics/Rectangle.h>
#include <meazure/ui/ToolDataWindow.h>


/// Window measurement tool. This tool tracks the mouse pointer and attempts to identify a top level window under the
/// pointer.
///
class WindowTool : public RadioTool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "WindowTool" };

    explicit WindowTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                        QObject* parent = nullptr);

    ~WindowTool() override;

    WindowTool(const WindowTool&) = delete;
    WindowTool(WindowTool&&) = delete;
    WindowTool& operator=(const WindowTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    [[nodiscard]] RadioToolTraits getTraits() const override {
        return k_traits;
    }

    [[nodiscard]] QString getInstructions() const override {
        return tr("Ctrl+R captures window");
    }

    void setEnabled(bool enable) override;

    void refresh() override;
    void strobe() override;

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
    void pointerMotion(int16_t x, int16_t y);
    void windowChanged(unsigned long windowId, int16_t x, int16_t y, uint16_t width, uint16_t height);

private:
    static constexpr RadioToolTraits k_traits { XY1ReadOnly | XY2ReadOnly | WHReadOnly | DistReadOnly |
                                                AngleReadOnly | AspectReadOnly | AreaReadOnly };

    void setPosition(const QPoint& position);

    WindowFinder* m_windowFinder { new WindowFinder() };
    PointerTracker* m_pointerTracker { new PointerTracker(this) };
    WindowTracker* m_windowTracker { new WindowTracker(this) };
    Rectangle* m_rectangle;
    ToolDataWindow* m_dataWindow;
};
