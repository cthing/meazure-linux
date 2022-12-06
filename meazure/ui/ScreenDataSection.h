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

#pragma once

#include <meazure/units/Units.h>
#include <meazure/units/UnitsMgr.h>
#include <meazure/environment/ScreenInfoProvider.h>
#include "DataField.h"
#include <QGroupBox>
#include <QLabel>
#include <QPointF>
#include <QPoint>


/// Presents the display screen information.
///
class ScreenDataSection : public QGroupBox {

    Q_OBJECT

public:
    ScreenDataSection();

private slots:
    void linearUnitsChanged(LinearUnitsId unitsId);
    void update(QPointF coord, QPoint rawPos);

private:
    static constexpr int k_fieldWidth { 7 };

    /// Creates the text fields that provide the screen information display.
    ///
    void createFields();

    void refresh();

    const ScreenInfoProvider& m_screenInfo;
    const UnitsMgr& m_unitsMgr;
    int m_currentScreenIdx { -1 };
    DataField* m_wField;
    DataField* m_hField;
    DataField* m_rxField;
    DataField* m_ryField;
    QLabel* m_screenName;
    QLabel* m_hUnits;
    QLabel* m_ryUnits;
};
