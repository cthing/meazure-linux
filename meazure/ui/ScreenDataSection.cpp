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

#include "ScreenDataSection.h"
#include <meazure/tools/ToolMgr.h>
#include <meazure/App.h>
#include <QGridLayout>
#include <QPushButton>
#include <QIcon>


ScreenDataSection::ScreenDataSection() : // NOLINT(cppcoreguidelines-pro-type-member-init)
        m_screenInfo(App::instance()->getScreenInfo()),
        m_unitsMgr(App::instance()->getUnitsMgr()) {
    createFields();

    const ToolMgr& toolMgr = App::instance()->getToolMgr();
    connect(&toolMgr, &ToolMgr::xy1PositionChanged, this, &ScreenDataSection::update);
    connect(&toolMgr, &ToolMgr::xy2PositionChanged, this, &ScreenDataSection::update);
    connect(&toolMgr, &ToolMgr::xyvPositionChanged, this, &ScreenDataSection::update);

    connect(&m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, &ScreenDataSection::linearUnitsChanged);
}

void ScreenDataSection::createFields() {
    auto* screenLabel = new QLabel(tr("Screen:"));
    m_screenName = new QLabel();

    auto* wLabel = new QLabel(tr("W:"));
    m_wField = new DataField(k_fieldWidth, false, true);

    auto* hLabel = new QLabel(tr("H:"));
    m_hField = new DataField(k_fieldWidth, false, true);
    m_hUnits = new QLabel();
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_hField);
    hLayout->addWidget(m_hUnits);

    auto* calButton = new QPushButton(QIcon(":/images/CalWarning.svg"), "");
    calButton->setToolTip(tr("Calibrate screen resolution"));
    hLayout->addSpacing(10);
    hLayout->addWidget(calButton);

    auto* rxLabel = new QLabel(tr("Rx:"));
    m_rxField = new DataField(k_fieldWidth, false, true);

    auto* ryLabel = new QLabel(tr("Ry:"));
    m_ryField = new DataField(k_fieldWidth, false, true);
    m_ryUnits = new QLabel();
    auto* ryLayout = new QHBoxLayout();
    ryLayout->addWidget(m_ryField);
    ryLayout->addWidget(m_ryUnits);

    auto* layout = new QGridLayout();
    layout->addWidget(screenLabel, 0, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_screenName, 0, 1, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(wLabel, 1, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_wField, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(hLabel, 1, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(hLayout, 1, 4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(rxLabel, 2, 0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_rxField, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(ryLabel, 2, 3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(ryLayout, 2, 4, Qt::AlignLeft | Qt::AlignVCenter);

    setLayout(layout);
}

void ScreenDataSection::update(QPointF, QPoint rawPos) {
    const int screenIdx = m_screenInfo.screenForPoint(rawPos);
    if (screenIdx != m_currentScreenIdx) {
        m_currentScreenIdx = screenIdx;
        refresh();
    }
}

void ScreenDataSection::refresh() {
    QString title = m_screenInfo.getScreenName(m_currentScreenIdx);
    if (title.isEmpty()) {
        title = QString("%1").arg(m_currentScreenIdx);
    }
    if (m_screenInfo.isPrimary(m_currentScreenIdx)) {
        title += tr(" (primary)");
    }
    m_screenName->setText(title);

    const QRect screenRect = m_screenInfo.getScreenRect(m_currentScreenIdx);
    const QSizeF wh = m_unitsMgr.getWidthHeight(screenRect.topLeft(), screenRect.bottomRight());
    m_wField->setValue(wh.width());
    m_hField->setValue(wh.height());

    const QSizeF res = m_screenInfo.getScreenRes(m_currentScreenIdx);
    const QSizeF convertedRes = m_unitsMgr.convertRes(res);
    m_rxField->setValue(convertedRes.width());
    m_ryField->setValue(convertedRes.height());
}

void ScreenDataSection::linearUnitsChanged(LinearUnitsId) {
    const LinearUnits* linearUnits = m_unitsMgr.getLinearUnits();

    m_hUnits->setText(linearUnits->getLengthLabel());
    m_ryUnits->setText(linearUnits->getResLabel());

    m_wField->setDecimals(linearUnits->getDisplayPrecision(Width));
    m_hField->setDecimals(linearUnits->getDisplayPrecision(Height));
    m_rxField->setDecimals(linearUnits->getDisplayPrecision(ResX));
    m_ryField->setDecimals(linearUnits->getDisplayPrecision(ResY));

    refresh();
}
