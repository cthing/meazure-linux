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
#include <algorithm>


PrefsDialog::PrefsDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Preferences"));

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PrefsDialog::accept);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PrefsDialog::apply);

    m_prefsPages.push_back(new ToolPrefsPage());
    m_prefsPages.push_back(new RulerPrefsPage());

    auto* tabs = new QTabWidget();
    for (PrefsPage* page : m_prefsPages) {
        tabs->addTab(page, page->getName());

        connect(page, &PrefsPage::dirtyChanged, this, [this, buttonBox](bool) {
            buttonBox->button(QDialogButtonBox::Apply)->setEnabled(isDirty());
        });
    }

    auto* layout = new QVBoxLayout();
    layout->addWidget(tabs);
    layout->addWidget(buttonBox);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

void PrefsDialog::showEvent(QShowEvent*) {
    for (PrefsPage* page : m_prefsPages) {
        page->initialize();
    }
}

bool PrefsDialog::isDirty() const {
    return std::any_of(m_prefsPages.begin(), m_prefsPages.end(), [](PrefsPage* page) { return page->isDirty(); });
}

void PrefsDialog::apply() {
    for (PrefsPage* page : m_prefsPages) {
        page->apply();
    }
}

void PrefsDialog::accept() {
    apply();
    QDialog::accept();
}
