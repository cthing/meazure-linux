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

#include "ToolDataWindow.h"
#include <meazure/units/Units.h>
#include <QGridLayout>
#include <QGraphicsOpacityEffect>


ToolDataWindow::ToolDataWindow(const ScreenInfoProvider& screenInfoProvider, const UnitsProvider& unitsProvider, // NOLINT(cppcoreguidelines-pro-type-member-init)
                               RadioToolTraits traits, QWidget* parent, QRgb opacity) :
        QFrame(parent),
        m_screenInfo(screenInfoProvider),
        m_units(unitsProvider) {
    constexpr int k_margin = 4;           // Pixels
    constexpr int k_verticalSpace = 2;    // Pixels
    constexpr int k_horizontalSpace = 3;  // Pixels

    if (parent == nullptr) {
        setWindowFlag(Qt::X11BypassWindowManagerHint, true);
        setAttribute(Qt::WA_QuitOnClose, false);
    }

    m_backgroundColor = palette().color(QPalette::ToolTipBase);
    m_textColor = palette().color(QPalette::ToolTipText);

    setAutoFillBackground(true);
    setFrameStyle(Box | Plain);
    setLineWidth(1);
    setColors();

    if (parent == nullptr) {
        setWindowOpacity(Colors::opacityToFraction(opacity));
    } else {
        auto* effect = new QGraphicsOpacityEffect();
        effect->setOpacity(Colors::opacityToFraction(opacity));
        setGraphicsEffect(effect);
    }

    m_flashTimer.setTimerType(Qt::PreciseTimer);
    m_flashTimer.setInterval(100);
    connect(&m_flashTimer, &QTimer::timeout, this, &ToolDataWindow::flashHandler);

    auto* layout = new QGridLayout();
    layout->setContentsMargins(k_margin, k_margin, k_margin, k_margin);
    layout->setHorizontalSpacing(k_horizontalSpace);
    layout->setVerticalSpacing(k_verticalSpace);
    int row = 0;

    auto configure = [&row, &layout, &traits](RadioToolTrait availableTrait, const QString& labelStr,
            QLabel*& valueLabel) {
        const bool present = ((traits & availableTrait) != 0);
        if (present) {
            auto *label = new QLabel(labelStr);
            valueLabel = new QLabel();
            layout->addWidget(label, row, 0, Qt::AlignRight | Qt::AlignVCenter);
            layout->addWidget(valueLabel, row, 1, Qt::AlignRight | Qt::AlignVCenter);
            row++;
        }
    };

    configure(XY1Available, tr("X1:"), m_x1Value);
    configure(XY1Available, tr("Y1:"), m_y1Value);
    configure(XY2Available, tr("X2:"), m_x2Value);
    configure(XY2Available, tr("Y2:"), m_y2Value);
    configure(XYVAvailable, tr("XV:"), m_xvValue);
    configure(XYVAvailable, tr("YV:"), m_yvValue);
    configure(WHAvailable, tr("W:"), m_wValue);
    configure(WHAvailable, tr("H:"), m_hValue);
    configure(DistAvailable, tr("D:"), m_dValue);
    configure(AngleAvailable, tr("A:"), m_aValue);
    configure(AspectAvailable, tr("As:"), m_asValue);
    configure(AreaAvailable, tr("Ar:"), m_arValue);

    setLayout(layout);
}

void ToolDataWindow::setOpacity(int opacityPercent) {
    if (parent() == nullptr) {
        setWindowOpacity(opacityPercent / 100.0);
    } else {
        auto* effect = dynamic_cast<QGraphicsOpacityEffect*>(graphicsEffect());
        effect->setOpacity(opacityPercent / 100.0);
    }
}

void ToolDataWindow::moveNear(const QRect& target) {
    const int screenIndex = m_screenInfo.screenForRect(target);
    const QRect screenRect = m_screenInfo.getScreenRect(screenIndex);
    const QRect expandedTarget = target.marginsAdded(k_targetMargins);

    int x = expandedTarget.left() - size().width();
    if (x <= screenRect.left()) {
        x = expandedTarget.right();
    }
    int y = expandedTarget.bottom();
    if (y + size().height() >= screenRect.bottom()) {
        y = expandedTarget.top() - size().height();
    }

    move(x, y);
}

void ToolDataWindow::moveNear(const QPoint& target) {
    const QRect targetRect(target.x(), target.y(), 1, 1);
    moveNear(targetRect);
}

void ToolDataWindow::strobe() {
    m_flashCountDown = 1;
    m_flashTimer.start();
}

void ToolDataWindow::flashHandler() {
    m_flashCountDown--;
    if (m_flashCountDown < 0) {
        m_showText = true;
        m_flashTimer.stop();
    } else {
        m_showText = !m_showText;
    }

    setColors();
}

void ToolDataWindow::setColors() {
    QPalette p;
    p.setColor(QPalette::Window, m_backgroundColor);
    p.setColor(QPalette::WindowText, m_showText ? m_textColor : m_backgroundColor);
    setPalette(p);

    repaint();
}

void ToolDataWindow::xy1PositionChanged(QPointF coord, QPoint) {
    m_x1Value->setText(m_units.format(XCoord, coord.x()));
    m_y1Value->setText(m_units.format(YCoord, coord.y()));
    adjustSize();
}

void ToolDataWindow::xy2PositionChanged(QPointF coord, QPoint) {
    m_x2Value->setText(m_units.format(XCoord, coord.x()));
    m_y2Value->setText(m_units.format(YCoord, coord.y()));
    adjustSize();
}

void ToolDataWindow::xyvPositionChanged(QPointF coord, QPoint) {
    m_xvValue->setText(m_units.format(XCoord, coord.x()));
    m_yvValue->setText(m_units.format(YCoord, coord.y()));
    adjustSize();
}

void ToolDataWindow::widthHeightChanged(QSizeF widthHeight) {
    m_wValue->setText(m_units.format(Width, widthHeight.width()));
    m_hValue->setText(m_units.format(Height, widthHeight.height()));
    adjustSize();
}

void ToolDataWindow::distanceChanged(double distance) {
    m_dValue->setText(m_units.format(Distance, distance));
    adjustSize();
}

void ToolDataWindow::angleChanged(double angle) {
    m_aValue->setText(m_units.format(Angle, angle));
    adjustSize();
}

void ToolDataWindow::aspectChanged(double aspect) {
    m_asValue->setText(LinearUnits::formatAspect(aspect));
    adjustSize();
}

void ToolDataWindow::areaChanged(double area) {
    m_arValue->setText(m_units.format(Area, area));
    adjustSize();
}
