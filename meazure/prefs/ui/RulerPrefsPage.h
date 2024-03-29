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
#include <meazure/prefs/models/RulerPrefsModel.h>
#include <meazure/graphics/Ruler.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/environment/ScreenInfo.h>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QPoint>


/// Ruler preferences user interface.
///
class RulerPrefsPage : public PrefsPage {

    Q_OBJECT

public:
    RulerPrefsPage(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr);

    PrefsPageId getId() override {
        return PrefsPageId::RulerPage;
    }

    QString getName() override {
        return tr("Rulers");
    };

    void initialize() override;
    void apply() override;

    [[nodiscard]] bool isDirty() const override;

private:
    static constexpr QMargins k_contentMargin { 10, 10, 10, 10 };   // Page margin, pixels
    static constexpr QPoint k_rulerOrigin { 10, 10 };               // Sample rulers origin on checkerboards, pixels
    static constexpr int k_rulerLength { 200 };                     // Sample rulers length, pixels
    static constexpr int k_columnSpacer { 10 };                     // Column spacing, pixels
    static constexpr int k_verticalSpacing { 15 };                  // Vertical space between rows, pixels
    static constexpr int k_opacitySpace { 5 };                      // Space between opacity slider items
    static constexpr int k_minOpacity { 20 };                       // Minimum allowed opacity, percent
    static constexpr int k_maxOpacity { 100 };                      // Maximum allowed opacity, percent

    void createUI();
    void configure();

    RulerPrefsModel* m_model;
    Ruler* m_ruler1;
    Ruler* m_ruler2;
    QPushButton* m_backColorButton;
    QPushButton* m_backDefaultButton;
    QPushButton* m_borderColorButton;
    QPushButton* m_borderDefaultButton;
    QSlider* m_opacitySlider;
};
