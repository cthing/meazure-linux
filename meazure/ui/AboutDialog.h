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

#include <meazure/environment/ScreenInfoProvider.h>
#include <QDialog>
#include <QWidget>
#include <QString>


class AboutDialog : public QDialog {

    Q_OBJECT

public:
    explicit AboutDialog(const ScreenInfoProvider* screenInfo, QWidget* parent);

private slots:
    static void copy();

private:
    static constexpr int k_vspacing { 10 };
    static constexpr int k_hspacing { 20 };
    static constexpr int k_logoSize { 32 };
    static constexpr int k_cthingSize { 70 };

    static QString getInfo();
};
