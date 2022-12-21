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
#include <meazure/App.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/environment/ScreenInfo.h>
#include <meazure/tools/RadioToolTraits.h>
#include <QGridLayout>
#include <QLabel>
#include <QToolTip>
#include <QSignalBlocker>
#include <QFrame>
#include <QColorDialog>


ToolPrefsPage::ToolPrefsPage() : m_model(new ToolPrefsModel(this)) {    // NOLINT(cppcoreguidelines-pro-type-member-init)
    createUI();
    configure();
}

void ToolPrefsPage::createUI() {
    const ScreenInfo& screenInfo = App::instance()->getScreenInfo();
    const UnitsMgr& unitsMgr = App::instance()->getUnitsMgr();

    auto* crosshairLabel = new QLabel(tr("<b>Crosshairs and Data Windows</b>"));
    m_backColorButton = new QPushButton(tr("Background..."));
    m_backColorButton->setFixedWidth(k_buttonWidth);
    m_backDefaultButton = new QPushButton(tr("Default"));
    m_backDefaultButton->setFixedWidth(k_buttonWidth);
    m_highlightColorButton = new QPushButton(tr("Highlight..."));
    m_highlightColorButton->setFixedWidth(k_buttonWidth);
    m_highlightDefaultButton = new QPushButton(tr("Default"));
    m_highlightDefaultButton->setFixedWidth(k_buttonWidth);
    m_borderColorButton = new QPushButton(tr("Border..."));
    m_borderColorButton->setFixedWidth(k_buttonWidth);
    m_borderDefaultButton = new QPushButton(tr("Default"));
    m_borderDefaultButton->setFixedWidth(k_buttonWidth);
    auto* opacityLabel = new QLabel(tr("Opacity:"));
    auto* minOpacityLabel = new QLabel(tr("%1%").arg(k_minOpacity));
    auto* maxOpacityLabel = new QLabel(tr("%1%").arg(k_maxOpacity));
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_normalCrosshair1 = new CrossHair(screenInfo, unitsMgr, this, "Normal", -1,
                                       m_model->m_crosshairBackColor->getValue(),
                                       m_model->m_crosshairBackColor->getValue());
    m_normalCrosshair2 = new CrossHair(screenInfo, unitsMgr, this, "Normal", -1,
                                       m_model->m_crosshairBackColor->getValue(),
                                       m_model->m_crosshairBackColor->getValue());
    m_hiliteCrosshair1 = new CrossHair(screenInfo, unitsMgr, this, "Highlight", -1,
                                       m_model->m_crosshairHiliteColor->getValue(),
                                       m_model->m_crosshairHiliteColor->getValue());
    m_hiliteCrosshair2 = new CrossHair(screenInfo, unitsMgr, this, "Highlight", -1,
                                       m_model->m_crosshairHiliteColor->getValue(),
                                       m_model->m_crosshairHiliteColor->getValue());
    m_dataWindow1 = new ToolDataWindow(screenInfo, unitsMgr, XY1ReadOnly, this);
    m_dataWindow1->xy1PositionChanged(QPointF(100, 200), QPoint(100, 200));
    m_dataWindow2 = new ToolDataWindow(screenInfo, unitsMgr, XY1ReadOnly, this);
    m_dataWindow2->xy1PositionChanged(QPointF(100, 200), QPoint(100, 200));

    auto* checkerBoardLight = new CheckerBoard(CheckerBoard::Light);
    auto* checkerBoardDark = new CheckerBoard(CheckerBoard::Dark);

    auto* linesLabel = new QLabel(tr("<b>Lines</b>"));
    m_lineColorButton = new QPushButton(tr("Color..."));
    m_lineColorButton->setFixedWidth(k_buttonWidth);
    m_lineDefaultButton = new QPushButton(tr("Default"));
    m_lineDefaultButton->setFixedWidth(k_buttonWidth);
    auto* lineThkLabel = new QLabel(tr("Line Thickness:"));
    m_lineThkSpin = new QSpinBox();
    auto* lineThkUnitsLabel = new QLabel(tr("px"));

    auto* lineSampleLight = new QFrame();
    lineSampleLight->setFixedSize(k_lineSampleSize);
    lineSampleLight->setFrameStyle(QFrame::NoFrame);
    lineSampleLight->setAutoFillBackground(true);
    lineSampleLight->setPalette(QPalette(QPalette::Window, QColorConstants::White));

    auto* lineSampleDark = new QFrame();
    lineSampleDark->setFixedSize(k_lineSampleSize);
    lineSampleDark->setFrameStyle(QFrame::NoFrame);
    lineSampleDark->setAutoFillBackground(true);
    lineSampleDark->setPalette(QPalette(QPalette::Window, QColorConstants::Black));

    m_line1 = new Line(screenInfo, unitsMgr, 0.0, lineSampleLight);
    m_line2 = new Line(screenInfo, unitsMgr, 0.0, lineSampleDark);

    m_line1->setPosition(QPoint(k_lineSampleMargin, k_lineSampleMargin),
                         QPoint(lineSampleLight->width() - k_lineSampleMargin - 1,
                                lineSampleLight->height() - k_lineSampleMargin - 1));
    m_line2->setPosition(QPoint(k_lineSampleMargin, k_lineSampleMargin),
                         QPoint(lineSampleDark->width() - k_lineSampleMargin - 1,
                                lineSampleDark->height() - k_lineSampleMargin - 1));

    auto* layout = new QGridLayout();
    setLayout(layout);

    layout->setContentsMargins(k_contentMargin);

    layout->addWidget(crosshairLabel, 0, 0, 1, 5, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_backColorButton, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_backDefaultButton, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_highlightColorButton, 2, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_highlightDefaultButton, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_borderColorButton, 3, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_borderDefaultButton, 3, 1, Qt::AlignLeft | Qt::AlignVCenter);

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
    layout->addWidget(m_lineColorButton, 7, 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addWidget(m_lineDefaultButton, 7, 1, Qt::AlignLeft | Qt::AlignVCenter);

    auto* lineLayoutLight = new QVBoxLayout();
    lineSampleLight->setLayout(lineLayoutLight);
    lineLayoutLight->addWidget(m_line1);
    layout->addWidget(lineSampleLight, 7, 3, 2, 1);

    auto* lineLayoutDark = new QVBoxLayout();
    lineSampleDark->setLayout(lineLayoutDark);
    lineLayoutDark->addWidget(m_line2);
    layout->addWidget(lineSampleDark, 7, 4, 2, 1);

    auto* lineThkLayout = new QHBoxLayout();
    lineThkLayout->addWidget(lineThkLabel);
    lineThkLayout->addWidget(m_lineThkSpin);
    lineThkLayout->addWidget(lineThkUnitsLabel, 1);
    layout->addLayout(lineThkLayout, 8, 0, 1, 2);
}

void ToolPrefsPage::configure() {
    // Signals

    connect(m_model, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

    // Crosshair colors

    connect(m_backColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_model->m_crosshairBackColor->getValue(), this,
                                                    tr("Crosshair Background"));
        if (color.isValid()) {
            m_model->m_crosshairBackColor->setValue(color.rgb());
        }
    });
    connect(m_backDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_crosshairBackColor->restore();
    });
    connect(m_model->m_crosshairBackColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb backColor = value.value<QRgb>();
        m_normalCrosshair1->setColors(backColor, backColor, *m_model->m_crosshairBorderColor);
        m_normalCrosshair2->setColors(backColor, backColor, *m_model->m_crosshairBorderColor);
    });

    connect(m_highlightColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_model->m_crosshairHiliteColor->getValue(), this,
                                                    tr("Crosshair Highlight"));
        if (color.isValid()) {
            m_model->m_crosshairHiliteColor->setValue(color.rgb());
        }
    });
    connect(m_highlightDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_crosshairHiliteColor->restore();
    });
    connect(m_model->m_crosshairHiliteColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb highlightColor = value.value<QRgb>();
        m_hiliteCrosshair1->setColors(highlightColor, highlightColor, *m_model->m_crosshairBorderColor);
        m_hiliteCrosshair2->setColors(highlightColor, highlightColor, *m_model->m_crosshairBorderColor);
    });

    connect(m_borderColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_model->m_crosshairBorderColor->getValue(), this,
                                                    tr("Crosshair Border"));
        if (color.isValid()) {
            m_model->m_crosshairBorderColor->setValue(color.rgb());
        }
    });
    connect(m_borderDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_crosshairBorderColor->restore();
    });
    connect(m_model->m_crosshairBorderColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb borderColor = value.value<QRgb>();
        m_normalCrosshair1->setColors(*m_model->m_crosshairBackColor, *m_model->m_crosshairBackColor, borderColor);
        m_normalCrosshair2->setColors(*m_model->m_crosshairBackColor, *m_model->m_crosshairBackColor, borderColor);
        m_hiliteCrosshair1->setColors(*m_model->m_crosshairHiliteColor, *m_model->m_crosshairHiliteColor, borderColor);
        m_hiliteCrosshair2->setColors(*m_model->m_crosshairHiliteColor, *m_model->m_crosshairHiliteColor, borderColor);
    });

    // Opacity

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

        m_normalCrosshair1->setOpacity(opacity);
        m_normalCrosshair2->setOpacity(opacity);
        m_hiliteCrosshair1->setOpacity(opacity);
        m_hiliteCrosshair2->setOpacity(opacity);
        m_dataWindow1->setOpacity(opacity);
        m_dataWindow2->setOpacity(opacity);
    });

    // Line color

    connect(m_lineColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_model->m_lineForeColor->getValue(), this, tr("Line Color"));
        if (color.isValid()) {
            m_model->m_lineForeColor->setValue(color.rgb());
        }
    });
    connect(m_lineDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_lineForeColor->restore();
    });
    connect(m_model->m_lineForeColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb lineColor = value.value<QRgb>();
        m_line1->setColor(lineColor);
        m_line2->setColor(lineColor);
    });

    // Line thickness

    m_lineThkSpin->setMinimum(k_minLineThk);
    m_lineThkSpin->setMaximum(k_maxLineThk);

    connect(m_lineThkSpin, &QSpinBox::valueChanged, this, [this](int value) {
        m_model->m_lineWidth->setValue(value);
    });
    connect(m_model->m_lineWidth, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const int thk = value.value<int>();
        m_line1->setLineWidth(thk);
        m_line2->setLineWidth(thk);
    });
}

void ToolPrefsPage::initialize() {
    m_model->initialize();
}

void ToolPrefsPage::apply() {
    m_model->apply();
}

[[nodiscard]] bool ToolPrefsPage::isDirty() const {
    return m_model->isDirty();
}
