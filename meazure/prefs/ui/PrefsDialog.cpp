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

#include "PrefsDialog.h"
#include "ToolPrefsPage.h"
#include "RulerPrefsPage.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>


PrefsDialog::PrefsDialog(QWidget *parent) :
        QDialog(parent),
        m_toolPage(new ToolPrefsPage()),
        m_rulerPage(new RulerPrefsPage()) {
    setWindowTitle(tr("Preferences"));

    auto* tabs = new QTabWidget();
    tabs->addTab(m_toolPage, tr("Tools"));
    tabs->addTab(m_rulerPage, tr("Rulers"));

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

    auto* layout = new QVBoxLayout();
    layout->addWidget(tabs);
    layout->addWidget(buttons);

    setLayout(layout);
}

void PrefsDialog::showEvent(QShowEvent*) {
    m_toolPage->update();
    m_rulerPage->update();
}
