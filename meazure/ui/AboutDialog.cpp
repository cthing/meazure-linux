/*
 * Copyright 2023 C Thing Software
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

#include "AboutDialog.h"
#include "AppVersion.h"
#include <meazure/utils/LayoutUtils.h>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QDesktopServices>
#include <QUrl>
#include <QSysInfo>
#include <QGuiApplication>
#include <QClipboard>


AboutDialog::AboutDialog(QWidget* parent) : QDialog(parent) {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* infoLabel = new QLabel(getInfo());
    auto* cthingLabel = new QLabel("<a href='https://www.cthing.com'>www.cthing.com</a>");
    cthingLabel->setOpenExternalLinks(true);
    auto* copyrightLabel = new QLabel("Copyright 2001 C Thing Software");
    auto* contributeMsg = new QLabel(tr("We hope Meazure is a valuable addition\n"
                                        "to your software toolbox. Please consider\n"
                                        "making a financial contribution.\n\nThank you!"));
    auto* meazureIcon = new QLabel();
    meazureIcon->setPixmap(QIcon(":/images/Meazure.svg").pixmap(k_logoSize, k_logoSize));
    auto* cthingIcon = new QLabel();
    cthingIcon->setPixmap(QIcon(":/images/CThingSoftware.svg").pixmap(QSize(k_cthingSize, k_cthingSize)));

    auto* infoLayout = new QGridLayout();
    infoLayout->setHorizontalSpacing(k_hspacing);
    infoLayout->addWidget(meazureIcon, k_row0, k_col0, k_rowspan2, k_colspan1, Qt::AlignTop);
    infoLayout->addWidget(cthingIcon, k_row0, k_col2, k_rowspan2, k_colspan1, Qt::AlignTop);
    infoLayout->addWidget(infoLabel, k_row0, k_col1);
    infoLayout->addWidget(cthingLabel, k_row1, k_col0, k_rowspan1, k_colspan2);
    infoLayout->addWidget(copyrightLabel, k_row2, k_col0, k_rowspan1, k_colspan2);
    infoLayout->setColumnStretch(k_col1, k_stretch1);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    auto* contributeButton = buttonBox->addButton(tr("Contribute"), QDialogButtonBox::HelpRole);
    contributeButton->setDefault(true);
    auto* copyButton = buttonBox->addButton(tr("Copy"), QDialogButtonBox::ButtonRole::ActionRole);
    connect(contributeButton, &QPushButton::clicked, this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/sponsors/baron1405"));
    });
    connect(copyButton, &QPushButton::clicked, this, &AboutDialog::copy);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    auto* aboutLayout = new QVBoxLayout();
    aboutLayout->addLayout(infoLayout);
    aboutLayout->addSpacing(k_vspacing);
    aboutLayout->addWidget(contributeMsg);
    aboutLayout->addSpacing(k_vspacing);
    aboutLayout->addWidget(buttonBox);
    aboutLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(aboutLayout);
}

void AboutDialog::copy() {
    QGuiApplication::clipboard()->setText(getInfo());
}

QString AboutDialog::getInfo() {
    return QString("Meazure\nVersion: %1\nBuild: %2\nOS: %3")
            .arg(appVersion)
            .arg(appBuild)
            .arg(QSysInfo::prettyProductName());
}
