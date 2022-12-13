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
                        return true;
                    }
                    if (m_2Pressed) {
                        emit ctrl2Up();
                        return true;
                    }
                    if (m_3Pressed) {
                        emit ctrl3Up();
                        return true;
                    }
                    break;
                case Qt::Key_Down:
                    if (m_1Pressed) {
                        emit ctrl1Down();
                        return true;
                    }
                    if (m_2Pressed) {
                        emit ctrl2Down();
                        return true;
                    }
                    if (m_3Pressed) {
                        emit ctrl3Down();
                        return true;
                    }
                    break;
                case Qt::Key_Left:
                    if (m_1Pressed) {
                        emit ctrl1Left();
                        return true;
                    }
                    if (m_2Pressed) {
                        emit ctrl2Left();
                        return true;
                    }
                    if (m_3Pressed) {
                        emit ctrl3Left();
                        return true;
                    }
                    break;
                case Qt::Key_Right:
                    if (m_1Pressed) {
                        emit ctrl1Right();
                        return true;
                    }
                    if (m_2Pressed) {
                        emit ctrl2Right();
                        return true;
                    }
                    if (m_3Pressed) {
                        emit ctrl3Right();
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
