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

#include "PrefsPageId.h"
#include <meazure/environment/ScreenInfo.h>
#include <meazure/units/UnitsMgr.h>
#include <QWidget>
#include <QString>


/// Base class for preferences pages.
///
class PrefsPage : public QWidget {

    Q_OBJECT

public:
    virtual PrefsPageId getId() = 0;
    virtual QString getName() = 0;

    virtual void initialize() = 0;
    virtual void apply() = 0;

    [[nodiscard]] virtual bool isDirty() const = 0;

signals:
    void dirtyChanged(bool dirty);
    void pageRequested(PrefsPageId pageId);

protected:
    PrefsPage(const ScreenInfo* screenInfo, const UnitsMgr* unitsMgr) :
            m_screenInfo(screenInfo),
            m_unitsMgr(unitsMgr) {
    }

    const ScreenInfo* m_screenInfo;
    const UnitsMgr* m_unitsMgr;
};
