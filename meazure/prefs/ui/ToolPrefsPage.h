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

#include "PrefsPage.h"
#include "PrefsPageId.h"
#include <meazure/prefs/models/ToolPrefsModel.h>
#include <meazure/graphics/Crosshair.h>
#include <meazure/graphics/Line.h>
#include <meazure/ui/ToolDataWindow.h>
#include <meazure/ui/fields/IntegerDataField.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsMgr.h>
#include <QWidget>
#include <QMargins>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QSize>


/// Tool preferences user interface.
///
class ToolPrefsPage : public PrefsPage {

    Q_OBJECT

public:
    ToolPrefsPage(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr);

    PrefsPageId getId() override {
        return PrefsPageId::ToolPage;
    }

    QString getName() override {
        return tr("Tools");
    };

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

private:
    static constexpr QMargins k_contentMargin { 10, 10, 10, 10 };   // Page margin, pixels
    static constexpr int k_verticalSpacer { 10 };                   // Spacing column, pixels
    static constexpr int k_hoirzontalSpacer { 10 };                 // Spacing row, pixels
    static constexpr QSize k_lineSampleSize { 80, 50 };             // Size of line sample, pixels
    static constexpr int k_lineSampleMargin { 5 };                  // Margin for line sample, pixels
    static constexpr int k_minOpacity { 20 };                       // Minimum allowed opacity, percent
    static constexpr int k_maxOpacity { 100 };                      // Maximum allowed opacity, percent
    static constexpr int k_minLineThk { 1 };                        // Minimum line thickness, pixels
    static constexpr int k_maxLineThk { 5 };                        // Maximum line thickness, pixels

    void createUI();
    void configure();

    ToolPrefsModel* m_model;
    Crosshair* m_normalCrosshair1;
    Crosshair* m_normalCrosshair2;
    Crosshair* m_highlightCrosshair1;
    Crosshair* m_highlightCrosshair2;
    ToolDataWindow* m_dataWindow1;
    ToolDataWindow* m_dataWindow2;
    Line* m_line1;
    Line* m_line2;
    QPushButton* m_backColorButton;
    QPushButton* m_backDefaultButton;
    QPushButton* m_highlightColorButton;
    QPushButton* m_highlightDefaultButton;
    QPushButton* m_borderColorButton;
    QPushButton* m_borderDefaultButton;
    QPushButton* m_lineColorButton;
    QPushButton* m_lineDefaultButton;
    QSlider* m_opacitySlider;
    IntegerDataField* m_lineThkSpin;
};
