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

#include "Tool.h"
#include <meazure/graphics/Grid.h>
#include <meazure/units/Units.h>
#include <meazure/config/Config.h>
#include <QPoint>
#include <QSize>
#include <QSizeF>


/// Grid overlay tool. This tool overlays a rectangular grid on the screen. If there are multiple monitors, the grid
/// is overlaid continuously across all monitors. The grid spacing, origin offset and orientation can be configured.
///
class GridTool : public Tool {

    Q_OBJECT

public:
    static constexpr const char* k_toolName { "GridTool" };
    static constexpr int k_defaultSpacing { 100 };                  ///< Default grid spacing in default units
    static constexpr int k_minSize { 5 };                           ///< Minimum grid width and height, pixels
    static constexpr int k_maxSize { 100000 };                      ///< Maximum grid width and height, pixels
    static constexpr LinearUnitsId k_defaultUnits { PixelsId };     ///< Default units for the grid spacing

    explicit GridTool(const ScreenInfo* screenInfo, const UnitsProvider* unitsProvider, QObject* parent = nullptr);

    ~GridTool() override;

    GridTool(const GridTool&) = delete;
    GridTool(GridTool&&) = delete;
    GridTool& operator=(const GridTool&) = delete;

    [[nodiscard]] const char* getName() const override {
        return k_toolName;
    }

    void setEnabled(bool enable) override;

    void writeConfig(Config& config) const override;

    void readConfig(const Config& config) override;

    void softReset();
    void hardReset() override;

    void refresh() override;

    [[nodiscard]] int getXPosition() const { return m_x; }
    [[nodiscard]] int getYPosition() const { return m_y; }
    [[nodiscard]] int getWidth() const { return m_width; }
    [[nodiscard]] int getHeight() const { return m_height; }
    [[nodiscard]] int getAngle() const { return m_angle; }
    [[nodiscard]] double getHorizontalSpacing() const { return m_hSpacing; }
    [[nodiscard]] double getVerticalSpacing() const { return m_vSpacing; }
    [[nodiscard]] bool isLinkedSpacing() const { return m_linkedSpacing; }
    [[nodiscard]] LinearUnitsId getSpacingUnits() const { return m_units; }

    void setSpacing(double hspace, double vspace, bool linkSpacing, LinearUnitsId units);
    void setOrientation(int x, int y, int width, int height, int angle);

public slots:
    void setXPosition(int x);
    void setYPosition(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setAngle(int angle);
    void setHorizontalSpacing(double hspace);
    void setVerticalSpacing(double vspace);
    void setLinkedSpacing(bool linkSpacing);

signals:
    void spacingChanged(double hspace, double vspace, bool linkSpacing, LinearUnitsId units);

private:
    void init();

    void setSpacing();
    void setOrientation();


    Grid* m_grid;                 ///< Grid positioned at topLeft with widthHeight dimensions
    int m_x;                      ///< Left corner of the grid, in grid units
    int m_y;                      ///< Top corner of the grid, in grid units
    int m_width;                  ///< Width of the grid, in grid units
    int m_height;                 ///< Height of the grid, in grid units
    int m_angle;                  ///< Rotation angle of the grid, degrees
    double m_vSpacing;            ///< Horizontal grid line spacing
    double m_hSpacing;            ///< Vertical grid line spacing
    bool m_linkedSpacing;         ///< Keep vertical and horizontal spacing equal
    LinearUnitsId m_units;        ///< Units for the grid spacing
};
