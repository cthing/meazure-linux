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
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/config/ConfigMgr.h>
#include <QDialog>
#include <QWidget>
#include <QShowEvent>
#include <QTabWidget>
#include <vector>


/// Application preferences.
///
class PrefsDialog : public QDialog {

public:
    explicit PrefsDialog(ScreenInfo* screenInfo, UnitsMgr* unitsMgr, ConfigMgr* configMgr, QWidget* parent);

public slots:
    int execPage(PrefsPageId pageId);
    void selectPage(PrefsPageId pageId);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void apply();
    void accept() override;
    void reset();

private:
    [[nodiscard]] bool isDirty() const;

    QTabWidget* m_tabs;
    std::vector<PrefsPage*> m_prefsPages;
    ConfigMgr* m_configMgr;
};
