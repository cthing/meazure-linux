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

#include "RulerPrefsPage.h"
#include "CheckerBoard.h"
#include <meazure/utils/LayoutUtils.h>
#include <QLabel>
#include <QGridLayout>
#include <QColorDialog>
#include <QToolTip>


RulerPrefsPage::RulerPrefsPage(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr) : // NOLINT(cppcoreguidelines-pro-type-member-init)
        PrefsPage(screenInfo, unitsMgr),
        m_model(new RulerPrefsModel(this)) {
    createUI();
    configure();
}

void RulerPrefsPage::createUI() {
    using namespace LayoutUtils;        // NOLINT(google-build-using-namespace)

    m_backColorButton = new QPushButton(tr("Background..."));
    m_backColorButton->setToolTip(tr("Select ruler background color"));
    m_backColorButton->setWhatsThis(tr("Selects the ruler background color."));

    m_backDefaultButton = new QPushButton(tr("Default"));
    m_backDefaultButton->setToolTip(tr("Restore default background color"));
    m_backDefaultButton->setWhatsThis(tr("Restores the ruler default background color."));

    m_borderColorButton = new QPushButton(tr("Marks/Text..."));
    m_borderColorButton->setToolTip(tr("Select ruler marks and text color"));
    m_borderColorButton->setWhatsThis(tr("Selects the rule marks and text color."));

    m_borderDefaultButton = new QPushButton(tr("Default"));
    m_borderDefaultButton->setToolTip(tr("Restore default marks and text color"));
    m_borderDefaultButton->setWhatsThis(tr("Restores the ruler default marks and text color."));

    auto* opacityLabel = new QLabel(tr("Opacity:"));
    auto* minOpacityLabel = new QLabel(tr("%1%").arg(k_minOpacity));
    auto* maxOpacityLabel = new QLabel(tr("%1%").arg(k_maxOpacity));
    m_opacitySlider = new QSlider(Qt::Horizontal);
    m_opacitySlider->setWhatsThis(tr("Selects the opacity of the ruler background, marks and text."));

    auto* checkerBoardLight = new CheckerBoard(CheckerBoard::Light);
    auto* checkerBoardDark = new CheckerBoard(CheckerBoard::Dark);

    m_ruler1 = new Ruler(m_screenInfo, m_unitsMgr, false, checkerBoardLight, *m_model->m_rulerBackColor,
                         *m_model->m_rulerBorderColor, Colors::opacityFromPercent(*m_model->m_rulerOpacity));
    m_ruler2 = new Ruler(m_screenInfo, m_unitsMgr, false, checkerBoardDark, *m_model->m_rulerBackColor,
                         *m_model->m_rulerBorderColor, Colors::opacityFromPercent(*m_model->m_rulerOpacity));

    const int screenIdx = m_screenInfo->screenForWindow(this);
    const QSizeF platformScale = m_screenInfo->getPlatformScale(screenIdx);
    const int rulerLength = qRound(platformScale.height() * k_rulerLength);

    m_ruler1->setPosition(k_rulerOrigin, rulerLength, 90);
    m_ruler2->setPosition(k_rulerOrigin, rulerLength, 90);

    auto* layout = new QGridLayout();
    setLayout(layout);

    layout->setContentsMargins(k_contentMargin);
    layout->setVerticalSpacing(k_verticalSpacing);

    layout->addWidget(m_backColorButton,     k_row0, k_col0);
    layout->addWidget(m_backDefaultButton,   k_row0, k_col1);
    layout->addWidget(m_borderColorButton,   k_row1, k_col0);
    layout->addWidget(m_borderDefaultButton, k_row1, k_col1);

    auto* opacityLayout = new QHBoxLayout();
    opacityLayout->addWidget(opacityLabel);
    opacityLayout->addSpacing(k_opacitySpace);
    opacityLayout->addWidget(minOpacityLabel);
    opacityLayout->addSpacing(k_opacitySpace);
    opacityLayout->addWidget(m_opacitySlider);
    opacityLayout->addSpacing(k_opacitySpace);
    opacityLayout->addWidget(maxOpacityLabel);
    layout->addLayout(opacityLayout, k_row2, k_col0, k_rowspan1, k_colspan2, Qt::AlignTop);
    layout->setRowStretch(k_row2, k_stretch1);

    layout->setColumnMinimumWidth(k_col2, k_columnSpacer);

    layout->addWidget(checkerBoardLight, k_row0, k_col3, k_rowspan4, k_colspan1);
    layout->addWidget(checkerBoardDark,  k_row0, k_col4, k_rowspan4, k_colspan1);
}

void RulerPrefsPage::configure() {
    // Signals

    connect(m_model, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

    // Ruler colors

    connect(m_backColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_model->m_rulerBackColor->getValue(), this,
                                                    tr("Ruler Background"));
        if (color.isValid()) {
            m_model->m_rulerBackColor->setValue(color.rgb());
        }
    });
    connect(m_backDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_rulerBackColor->restore();
    });
    connect(m_model->m_rulerBackColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb backColor = value.value<QRgb>();
        m_ruler1->setColors(backColor, *m_model->m_rulerBorderColor);
        m_ruler2->setColors(backColor, *m_model->m_rulerBorderColor);
    });

    connect(m_borderColorButton, &QPushButton::clicked, this, [this]() {
        const QColor color = QColorDialog::getColor(m_model->m_rulerBorderColor->getValue(), this,
                                                    tr("Markings and Text"));
        if (color.isValid()) {
            m_model->m_rulerBorderColor->setValue(color.rgb());
        }
    });
    connect(m_borderDefaultButton, &QPushButton::clicked, this, [this]() {
        m_model->m_rulerBorderColor->restore();
    });
    connect(m_model->m_rulerBorderColor, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QRgb borderColor = value.value<QRgb>();
        m_ruler1->setColors(*m_model->m_rulerBackColor, borderColor);
        m_ruler2->setColors(*m_model->m_rulerBackColor, borderColor);
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
        m_model->m_rulerOpacity->setValue(value);
    });
    connect(m_model->m_rulerOpacity, &PreferenceBase::valueChanged, this, [this](QVariant value) { // NOLINT(performance-unnecessary-value-param)
        const QSignalBlocker blocker(m_opacitySlider);
        const int opacity = value.value<int>();

        m_opacitySlider->setValue(opacity);
        m_opacitySlider->setToolTip(QString("%1%").arg(opacity));

        m_ruler1->setOpacity(opacity);
        m_ruler2->setOpacity(opacity);
    });
}

void RulerPrefsPage::initialize() {
    m_model->initialize();
}

void RulerPrefsPage::apply() {
    m_model->apply();
}

[[nodiscard]] bool RulerPrefsPage::isDirty() const {
    return m_model->isDirty();
}
