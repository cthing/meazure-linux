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

#include "Graphic.h"
#include "Colors.h"
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsProvider.h>
#include <QPoint>
#include <QTimer>
#include <QBrush>
#include <QPen>
#include <QPainterPath>


/// Provides a drag handle in the form of a small circle. Typically, these are used to resize or rotate other
/// graphic objects.
///
class Handle : public Graphic {

    Q_OBJECT

public:
    explicit Handle(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider,
                    QWidget* parent = nullptr, const QString& tooltip = "", int id = -1,
                    QRgb backgroundColor = Colors::get(Colors::CrosshairBack),
                    QRgb highlightColor = Colors::get(Colors::CrosshairHighlight),
                    QRgb borderColor = Colors::get(Colors::CrosshairBorder),
                    QRgb opacity = Colors::get(Colors::CrosshairOpacity));

    void setColors(QRgb background, QRgb highlight, QRgb border);

    void setOpacity(int opacityPercent);

    void setPosition(const QPoint& center);

    void flash(int flashCount = k_defaultFlashCount);

    void strobe();

signals:
    void moved(Handle& handle, int id, QPoint center);

    void entered(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);

    void departed(Handle& handle, int id);

    void selected(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);

    void deselected(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);

    void dragged(Handle& handle, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void flashHandler();
    void colorChanged(Colors::Item item, QRgb color);

private:
    static constexpr double k_size { 0.125 };       // Inches
    static constexpr int k_sizeMin { 5 };           // Pixels
    static constexpr double k_outlineWidth { 1.0 }; // Pixels
    static constexpr int k_defaultFlashCount { 9 };
    static constexpr int k_strobeCount { 1 };

    void init();

    [[nodiscard]] QPoint findCenter(const QPoint& point) const;

    int m_id;
    QBrush m_backgroundBrush;
    QBrush m_highlightBrush;
    QPen m_highlightPen;
    QPen m_borderPen;
    bool m_pointerOver { false };
    bool m_highlight { false };
    QPoint m_centerOffset;
    QPoint m_position;
    QPoint m_initialGrabPosition;
    QTimer m_flashTimer;
    int m_flashCountDown { -1 };
};
