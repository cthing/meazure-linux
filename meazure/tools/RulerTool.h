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

#include "Tool.h"
#include <meazure/graphics/Handle.h>
#include <meazure/graphics/Ruler.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <meazure/ui/ToolDataWindow.h>
#include <QPoint>


/// Ruler measurement tool. Provides one or more rulers that can be moved, rotated and resized.
///
class RulerTool : public Tool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "RulerTool" };

    explicit RulerTool(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                       QObject* parent = nullptr);

    ~RulerTool() override;

    RulerTool(const RulerTool&) = delete;
    RulerTool(RulerTool&&) = delete;
    RulerTool& operator=(const RulerTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    void setEnabled(bool enable) override;

    void saveProfile(Profile& profile) const override;
    void loadProfile(Profile& profile) override;

    void refresh() override;

private slots:
    void handleEntered(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void handleDeparted(Handle& handle, int id);
    void lengthHandleDragged(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void rotateHandleDragged(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void moveHandleDragged(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void moved(Handle& handle, int id, QPoint center);

private:
    static constexpr int k_hLengthId { 1 };
    static constexpr int k_vLengthId { 2 };
    static constexpr int k_moveId { 3 };
    static constexpr int k_rotateId { 4 };
    static constexpr int k_minLength { 30 };            // pixels
    static constexpr int k_rotateHandleSpacing { 4 };   // pixels

    void setPositionConstrained(const QPoint& origin, int angle, int hLength, int vLength);
    bool setPosition();

    QRect constructBoundingRect();

    QPoint m_origin;
    QPoint m_moveAnchorPoint;
    int m_hLength { 400 };
    int m_vLength { 400 };
    int m_angle { 0 };
    Ruler* m_hRuler;
    Ruler* m_vRuler;
    Handle* m_hLengthHandle;
    Handle* m_vLengthHandle;
    Handle* m_moveHandle;
    Handle* m_rotateHandle;
    ToolDataWindow* m_hLengthDataWin;
    ToolDataWindow* m_vLengthDataWin;
    ToolDataWindow* m_moveDataWin;
    ToolDataWindow* m_rotateDataWin;
};
