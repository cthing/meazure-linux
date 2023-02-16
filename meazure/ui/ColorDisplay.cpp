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

#include "ColorDisplay.h"
#include <meazure/graphics/Colors.h>
#include <QHBoxLayout>
#include <QPushButton>
#include <QActionGroup>
#include <QPalette>
#include <QColor>
#include <QGuiApplication>
#include <QClipboard>


ColorDisplay::ColorDisplay(const ScreenInfoProvider* screenInfo) :
        m_colorSpaceLabel(new QLabel()),
        m_colorField(new QLineEdit()),
        m_colorSwatch(new QFrame()),
        m_colorSwatchPalette(new QPalette()) {
    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    layout->addWidget(m_colorSpaceLabel);

    m_colorField->setAlignment(Qt::AlignLeft);
    m_colorField->setReadOnly(true);
    m_colorField->setStatusTip(tr("Active position color"));
    m_colorField->setWhatsThis(tr("Color of the active position in the current format."));
    QPalette palette;
    palette.setColor(QPalette::Base, palette.color(QPalette::Window));
    m_colorField->setPalette(palette);
    layout->addWidget(m_colorField, Qt::AlignVCenter);

    m_colorSwatch->setStatusTip(tr("Active position color"));
    m_colorSwatch->setWhatsThis(tr("Color of the active position."));
    m_colorSwatch->setAutoFillBackground(true);
    m_colorSwatch->setLineWidth(1);
    m_colorSwatch->setFrameStyle(QFrame::Box | QFrame::Plain);
    layout->addWidget(m_colorSwatch, Qt::AlignVCenter);

    m_copyColorAction = new QAction(tr("Copy Color"), this);
    m_copyColorAction->setShortcut(QKeySequence("Ctrl+L"));
    m_copyColorAction->setStatusTip(tr("Copy color to clipboard"));
    m_copyColorAction->setWhatsThis(tr("Copies the active position color to the clipboard."));
    connect(m_copyColorAction, &QAction::triggered, this, [this]() {
        QGuiApplication::clipboard()->setText(m_colorField->text());
    });

    const int screenIdx = screenInfo->screenForWindow(this);
    const QSizeF& platformScale = screenInfo->getPlatformScale(screenIdx);

    auto* copyButton = new QPushButton(QIcon(":/images/Clipboard.svg"), "");
    copyButton->setIconSize(QSize(qRound(platformScale.width() * k_copyButtonIconSize),
                                  qRound(platformScale.height() * k_copyButtonIconSize)));
    copyButton->setFixedSize(QSize(qRound(platformScale.width() * k_copyButtonSize),
                                   qRound(platformScale.height() * k_copyButtonSize)));
    copyButton->setToolTip(tr("Copy color to clipboard"));
    copyButton->setStatusTip(tr("Copy color to clipboard"));
    copyButton->setWhatsThis(tr("Copies the active position color to the clipboard."));
    connect(copyButton, &QPushButton::clicked, this, [this]() {
        QGuiApplication::clipboard()->setText(m_colorField->text());
    });
    layout->addWidget(copyButton, Qt::AlignVCenter);

    auto* colorFormatGroup = new QActionGroup(this);
    colorFormatGroup->setExclusive(true);

    auto createFormatAction = [this, colorFormatGroup](const QString& text, const QString& statusTip,
            ColorFormatId colorFormatId, bool colorMatching) {
        auto* action = new QAction(text, colorFormatGroup);
        action->setCheckable(true);
        action->setStatusTip(statusTip);
        action->setData(colorMatching);
        connect(action, &QAction::triggered, this, [this, colorFormatId] { setColorFormat(colorFormatId); });
        connect(this, &ColorDisplay::colorFormatChanged, this, [action, colorFormatId](ColorFormatId id) {
            action->setChecked(id == colorFormatId);
        });
        m_colorFormatActions.push_back(action);
    };

    createFormatAction(tr("&R G B"), tr("Decimal red, green, blue"), RGBFmt, false);
    createFormatAction(tr("&#RRGGBB"), tr("Hex red, green, blue"), RGBHexFmt, false);
    createFormatAction(tr("&C M Y"), tr("Cyan, magenta, yellow"), CMYFmt, false);
    createFormatAction(tr("C &M Y K"), tr("Cyan, magenta, yellow, black"), CMYKFmt, false);
    createFormatAction(tr("&H S L"), tr("Hue, saturation, lightness"), HSLFmt, false);
    createFormatAction(tr("&Y Cb Cr"), tr("Luminance, blue diff, red diff"), YCbCrFmt, false);
    createFormatAction(tr("Y &I Q"), tr("Luminance, in-phase, quadrature"), YIQFmt, false);
    createFormatAction(tr("&Extended Name"), tr("Match to extended web color name"), ExtendedNameFmt, true);
    createFormatAction(tr("Extended #RRGGBB"), tr("Match to extended web color"), ExtendedHexFmt, true);
    createFormatAction(tr("&Basic Name"), tr("Match to basic web color name"), BasicNameFmt, true);
    createFormatAction(tr("Basic #RRGGBB"), tr("Match to basic web color"), BasicHexFmt, true);
}

