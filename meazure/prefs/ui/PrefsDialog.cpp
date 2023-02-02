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
#include <meazure/utils/HelpUtils.h>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QWhatsThis>
#include <algorithm>


PrefsDialog::PrefsDialog(ScreenInfo* screenInfo, UnitsMgr* unitsMgr, ConfigMgr* configMgr, QWidget *parent) :
        QDialog(parent),
        m_tabs(new QTabWidget()),
        m_configMgr(configMgr) {
    setWindowTitle(tr("Preferences"));

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply
            | QDialogButtonBox::Help);

    auto* resetButton = buttonBox->addButton(tr("Hard Reset"), QDialogButtonBox::ButtonRole::ResetRole);
    resetButton->setToolTip(tr("Perform application reset"));
    resetButton->setWhatsThis(tr("Performs a full application settings reset."));

    auto* applyButton = buttonBox->button(QDialogButtonBox::Apply);
    applyButton->setToolTip(tr("Apply all preference settings"));
    applyButton->setWhatsThis(tr("Accepts all preference settings without closing the dialog."));

    auto* okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setToolTip(tr("Accept all preference settings"));
    okButton->setWhatsThis(tr("Accepts all preference settings and closes the dialog."));

    auto* cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    cancelButton->setToolTip(tr("Discard all preference changes"));
    cancelButton->setWhatsThis(tr("Discards all preference changes and closes the dialog."));

    auto* helpButton = buttonBox->button(QDialogButtonBox::Help);
    helpButton->setText("");
    helpButton->setIcon(HelpUtils::getHelpIcon(style()));

    connect(resetButton, &QPushButton::clicked, this, &PrefsDialog::reset);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &PrefsDialog::accept);
    connect(applyButton, &QPushButton::clicked, this, &PrefsDialog::apply);
    connect(helpButton, &QPushButton::clicked, this, []() {
        QWhatsThis::enterWhatsThisMode();
    });

    m_prefsPages.push_back(new CalibrationPrefsPage(screenInfo, unitsMgr));
    m_prefsPages.push_back(new ToolPrefsPage(screenInfo, unitsMgr));
    m_prefsPages.push_back(new RulerPrefsPage(screenInfo, unitsMgr));
    m_prefsPages.push_back(new PrecisionPrefsPage(screenInfo, unitsMgr));
    m_prefsPages.push_back(new UnitsPrefsPage(screenInfo, unitsMgr));

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

void PrefsDialog::reset() {
    const QMessageBox::StandardButton response =
            QMessageBox::question(this, tr("Confirm Reset"),
                                  tr("A hard reset restores all factory default settings.\n"
                                     "All customizations will be lost, including resolution\n"
                                     "calibration, custom units settings and color selections.\n"
                                     "A hard reset cannot be undone.\n\nPerform a hard reset?"),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (response == QMessageBox::Yes) {
        m_configMgr->hardReset();
        QDialog::accept();
    }
}
