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
#include <meazure/environment/ScreenInfoProvider.h>
#include <meazure/units/UnitsProvider.h>
#include <QFont>
#include <QRect>
#include <QPoint>


/// A ruler graphical element. The ruler resembles a classic measurement straight edge. The ruler can be oriented at
/// any angle. A ruler can be positioned anywhere on the screen and given a length. The ruler can be interactively
/// dragged resized and rotated using grab points.
///
/// Multiple position indicators can be displayed on the face of the ruler. These indicators can be used to show the
/// current position of crosshairs and other measurement points.
///
class Ruler : public Graphic {

    Q_OBJECT

public:
    explicit Ruler(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider, bool flip,
                   QWidget* parent = nullptr);

    void setPosition(const QPoint& origin, int length, int angle);

    int getRulerThk() const { return m_rulerRect.height(); }

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    static constexpr double k_majorTickHeight { 0.125 };  ///< Height of major tick marks, inches
    static constexpr int k_majorTickMinHeight { 8 };      ///< Minimum height of major tick marks, pixels
    static constexpr double k_minorTickHeight { 0.0625 }; ///< Height of minor tick marks, inches
    static constexpr int k_minorTickMinHeight { 4 };      ///< Minimum height of minor tick marks, pixels
    static constexpr double k_labelBottomMargin { 0.02 }; ///< Space between major tick and label bottom, inches
    static constexpr int k_labelBottomMinMargin { 2 };    ///< Minimum space between major tick and label bottom, pixels
    static constexpr double k_labelTopMargin { 0.02 };    ///< Space between label top and ruler border, inches
    static constexpr int k_labelTopMinMargin { 2 };       ///< Minimum space between label top and ruler border, pixels

    int convertToPixels(const QSizeF& res, double angleFrac, double value, int minValue);

    const ScreenInfoProvider& m_screenInfo;
    const UnitsProvider& m_unitsProvider;
    QFont m_font;
    bool m_flip;                                        ///< Flip rule on long edge
    int m_length { 100 };                               ///< Ruler length, pixels
    int m_angle { 0 };                                  ///< Rotation angle, degrees
    QRect m_rulerRect;                                  ///< Actual ruler rectangle
    int m_majorTickHeight { k_majorTickMinHeight };     ///< Height of major tick marks, pixels
    int m_minorTickHeight { k_minorTickMinHeight };     ///< Height of minor tick marks, pixels
    int m_labelBottomMargin { k_labelBottomMinMargin }; ///< Space between major tick and label bottom, pixels
    int m_labelTopMargin { k_labelTopMinMargin };       ///< Space between label top and ruler border, pixels
    int m_fontHeight;                                   ///< Height of the label font, pixels
    QTransform m_rulerTransform;                        ///< Rotational transform for the ruler
};
