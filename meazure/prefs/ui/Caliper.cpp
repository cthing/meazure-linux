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

#include "Caliper.h"
#include <meazure/utils/Geometry.h>
#include <QPainter>
#include <QMouseEvent>
#include <QTransform>
#include <vector>


Caliper::Caliper(Caliper::Orientation orientation, int length, QWidget *parent) :
        QWidget(parent),
        m_orientation(orientation),
        m_length(length) {
    setMouseTracking(true);

    if (m_orientation == Horizontal) {
        setFixedSize(length, k_thk);

        m_originJawPath.moveTo(0, 0);
        m_originJawPath.lineTo(k_jawThk, 0);
        m_originJawPath.lineTo(0, k_thk);
    } else {
        setFixedSize(k_thk, length);

        m_originJawPath.moveTo(0, 0);
        m_originJawPath.lineTo(k_thk, 0);
        m_originJawPath.lineTo(k_thk, k_jawThk);
    }
}

void Caliper::setJawPosition(int position) {
    if (position >= k_jawThk && position < m_length) {
        m_jawPosition = position;

        repaint();

        emit jawPositionChanged(position);
    }
}

void Caliper::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
    painter.setFont(m_positionFont);

    if (m_orientation == Horizontal) {
        if (isEnabled()) {
            painter.fillRect(QRect(0, 0, m_jawPosition + 1, k_thk), palette().window());
        }

        std::vector<QLine> tics;
        for (int x = -1; x < m_length; x += k_tickInterval) {
            tics.emplace_back(x, 0, x, k_ticHeight);
        }
        painter.drawLines(tics.data(), static_cast<int>(tics.size()));

        painter.fillPath(m_originJawPath, palette().text());

        QPainterPath movingJawPath;
        movingJawPath.moveTo(m_jawPosition, 0);
        movingJawPath.lineTo(m_jawPosition - k_jawThk, 0);
        movingJawPath.lineTo(m_jawPosition, k_thk);
        painter.fillPath(movingJawPath, palette().text());

        if (m_mouseDown) {
            const QString positionStr(QString::number(m_jawPosition));
            const QRect numberRect = m_positionFontMetrics.boundingRect(positionStr);
            const int x = (m_jawPosition - numberRect.width()) / 2;
            const int y = qRound((k_thk + k_ticHeight + numberRect.height() - m_positionFontMetrics.descent()) / 2.0);
            painter.drawText(x, y, positionStr);
        }
    } else {
        if (isEnabled()) {
            painter.fillRect(QRect(0, 0, k_thk, m_jawPosition + 1), palette().window());
        }

        std::vector<QLine> tics;
        for (int y = -1; y < m_length; y += k_tickInterval) {
            tics.emplace_back(k_thk, y, k_thk - k_ticHeight, y);
        }
        painter.drawLines(tics.data(), static_cast<int>(tics.size()));

        painter.fillPath(m_originJawPath, palette().text());

        QPainterPath movingJawPath;
        movingJawPath.moveTo(0, m_jawPosition);
        movingJawPath.lineTo(k_thk, m_jawPosition);
        movingJawPath.lineTo(k_thk, m_jawPosition - k_jawThk);
        painter.fillPath(movingJawPath, palette().text());

        if (m_mouseDown) {
            const QString positionStr(QString::number(m_jawPosition));
            const QRect numberRect = m_positionFontMetrics.boundingRect(positionStr);
            const int x = qRound((k_thk - k_ticHeight + numberRect.height()) / 2.0 - m_positionFontMetrics.descent());
            const int y = (m_jawPosition + numberRect.width()) / 2;

            painter.save();
            painter.setTransform(Geometry::rotateAround(-90, x, y));
            painter.drawText(x, y, positionStr);
            painter.restore();
        }
    }
}

void Caliper::mousePressEvent(QMouseEvent* event) {
    m_mouseDown = true;
    m_posDelta = (m_orientation == Horizontal ? event->pos().x() : event->pos().y()) - m_jawPosition;

    repaint();
}

void Caliper::mouseReleaseEvent(QMouseEvent*) {
    m_mouseDown = false;
    m_grab = false;

    repaint();
}

void Caliper::mouseMoveEvent(QMouseEvent *event) {
    if (m_orientation == Horizontal) {
        const int x = event->pos().x();

        if (m_grab) {
            setJawPosition(x - m_posDelta);
        } else if (x >= (m_jawPosition - k_jawThk) && x <= m_jawPosition) {
            setCursor(Qt::SizeHorCursor);
            if (m_mouseDown) {
                m_grab = true;
            }
        } else {
            setCursor(m_originalCursor);
        }
    } else {
        const int y = event->pos().y();

        if (m_grab) {
            setJawPosition(y - m_posDelta);
        } else if (y >= (m_jawPosition - k_jawThk) && y <= m_jawPosition) {
            setCursor(Qt::SizeVerCursor);
            if (m_mouseDown) {
                m_grab = true;
            }
        } else {
            setCursor(m_originalCursor);
        }
    }
}
