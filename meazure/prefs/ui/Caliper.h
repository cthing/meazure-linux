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

#include <QWidget>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QCursor>
#include <QFont>
#include <QFontMetrics>
#include <QPainterPath>


/// Provides a caliper for use in calibration. The caliper can be horizontally or vertically oriented.
///
class Caliper : public QWidget {

    Q_OBJECT

public:
    enum Orientation {
        Horizontal,
        Vertical
    };

    Caliper(Orientation orientation, int length, QWidget* parent = nullptr);

    void setJawPosition(int position);

signals:
    void jawPositionChanged(int position);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    static constexpr int k_thk { 30 };              // Pixels
    static constexpr int k_ticHeight { k_thk / 3};  // Pixels
    static constexpr int k_jawThk { 10 };           // Pixels
    static constexpr int k_tickInterval { 10 };     // Pixels

    Orientation m_orientation;
    int m_length;
    QFont m_positionFont { "FreeSans", 8 };
    QFontMetrics m_positionFontMetrics { m_positionFont };
    QPainterPath m_originJawPath;
    int m_jawPosition { 30 };   // Pixels
    QCursor m_originalCursor { cursor() };
    bool m_mouseDown { false };
    int m_posDelta { 0 };
    bool m_grab { false };
};
