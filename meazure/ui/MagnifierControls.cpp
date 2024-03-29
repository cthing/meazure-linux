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

#include "MagnifierControls.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSignalBlocker>
#include <QToolButton>
#include <QIcon>


MagnifierControls::MagnifierControls(Magnifier* magnifier, const ScreenInfoProvider* screenInfo) {
    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 5, 0, 5);
    setLayout(layout);

    auto* zoomLabel = new QLabel(tr("Zoom:"));
    layout->addWidget(zoomLabel);

    auto* zoom = new QSlider(Qt::Horizontal);
    zoom->setStatusTip(tr("Magnifier zoom"));
    zoom->setWhatsThis(tr("Sets the magnification around the active position."));
    layout->addWidget(zoom);

    auto* factorLabel = new QLabel();
    layout->addWidget(factorLabel);

    QIcon freezeIcon;
    freezeIcon.addFile(Colors::isDarkMode() ? ":/images/PauseOffDark.svg" : ":/images/PauseOff.svg", QSize(),
                       QIcon::Normal, QIcon::Off);
    freezeIcon.addFile(":/images/PauseOn.svg", QSize(), QIcon::Normal, QIcon::On);

    m_freezeAction = new QAction(tr("&Freeze Magnifier"), this);
    m_freezeAction->setCheckable(true);
    m_freezeAction->setShortcut(QKeySequence("Ctrl+M"));
    m_freezeAction->setStatusTip(tr("Toggle freezing the magnifier"));
    m_freezeAction->setWhatsThis(tr("Toggles freezing the magnifier image."));
    m_freezeAction->setIcon(freezeIcon);
    connect(m_freezeAction, &QAction::triggered, magnifier, &Magnifier::setFreeze);
    connect(magnifier, &Magnifier::freezeChanged, m_freezeAction, &QAction::setChecked);

    const int screenIdx = screenInfo->screenForWindow(this);
    const QSizeF& platformScale = screenInfo->getPlatformScale(screenIdx);

    auto* freezeButton = new QToolButton();
    freezeButton->setDefaultAction(m_freezeAction);
    freezeButton->setIconSize(QSize(qRound(platformScale.width() * k_freezeButtonIconSize),
                                    qRound(platformScale.height() * k_freezeButtonIconSize)));
    freezeButton->setFixedSize(QSize(qRound(platformScale.width() * k_freezeButtonSize),
                                     qRound(platformScale.height() * k_freezeButtonSize)));
    freezeButton->setToolTip(tr("Freeze magnifier"));
    layout->addWidget(freezeButton);

    zoom->setRange(0, Magnifier::getZoomFactors().size() - 1);
    connect(zoom, &QSlider::valueChanged, magnifier, &Magnifier::setZoom);

    connect(magnifier, &Magnifier::zoomChanged, this, [zoom, factorLabel](int zoomIndex) {
        const QSignalBlocker blocker(zoom);
        const QString zoomFactor = QString("%1X").arg(Magnifier::getZoomFactors()[zoomIndex]);

        zoom->setValue(zoomIndex);
        zoom->setToolTip(zoomFactor);

        factorLabel->setText(zoomFactor);
    });
}
