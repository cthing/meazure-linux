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

#include <QApplication>
#include <QPushButton>
#include <QtPlugin>
#include <QWindow>
#include <QScreen>

Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();

    QWindow w(static_cast<QScreen*>(nullptr));
    Qt::WindowFlags flags = w.flags();
    w.setFlags(flags | Qt::FramelessWindowHint);
    w.setGeometry(300, 300, 100, 100);
    w.show();

    return QApplication::exec();
}
