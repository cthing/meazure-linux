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

#include "Graphic.h"
#include "Colors.h"
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <QPainterPath>
#include <QSize>
#include <QSizeF>
#include <QPoint>
#include <QTimer>


class CrossHair : public Graphic {

    Q_OBJECT

public:
    /// Constructs a crosshair.
    ///
    /// @param[in] screenInfoProvider Information about the display screens
    /// @param[in] unitsProvider Measurement units
    /// @param[in] parent Parent widget for the crosshair or nullptr for a top level crosshair
    /// @param[in] tooltip Tooltip shown when hovering over the crosshair
    /// @param[in] id Caller defined identifier for the crosshair
    /// @param[in] backgroundColor Crosshair background color
    /// @param[in] hiliteColor Crosshair hilite color shown when the pointer is over the crosshair and during flashing
    /// @param[in] borderColor Crosshair border color
    /// @param[in] opacity Crosshair opacity
    ///
    explicit CrossHair(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider,
                       QWidget* parent = nullptr, const QString& tooltip = "", int id = -1,
                       QRgb backgroundColor = Colors::get(Colors::CrossHairBack),
                       QRgb hiliteColor = Colors::get(Colors::CrossHairHilite),
                       QRgb borderColor = Colors::get(Colors::CrossHairBorder),
                       QRgb opacity = Colors::get(Colors::CrossHairOpacity));

    /// Sets the crosshair colors.
    ///
    /// @param[in] background Crosshair background color
    /// @param[in] hilite Crosshair hilite color shown when the pointer is over the crosshair and during flashing
    /// @param[in] border Crosshair border color
    ///
    void setColors(QRgb background, QRgb hilite, QRgb border);

    /// Sets the crosshair opacity.
    ///
    /// @param[in] opacity Opacity of the crosshair ranging from 0 (transparent) to 255 (opaque)
    ///
    void setOpacity(int opacity);

    /// Sets the position of the crosshair. Emits the moved signal.
    ///
    /// @param[in] center Position for the center of the crosshair in global pixel coordinates.
    ///
    void setPosition(const QPoint& center);

    /// Visually flashes the crosshair by cycling its background between normal and hilite colors the specified
    /// number of times.
    ///
    /// @param[in] flashCount Number of times to cycle the crosshair background.
    ///
    void flash(int flashCount = defaultFlashCount);

signals:
    /// Indicates that the crosshair position has been change.
    ///
    /// @param[in] crosshair Crosshair whose position has changed
    /// @param[in] id User provided identifier for the crosshair
    /// @param[in] center New center of the crosshair in global pixel coordinates
    ///
    void moved(CrossHair& crosshair, int id, QPoint center);

    void entered(CrossHair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void departed(CrossHair& crosshair, int id);

    void selected(CrossHair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);
    void deselected(CrossHair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);

    void dragged(CrossHair& crosshair, int id, QPoint center, Qt::KeyboardModifiers keyboardModifiers);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void flashHandler();

private:
    static constexpr double outerSize { 0.30 };     // Inches
    static constexpr int outerSizeMin { 25 };       // Pixels
    static constexpr double petalWidth { 0.12 };    // Inches
    static constexpr int petalWidthMin { 10 };      // Pixels
    static constexpr int centerOffset { 2 };        // Pixels
    static constexpr double outlineWidth { 1.5 };   // Pixels
    static constexpr int defaultFlashCount { 9 };

    /// Determines the center point of the crosshair relative to the specified point.
    ///
    /// @param point  [in] Current location of the pointer, in pixels, relative to the widget.
    /// @return Center point of the crosshair relative to the specified point, in pixels, in global coordinates.
    ///
    [[nodiscard]] QPoint findCenter(const QPoint& point) const;

    /// Creates the path that defines the shape of the crosshair. The path is used as the window region and to
    /// draw the crosshair border.
    ///
    /// @param screenRes Screen resolution in pixels / inch
    /// @param size Size of the crosshair in pixels
    /// @return Path defining the shape of the crosshair
    ///
    [[nodiscard]] QPainterPath generateCrossHair(const QSizeF& screenRes, const QSize& size) const;

    const ScreenInfoProvider& m_screenProvider;
    const UnitsProvider& m_unitsProvider;
    int m_id;
    QColor m_backgroundColor;
    QColor m_hiliteColor;
    QColor m_borderColor;
    double m_opacity;
    QPainterPath m_crossHair;
    bool m_pointerOver { false };
    bool m_hilite { false };
    QPoint m_centerPosition;
    QPoint m_initialGrabPosition;
    QTimer m_flashTimer;
    int m_flashCountDown { -1 };
};