void ColorDisplay::writeConfig(Config& config) const {
    config.writeInt("ColorFmt", m_colorFmt);
}

void ColorDisplay::readConfig(const Config& config) {
    setColorFormat(static_cast<ColorFormatId>(config.readInt("ColorFmt", m_colorFmt)));
}

void ColorDisplay::setColorFormat(ColorFormatId colorFmt) {
    m_colorFmt = colorFmt;

    emit colorFormatChanged(colorFmt);
}

void ColorDisplay::setColor(QRgb color) {
    QString labelText;
    QString colorText;
    QRgb swatchColor = 0;

    switch (m_colorFmt) {
        case RGBFmt:
            labelText = "RGB";
            colorText = QString("%1 %2 %3").arg(qRed(color)).arg(qGreen(color)).arg(qBlue(color));
            swatchColor = color;
            break;
        case RGBHexFmt:
            labelText = "RGB";
            colorText = QString("#%1%2%3")
                            .arg(qRed(color), 2, 16, k_fillChar)
                            .arg(qGreen(color), 2, 16, k_fillChar)
                            .arg(qBlue(color), 2, 16, k_fillChar);
            swatchColor = color;
            break;
        case CMYFmt:
        {
            labelText = "CMY";
            const Colors::CMY cmy = Colors::RGBtoCMY(color);
            colorText = QString("%1 %2 %3").arg(cmy.cyan).arg(cmy.magenta).arg(cmy.yellow);
            swatchColor = color;
        }
            break;
        case CMYKFmt:
        {
            labelText = "CMYK";
            const Colors::CMYK cmyk = Colors::RGBtoCMYK(color);
            colorText = QString("%1 %2 %3 %4").arg(cmyk.cyan).arg(cmyk.magenta).arg(cmyk.yellow).arg(cmyk.black);
            swatchColor = color;
        }
            break;
        case HSLFmt:
        {
            labelText = "HSL";
            const Colors::HSL hsl = Colors::RGBtoHSL(color);
            colorText = QString("%1 %2 %3").arg(hsl.hue).arg(hsl.saturation).arg(hsl.lightness);
            swatchColor = color;
        }
            break;
        case YCbCrFmt:
        {
            labelText = "YCbCr";
            const Colors::YCbCr ycbcr = Colors::RGBtoYCbCr(color);
            colorText = QString("%1 %2 %3").arg(ycbcr.y).arg(ycbcr.cb).arg(ycbcr.cr);
            swatchColor = color;
        }
            break;
        case YIQFmt:
        {
            labelText = "YIQ";
            const Colors::YIQ yiq = Colors::RGBtoYIQ(color);
            colorText = QString("%1 %2 %3").arg(yiq.y).arg(yiq.i).arg(yiq.q);
            swatchColor = color;
        }
            break;
        case BasicNameFmt:
        {
            labelText = "Basic";
            const Colors::ColorTableEntry* colorEntry = Colors::matchBasicColor(color);
            colorText = colorEntry->name;
            swatchColor = colorEntry->rgb;
        }
            break;
        case BasicHexFmt:
        {
            labelText = "Basic";
            const Colors::ColorTableEntry* colorEntry = Colors::matchBasicColor(color);
            colorText = QString("#%1%2%3")
                    .arg(qRed(colorEntry->rgb), 2, 16, k_fillChar)
                    .arg(qGreen(colorEntry->rgb), 2, 16, k_fillChar)
                    .arg(qBlue(colorEntry->rgb), 2, 16, k_fillChar);
            swatchColor = colorEntry->rgb;
        }
            break;
        case ExtendedNameFmt:
        {
            labelText = "Ext";
            const Colors::ColorTableEntry* colorEntry = Colors::matchExtendedColor(color);
            colorText = colorEntry->name;
            swatchColor = colorEntry->rgb;
        }
            break;
        case ExtendedHexFmt:
        {
            labelText = "Ext";
            const Colors::ColorTableEntry* colorEntry = Colors::matchExtendedColor(color);
            colorText = QString("#%1%2%3")
                    .arg(qRed(colorEntry->rgb), 2, 16, k_fillChar)
                    .arg(qGreen(colorEntry->rgb), 2, 16, k_fillChar)
                    .arg(qBlue(colorEntry->rgb), 2, 16, k_fillChar);
            swatchColor = colorEntry->rgb;
        }
            break;
        default:
            break;
    }

    m_colorSpaceLabel->setText(labelText + ":");
    m_colorField->setText(colorText);

    m_colorSwatchPalette->setColor(QPalette::Window, swatchColor);
    m_colorSwatch->setPalette(*m_colorSwatchPalette);
}
