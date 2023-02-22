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

#pragma once

#include <QObject>


class GlobalShortcuts : public QObject {

    Q_OBJECT

public:
    explicit GlobalShortcuts(QObject* parent);

signals:
    void ctrl1Up();
    void ctrl1Down();
    void ctrl1Left();
    void ctrl1Right();

    void ctrl2Up();
    void ctrl2Down();
    void ctrl2Left();
    void ctrl2Right();

    void ctrl3Up();
    void ctrl3Down();
    void ctrl3Left();
    void ctrl3Right();

    void ctrlShift1();
    void ctrlShift2();
    void ctrlShift3();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    bool m_1Pressed { false };
    bool m_2Pressed { false };
    bool m_3Pressed { false };
};
