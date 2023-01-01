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
#include "UnitsPrefsPage.h"
#include "PrecisionPrefsPage.h"
#include "CalibrationPrefsPage.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <algorithm>


PrefsDialog::PrefsDialog(QWidget *parent) : QDialog(parent), m_tabs(new QTabWidget()) {
    setWindowTitle(tr("Preferences"));

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PrefsDialog::accept);
    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &PrefsDialog::apply);

    m_prefsPages.push_back(new CalibrationPrefsPage());
    m_prefsPages.push_back(new ToolPrefsPage());
    m_prefsPages.push_back(new RulerPrefsPage());
    m_prefsPages.push_back(new PrecisionPrefsPage());
    m_prefsPages.push_back(new UnitsPrefsPage());

    for (PrefsPage* page : m_prefsPages) {
        m_tabs->addTab(page, page->getName());

        connect(page, &PrefsPage::dirtyChanged, this, [this, buttonBox](bool) {
            buttonBox->button(QDialogButtonBox::Apply)->setEnabled(isDirty());
        });
        connect(page, &PrefsPage::pageRequested, this, &PrefsDialog::selectPage);
    }

    auto* layout = new QVBoxLayout();
    layout->addWidget(m_tabs);
    layout->addWidget(buttonBox);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

int PrefsDialog::execPage(PrefsPageId pageId) {
    selectPage(pageId);
    return exec();
}

void PrefsDialog::selectPage(PrefsPageId pageId) {
    for (PrefsPage* page : m_prefsPages) {
        if (page->getId() == pageId) {
            m_tabs->setCurrentWidget(page);
            return;
        }
    }
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
