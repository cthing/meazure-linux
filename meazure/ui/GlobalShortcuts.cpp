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

#include "GlobalShortcuts.h"
#include <QEvent>
#include <QKeyEvent>


GlobalShortcuts::GlobalShortcuts(QObject *parent) : QObject(parent) {
}

bool GlobalShortcuts::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        const QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        const bool ctrlPressed = keyEvent->modifiers().testFlag(Qt::ControlModifier);
        const int key = keyEvent->key();

        switch (key) {
            case Qt::Key_1:
                m_1Pressed = true;
                break;
            case Qt::Key_2:
                m_2Pressed = true;
                break;
            case Qt::Key_3:
                m_3Pressed = true;
                break;
            default:
                break;
        }

        if (ctrlPressed) {
            switch (key) {
                case Qt::Key_Exclam:        // Shift 1
                    emit ctrlShift1();
                    return true;
                case Qt::Key_At:            // Shift 2
                    emit ctrlShift2();
                    return true;
                case Qt::Key_NumberSign:    // Shift 3
                    emit ctrlShift3();
                    return true;
                case Qt::Key_Up:
                    if (m_1Pressed) {
                        emit ctrl1Up();
                    }
                    if (m_2Pressed) {
                        emit ctrl2Up();
                    }
                    if (m_3Pressed) {
                        emit ctrl3Up();
                    }
                    if (m_1Pressed || m_2Pressed || m_3Pressed) {
                        return true;
                    }
                    break;
                case Qt::Key_Down:
                    if (m_1Pressed) {
                        emit ctrl1Down();
                    }
                    if (m_2Pressed) {
                        emit ctrl2Down();
                    }
                    if (m_3Pressed) {
                        emit ctrl3Down();
                    }
                    if (m_1Pressed || m_2Pressed || m_3Pressed) {
                        return true;
                    }
                    break;
                case Qt::Key_Left:
                    if (m_1Pressed) {
                        emit ctrl1Left();
                    }
                    if (m_2Pressed) {
                        emit ctrl2Left();
                    }
                    if (m_3Pressed) {
                        emit ctrl3Left();
                    }
                    if (m_1Pressed || m_2Pressed || m_3Pressed) {
                        return true;
                    }
                    break;
                case Qt::Key_Right:
                    if (m_1Pressed) {
                        emit ctrl1Right();
                    }
                    if (m_2Pressed) {
                        emit ctrl2Right();
                    }
                    if (m_3Pressed) {
                        emit ctrl3Right();
                    }
                    if (m_1Pressed || m_2Pressed || m_3Pressed) {
                        return true;
                    }
                    break;
                default:
                    break;
            }
        }
    } else if (event->type() == QEvent::KeyRelease) {
        const QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        switch (keyEvent->key()) {
            case Qt::Key_1:
                m_1Pressed = false;
                break;
            case Qt::Key_2:
                m_2Pressed = false;
                break;
            case Qt::Key_3:
                m_3Pressed = false;
                break;
            default:
                break;
        }
    }

    return QObject::eventFilter(obj, event);
}
