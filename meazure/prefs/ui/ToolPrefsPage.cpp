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

#include "ToolPrefsPage.h"
#include "CheckerBoard.h"
#include <meazure/graphics/Line.h>
#include <meazure/App.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/tools/RadioToolTraits.h>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolTip>
#include <QSignalBlocker>
#include <QFrame>


ToolPrefsPage::ToolPrefsPage() : m_model(new ToolPrefsModel(this)) {    // NOLINT(cppcoreguidelines-pro-type-member-init)
    createUI();
    configure();
}

void ToolPrefsPage::createUI() {
    const ScreenInfo& screenInfo = App::instance()->getScreenInfo();
    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();

    auto* crosshairLabel = new QLabel(tr("<b>Crosshairs and Data Windows</b>"));
    auto* backColorButton = new QPushButton(tr("Background..."));
    backColorButton->setFixedWidth(k_buttonWidth);
    auto* backDefaultButton = new QPushButton(tr("Default"));
    backDefaultButton->setFixedWidth(k_buttonWidth);
    auto* borderColorButton = new QPushButton(tr("Border..."));
    borderColorButton->setFixedWidth(k_buttonWidth);
    auto* borderDefaultButton = new QPushButton(tr("Default"));
    borderDefaultButton->setFixedWidth(k_buttonWidth);
    auto* highlightColorButton = new QPushButton(tr("Highlight..."));
    highlightColorButton->setFixedWidth(k_buttonWidth);
    auto* highlightDefaultButton = new QPushButton(tr("Default"));
    highlightDefaultButton->setFixedWidth(k_buttonWidth);
    auto* opacityLabel = new QLabel(tr("Opacity:"));
    auto* minOpacityLabel = new QLabel(tr("%1%").arg(k_minOpacity));
    auto* maxOpacityLabel = new QLabel(tr("%1%").arg(k_maxOpacity));
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_normalCrosshair1 = new CrossHair(screenInfo, unitsMgr, this, "", -1,
                                       m_model->m_crosshairBackColor->getValue(),
                                       m_model->m_crosshairBackColor->getValue());
    m_normalCrosshair2 = new CrossHair(screenInfo, unitsMgr, this, "", -1,
                                       m_model->m_crosshairBackColor->getValue(),
                                       m_model->m_crosshairBackColor->getValue());
    m_hiliteCrosshair1 = new CrossHair(screenInfo, unitsMgr, this, "", -1,
                                       m_model->m_crosshairHiliteColor->getValue(),
                                       m_model->m_crosshairHiliteColor->getValue());
    m_hiliteCrosshair2 = new CrossHair(screenInfo, unitsMgr, this, "", -1,
                                       m_model->m_crosshairHiliteColor->getValue(),
                                       m_model->m_crosshairHiliteColor->getValue());
    m_dataWindow1 = new ToolDataWindow(screenInfo, unitsMgr, XY1ReadOnly, this);
    m_dataWindow1->xy1PositionChanged(QPointF(100, 200), QPoint(100, 200));
    m_dataWindow2 = new ToolDataWindow(screenInfo, unitsMgr, XY1ReadOnly, this);
    m_dataWindow2->xy1PositionChanged(QPointF(100, 200), QPoint(100, 200));

    auto* checkerBoardLight = new CheckerBoard(CheckerBoard::Light);
    auto* checkerBoardDark = new CheckerBoard(CheckerBoard::Dark);

    auto* linesLabel = new QLabel(tr("<b>Lines</b>"));
    auto* lineColorButton = new QPushButton(tr("Color..."));
    lineColorButton->setFixedWidth(k_buttonWidth);
    auto* lineDefaultButton = new QPushButton(tr("Default"));
    lineDefaultButton->setFixedWidth(k_buttonWidth);
    auto* lineThkLabel = new QLabel(tr("Line Thickness:"));
    m_lineThkSpin = new QSpinBox();
    auto* lineThkUnitsLabel = new QLabel(tr("px"));

    auto* lineSampleLight = new QFrame();
    lineSampleLight->setFrameStyle(QFrame::NoFrame);
    lineSampleLight->setAutoFillBackground(true);
    lineSampleLight->setPalette(QPalette(QPalette::Window, QColorConstants::White));

    auto* lineSampleDark = new QFrame();
    lineSampleDark->setFrameStyle(QFrame::NoFrame);
    lineSampleDark->setAutoFillBackground(true);
    lineSampleDark->setPalette(QPalette(QPalette::Window, QColorConstants::Black));

    auto* line1 = new Line(screenInfo, unitsMgr, 0.0, lineSampleLight);
    auto* line2 = new Line(screenInfo, unitsMgr, 0.0, lineSampleDark);

    auto* layout = new QGridLayout();
    setLayout(layout);

    layout->setContentsMargins(k_contentMargin);

    layout->addWidget(crosshairLabel, 0, 0, 1, 5, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(backColorButton, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(backDefaultButton, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(borderColorButton, 2, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(borderDefaultButton, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(highlightColorButton, 3, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(highlightDefaultButton, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);

    auto* opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(opacityLabel);
    opacityLayout->addWidget(minOpacityLabel);
    opacityLayout->addWidget(m_opacitySlider);
    opacityLayout->addWidget(maxOpacityLabel);
    layout->addLayout(opacityLayout, 4, 0, 1, 2);

    layout->setColumnMinimumWidth(2, k_verticalSpacer);

    layout->addWidget(checkerBoardLight, 1, 3, 4, 1);
    layout->addWidget(checkerBoardDark, 1, 4, 4, 1);

    auto* crosshairsLayoutLight = new QVBoxLayout();
    crosshairsLayoutLight->addWidget(m_normalCrosshair1, 0, Qt::AlignHCenter);
    crosshairsLayoutLight->addWidget(m_hiliteCrosshair1, 0, Qt::AlignHCenter);
    crosshairsLayoutLight->addWidget(m_dataWindow1, 0, Qt::AlignHCenter);
    checkerBoardLight->setLayout(crosshairsLayoutLight);

    auto* crosshairsLayoutDark = new QVBoxLayout();
    crosshairsLayoutDark->addWidget(m_normalCrosshair2, 0, Qt::AlignHCenter);
    crosshairsLayoutDark->addWidget(m_hiliteCrosshair2, 0, Qt::AlignHCenter);
    crosshairsLayoutDark->addWidget(m_dataWindow2, 0, Qt::AlignHCenter);
    checkerBoardDark->setLayout(crosshairsLayoutDark);

    layout->setRowMinimumHeight(5, k_hoirzontalSpacer);

    layout->addWidget(linesLabel, 6, 0, 1, 5, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(lineColorButton, 7, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(lineDefaultButton, 7, 1, Qt::AlignLeft | Qt::AlignVCenter);

    auto* lineLayoutLight = new QVBoxLayout();
    lineSampleLight->setLayout(lineLayoutLight);
    lineLayoutLight->addWidget(line1);
    layout->addWidget(lineSampleLight, 7, 3, 2, 1);

    auto* lineLayoutDark = new QVBoxLayout();
    lineSampleDark->setLayout(lineLayoutDark);
    lineLayoutDark->addWidget(line2);
    layout->addWidget(lineSampleDark, 7, 4, 2, 1);

    auto* lineThkLayout = new QHBoxLayout();
    lineThkLayout->addWidget(lineThkLabel);
    lineThkLayout->addWidget(m_lineThkSpin);
    lineThkLayout->addWidget(lineThkUnitsLabel, 1);
    layout->addLayout(lineThkLayout, 8, 0, 1, 2);
}

void ToolPrefsPage::configure() {
    connect(m_model->m_crosshairOpacity, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const int opacity = value.value<int>();
        m_normalCrosshair1->setOpacity(opacity);
        m_normalCrosshair2->setOpacity(opacity);
        m_hiliteCrosshair1->setOpacity(opacity);
        m_hiliteCrosshair2->setOpacity(opacity);
        m_dataWindow1->setOpacity(opacity);
        m_dataWindow2->setOpacity(opacity);
    });

    m_opacitySlider->setRange(k_minOpacity, k_maxOpacity);
    m_opacitySlider->setSingleStep(1);
    m_opacitySlider->setPageStep(10);

    connect(m_opacitySlider, &QSlider::sliderMoved, [this](int value) {
        m_opacitySlider->setToolTip(QString("%1%").arg(value));
        QToolTip::showText(QCursor::pos(), QString("%1%").arg(value));
    });
    connect(m_opacitySlider, &QSlider::valueChanged, this, [this](int value) {
        m_model->m_crosshairOpacity->setValue(value);
    });
    connect(m_model->m_crosshairOpacity, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QSignalBlocker blocker(this);
        const int opacity = value.value<int>();
        m_opacitySlider->setValue(opacity);
        m_opacitySlider->setToolTip(QString("%1%").arg(opacity));
    });

    m_lineThkSpin->setMinimum(k_minLineThk);
    m_lineThkSpin->setMaximum(k_maxLineThk);
}

void ToolPrefsPage::update() {
    m_model->update();
}
