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
#include <meazure/tools/RadioToolTraits.h>
#include <meazure/utils/LayoutUtils.h>
#include <QGridLayout>
#include <QLabel>
#include <QToolTip>
#include <QSignalBlocker>
#include <QFrame>
#include <QColorDialog>


ToolPrefsPage::ToolPrefsPage(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr) : // NOLINT(cppcoreguidelines-pro-type-member-init)
        PrefsPage(screenInfo, unitsMgr),
        m_model(new ToolPrefsModel(this)) {
    createUI();
    configure();
}

void ToolPrefsPage::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    auto* crosshairLabel = new QLabel(tr("<b>Crosshairs</b>"));
    m_backColorButton = new QPushButton(tr("Background..."));
    m_backColorButton->setToolTip(tr("Select crosshair background color"));
    m_backColorButton->setWhatsThis(tr("Selects background color for crosshairs."));

    m_backDefaultButton = new QPushButton(tr("Default"));
    m_backDefaultButton->setToolTip(tr("Restore crosshair default background color"));
    m_backDefaultButton->setWhatsThis(tr("Restores the crosshair default background color."));

    m_highlightColorButton = new QPushButton(tr("Highlight..."));
    m_highlightColorButton->setToolTip(tr("Select crosshair highlight color"));
    m_highlightColorButton->setWhatsThis(tr("Selects highlight color for crosshairs."));

    m_highlightDefaultButton = new QPushButton(tr("Default"));
    m_highlightDefaultButton->setToolTip(tr("Restore crosshair default highlight color"));
    m_highlightDefaultButton->setWhatsThis(tr("Restores the crosshair default highlight color."));

    m_borderColorButton = new QPushButton(tr("Border..."));
    m_borderColorButton->setToolTip(tr("Select crosshair border color"));
    m_borderColorButton->setWhatsThis(tr("Selects border color for crosshairs."));

    m_borderDefaultButton = new QPushButton(tr("Default"));
    m_borderDefaultButton->setToolTip(tr("Restore crosshair default border color"));
    m_borderDefaultButton->setWhatsThis(tr("Restores the crosshair default border color."));

    auto* opacityLabel = new QLabel(tr("Opacity:"));
    auto* minOpacityLabel = new QLabel(tr("%1%").arg(k_minOpacity));
    auto* maxOpacityLabel = new QLabel(tr("%1%").arg(k_maxOpacity));
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setWhatsThis(tr("Selects the opacity of the crosshairs and floating data windows"));

    m_normalCrosshair1 = new Crosshair(m_screenInfo, m_unitsMgr, this, "Normal", -1,
                                       m_model->m_crosshairBackColor->getValue(),
                                       m_model->m_crosshairBackColor->getValue());
    m_normalCrosshair1->setColorMode(Crosshair::AlwaysBackground);
    m_normalCrosshair2 = new Crosshair(m_screenInfo, m_unitsMgr, this, "Normal", -1,
                                       m_model->m_crosshairBackColor->getValue(),
                                       m_model->m_crosshairBackColor->getValue());
    m_normalCrosshair2->setColorMode(Crosshair::AlwaysBackground);
    m_highlightCrosshair1 = new Crosshair(m_screenInfo, m_unitsMgr, this, "Highlight", -1,
                                          m_model->m_crosshairHighlightColor->getValue(),
                                          m_model->m_crosshairHighlightColor->getValue());
    m_highlightCrosshair1->setColorMode(Crosshair::AlwaysHighlight);
    m_highlightCrosshair2 = new Crosshair(m_screenInfo, m_unitsMgr, this, "Highlight", -1,
                                          m_model->m_crosshairHighlightColor->getValue(),
                                          m_model->m_crosshairHighlightColor->getValue());
    m_highlightCrosshair2->setColorMode(Crosshair::AlwaysHighlight);
    m_dataWindow1 = new ToolDataWindow(m_screenInfo, m_unitsMgr, XY1ReadOnly, this);
    m_dataWindow1->xy1PositionChanged(QPointF(100, 200), QPoint(100, 200));
    m_dataWindow2 = new ToolDataWindow(m_screenInfo, m_unitsMgr, XY1ReadOnly, this);
    m_dataWindow2->xy1PositionChanged(QPointF(100, 200), QPoint(100, 200));

    auto* checkerBoardLight = new CheckerBoard(CheckerBoard::Light);
    auto* checkerBoardDark = new CheckerBoard(CheckerBoard::Dark);

    auto* linesLabel = new QLabel(tr("<b>Lines</b>"));
    m_lineColorButton = new QPushButton(tr("Color..."));
    m_lineColorButton->setToolTip(tr("Select line color"));
    m_lineColorButton->setWhatsThis(tr("Selects the color for lines."));

    m_lineDefaultButton = new QPushButton(tr("Default"));
    m_lineDefaultButton->setToolTip(tr("Restore line default color"));
    m_lineDefaultButton->setWhatsThis(tr("Restores the default color for lines."));

    auto* lineThkLabel = new QLabel(tr("Line Thickness:"));
    m_lineThkSpin = new IntegerDataField(2, true);
    m_lineThkSpin->setWhatsThis(tr("Sets the thickness of lines, in pixels."));

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

    m_line1 = new Line(m_screenInfo, m_unitsMgr, 0.0, lineSampleLight);
    m_line2 = new Line(m_screenInfo, m_unitsMgr, 0.0, lineSampleDark);

    m_line1->setPosition(QPoint(k_lineSampleMargin, k_lineSampleMargin),
                         QPoint(lineSampleLight->width() - k_lineSampleMargin - 1,
                                lineSampleLight->height() - k_lineSampleMargin - 1));
    m_line2->setPosition(QPoint(k_lineSampleMargin, k_lineSampleMargin),
                         QPoint(lineSampleDark->width() - k_lineSampleMargin - 1,
                                lineSampleDark->height() - k_lineSampleMargin - 1));

    auto* layout = new QGridLayout();
    setLayout(layout);

    layout->setContentsMargins(k_contentMargin);

    layout->addWidget(crosshairLabel,           k_row0, k_col0, k_rowspan1, k_colspan5);
    layout->addWidget(m_backColorButton,        k_row1, k_col0);
    layout->addWidget(m_backDefaultButton,      k_row1, k_col1);
    layout->addWidget(m_highlightColorButton,   k_row2, k_col0);
    layout->addWidget(m_highlightDefaultButton, k_row2, k_col1);
    layout->addWidget(m_borderColorButton,      k_row3, k_col0);
    layout->addWidget(m_borderDefaultButton,    k_row3, k_col1);

    auto* opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(opacityLabel);
    opacityLayout->addWidget(minOpacityLabel);
    opacityLayout->addWidget(m_opacitySlider);
    opacityLayout->addWidget(maxOpacityLabel);
    layout->addLayout(opacityLayout, k_row4, k_col0, k_rowspan1, k_colspan2);

    layout->setColumnMinimumWidth(k_col2, k_verticalSpacer);

    layout->addWidget(checkerBoardLight, k_row1, k_col3, k_rowspan4, k_colspan1);
    layout->addWidget(checkerBoardDark,  k_row1, k_col4, k_rowspan4, k_colspan1);

    auto* crosshairsLayoutLight = new QVBoxLayout();
    crosshairsLayoutLight->addWidget(m_normalCrosshair1, k_stretch0, Qt::AlignHCenter);
    crosshairsLayoutLight->addWidget(m_highlightCrosshair1, k_stretch0, Qt::AlignHCenter);
    crosshairsLayoutLight->addWidget(m_dataWindow1, k_stretch0, Qt::AlignHCenter);
    checkerBoardLight->setLayout(crosshairsLayoutLight);

    auto* crosshairsLayoutDark = new QVBoxLayout();
    crosshairsLayoutDark->addWidget(m_normalCrosshair2, k_stretch0, Qt::AlignHCenter);
    crosshairsLayoutDark->addWidget(m_highlightCrosshair2, k_stretch0, Qt::AlignHCenter);
    crosshairsLayoutDark->addWidget(m_dataWindow2, k_stretch0, Qt::AlignHCenter);
    checkerBoardDark->setLayout(crosshairsLayoutDark);

    layout->setRowMinimumHeight(k_row5, k_hoirzontalSpacer);

    layout->addWidget(linesLabel,          k_row6, k_col0, k_rowspan1, k_colspan5);
    layout->addWidget(m_lineColorButton,   k_row7, k_col0);
    layout->addWidget(m_lineDefaultButton, k_row7, k_col1);

    auto* lineLayoutLight = new QVBoxLayout();
    lineSampleLight->setLayout(lineLayoutLight);
    lineLayoutLight->addWidget(m_line1);
    layout->addWidget(lineSampleLight, k_row7, k_col3, k_rowspan2, k_colspan1);

    auto* lineLayoutDark = new QVBoxLayout();
    lineSampleDark->setLayout(lineLayoutDark);
    lineLayoutDark->addWidget(m_line2);
    layout->addWidget(lineSampleDark, k_row7, k_col4, k_rowspan2, k_colspan1);

    auto* lineThkLayout = new QHBoxLayout();
    lineThkLayout->addWidget(lineThkLabel);
    lineThkLayout->addWidget(m_lineThkSpin);
    lineThkLayout->addWidget(lineThkUnitsLabel, k_stretch1);
    layout->addLayout(lineThkLayout, k_row8, k_col0, k_rowspan1, k_colspan2);
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
        const QColor color = QColorDialog::getColor(m_model->m_crosshairHighlightColor->getValue(), this,
                                                    tr("Crosshair Highlight"));
        if (color.isValid()) {
            m_model->m_crosshairHighlightColor->setValue(color.rgb());
        }
    });
    connect(m_highlightDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_crosshairHighlightColor->restore();
    });
    connect(m_model->m_crosshairHighlightColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb highlightColor = value.value<QRgb>();
        m_highlightCrosshair1->setColors(highlightColor, highlightColor, *m_model->m_crosshairBorderColor);
        m_highlightCrosshair2->setColors(highlightColor, highlightColor, *m_model->m_crosshairBorderColor);
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
        m_highlightCrosshair1->setColors(*m_model->m_crosshairHighlightColor, *m_model->m_crosshairHighlightColor, borderColor);
        m_highlightCrosshair2->setColors(*m_model->m_crosshairHighlightColor, *m_model->m_crosshairHighlightColor, borderColor);
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
        const QSignalBlocker blocker(m_opacitySlider);
        const int opacity = value.value<int>();

        m_opacitySlider->setValue(opacity);
        m_opacitySlider->setToolTip(QString("%1%").arg(opacity));

        m_normalCrosshair1->setOpacity(opacity);
        m_normalCrosshair2->setOpacity(opacity);
        m_highlightCrosshair1->setOpacity(opacity);
        m_highlightCrosshair2->setOpacity(opacity);
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
        m_lineThkSpin->setValueQuietly(thk);
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
