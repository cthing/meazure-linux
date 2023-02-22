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

#include "ScreenDataSection.h"
#include <meazure/utils/LayoutUtils.h>
#include <QGridLayout>
#include <QIcon>


ScreenDataSection::ScreenDataSection(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr, const ToolMgr* toolMgr, // NOLINT(cppcoreguidelines-pro-type-member-init)
                                     PrefsDialog* prefsDialog) :
        m_screenInfo(screenInfo),
        m_unitsMgr(unitsMgr) {
    createFields();

    connect(toolMgr, &ToolMgr::xy1PositionChanged, this, &ScreenDataSection::update);
    connect(toolMgr, &ToolMgr::xy2PositionChanged, this, &ScreenDataSection::update);
    connect(toolMgr, &ToolMgr::xyvPositionChanged, this, &ScreenDataSection::update);

    connect(m_unitsMgr, &UnitsMgr::linearUnitsChanged, this, &ScreenDataSection::linearUnitsChanged);
    connect(m_unitsMgr, &UnitsMgr::precisionsChanged, this, &ScreenDataSection::linearUnitsChanged);

    connect(m_calButton, &QPushButton::clicked, this, [prefsDialog]() {
        prefsDialog->execPage(PrefsPageId::CalibrationPage);
    });
    connect(m_screenInfo, &ScreenInfo::resolutionChanged, this, [this]() {
        m_calButton->setVisible(m_screenInfo->isCalibrationRequired());

        refresh();
    });
}

void ScreenDataSection::createFields() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* screenLabel = new QLabel(tr("Screen:"));
    m_screenName = new QLabel();

    auto* wLabel = new QLabel(tr("W:"));
    m_wField = new DoubleDataField(k_fieldWidth, false, true);
    m_wField->setStatusTip(tr("Screen width"));
    m_wField->setWhatsThis(tr("Screen width in current units."));

    auto* hLabel = new QLabel(tr("H:"));
    m_hField = new DoubleDataField(k_fieldWidth, false, true);
    m_hField->setStatusTip(tr("Screen height"));
    m_hField->setWhatsThis(tr("Screen height in current units."));
    m_hUnits = new QLabel();
    auto* hLayout = new QHBoxLayout();
    hLayout->addWidget(m_hField);
    hLayout->addWidget(m_hUnits);

    m_calButton = new QPushButton(QIcon(":/images/CalWarning.svg"), "");
    m_calButton->setToolTip(tr("Calibrate screen resolution"));
    m_calButton->setStatusTip(tr("Calibrate screen resolution"));
    m_calButton->setWhatsThis(tr("Perform screen resolution calibration."));
    hLayout->addSpacing(15);
    hLayout->addWidget(m_calButton);

    auto* rxLabel = new QLabel(tr("Rx:"));
    m_rxField = new DoubleDataField(k_fieldWidth, false, true);
    m_rxField->setStatusTip(tr("Horizontal screen resolution"));
    m_rxField->setWhatsThis(tr("Screen resolution along the x-axis."));

    auto* ryLabel = new QLabel(tr("Ry:"));
    m_ryField = new DoubleDataField(k_fieldWidth, false, true);
    m_ryField->setStatusTip(tr("Vertical screen resolution"));
    m_ryField->setWhatsThis(tr("Screen resolution along the y-axis."));
    m_ryUnits = new QLabel();
    auto* ryLayout = new QHBoxLayout();
    ryLayout->addWidget(m_ryField);
    ryLayout->addWidget(m_ryUnits);

    auto* layout = new QGridLayout();
    layout->addWidget(screenLabel,  k_row0, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_screenName, k_row0, k_col1, k_rowspan1, k_colspan4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(wLabel,   k_row1, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_wField, k_row1, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(hLabel,   k_row1, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(hLayout,  k_row1, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->addWidget(rxLabel,   k_row2, k_col0, Qt::AlignRight | Qt::AlignVCenter);
    layout->addWidget(m_rxField, k_row2, k_col1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(ryLabel,   k_row2, k_col3, Qt::AlignRight | Qt::AlignVCenter);
    layout->addLayout(ryLayout,  k_row2, k_col4, Qt::AlignLeft | Qt::AlignVCenter);

    layout->setColumnStretch(k_col4, k_stretch1);

    setLayout(layout);
}

void ScreenDataSection::update(QPointF, QPoint rawPos) {
    const int screenIdx = m_screenInfo->screenForPoint(rawPos);
    if (screenIdx != m_currentScreenIdx) {
        m_currentScreenIdx = screenIdx;
        refresh();
    }
}

void ScreenDataSection::refresh() {
    QString title = m_screenInfo->getScreenName(m_currentScreenIdx);
    if (title.isEmpty()) {
        title = QString::number(m_currentScreenIdx);
    }
    if (m_screenInfo->isPrimary(m_currentScreenIdx)) {
        title += tr(" (primary)");
    }
    m_screenName->setText(title);

    const QRect screenRect = m_screenInfo->getScreenRect(m_currentScreenIdx);
    const QSizeF wh = m_unitsMgr->getWidthHeight(screenRect.topLeft(), screenRect.bottomRight());
    m_wField->setValue(wh.width());
    m_hField->setValue(wh.height());

    const QSizeF res = m_screenInfo->getScreenRes(m_currentScreenIdx);
    const QSizeF convertedRes = m_unitsMgr->convertRes(res);
    m_rxField->setValue(convertedRes.width());
    m_ryField->setValue(convertedRes.height());
}

void ScreenDataSection::linearUnitsChanged() {
    const LinearUnits* linearUnits = m_unitsMgr->getLinearUnits();

    m_hUnits->setText(linearUnits->getLengthLabel());
    m_ryUnits->setText(linearUnits->getResLabel());

    m_wField->setDecimals(linearUnits->getDisplayPrecision(Width));
    m_hField->setDecimals(linearUnits->getDisplayPrecision(Height));
    m_rxField->setDecimals(linearUnits->getDisplayPrecision(ResX));
    m_ryField->setDecimals(linearUnits->getDisplayPrecision(ResY));

    refresh();
}
