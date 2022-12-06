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

#include "UnitsMgr.h"
#include <meazure/utils/StringUtils.h>
#include <QPoint>
#include <QSizeF>
#include <cmath>


UnitsMgr::UnitsMgr(const ScreenInfoProvider& screenInfoProvider) :
        m_screenInfoProvider(screenInfoProvider),
        m_pixelUnits(screenInfoProvider),
        m_pointUnits(screenInfoProvider),
        m_twipUnits(screenInfoProvider),
        m_inchUnits(screenInfoProvider),
        m_cmUnits(screenInfoProvider),
        m_mmUnits(screenInfoProvider),
        m_picaUnits(screenInfoProvider),
        m_customUnits(screenInfoProvider),
        m_currentLinearUnits(&m_pixelUnits),
        m_currentAngularUnits(&m_degreeUnits) {
    m_linearUnitsMap[m_pixelUnits.getUnitsId()] = &m_pixelUnits;
    m_linearUnitsMap[m_pointUnits.getUnitsId()] = &m_pointUnits;
    m_linearUnitsMap[m_twipUnits.getUnitsId()] = &m_twipUnits;
    m_linearUnitsMap[m_inchUnits.getUnitsId()] = &m_inchUnits;
    m_linearUnitsMap[m_cmUnits.getUnitsId()] = &m_cmUnits;
    m_linearUnitsMap[m_mmUnits.getUnitsId()] = &m_mmUnits;
    m_linearUnitsMap[m_picaUnits.getUnitsId()] = &m_picaUnits;
    m_linearUnitsMap[m_customUnits.getUnitsId()] = &m_customUnits;

    m_angularUnitsMap[m_degreeUnits.getUnitsId()] = &m_degreeUnits;
    m_angularUnitsMap[m_radianUnits.getUnitsId()] = &m_radianUnits;
}

void UnitsMgr::saveProfile(Profile& profile) const {
    profile.writeStr("LinearUnits", getLinearUnitsStr());
    profile.writeStr("AngularUnits", getAngularUnitsStr());
    profile.writeBool("InvertY", isInvertY());
    profile.writeBool("SupplementalAngle", isSupplementalAngle());

    const QPointF pt = convertPos(getOrigin());
    profile.writeStr("OriginX", StringUtils::dblToStr(pt.x()));
    profile.writeStr("OriginY", StringUtils::dblToStr(pt.y()));

    if (!profile.userInitiated()) {
        profile.writeBool("HaveWarned", m_haveWarned);
    }

    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->saveProfile(profile);
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->saveProfile(profile);
    }
}

void UnitsMgr::loadProfile(Profile& profile) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->loadProfile(profile);
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->loadProfile(profile);
    }

    if (!profile.userInitiated()) {
        m_haveWarned = profile.readBool("HaveWarned", m_haveWarned);
    }

    if (profile.getVersion() == 1) {
        setLinearUnits(static_cast<LinearUnitsId>(profile.readInt("Units", PixelsId)));
    } else {
        setLinearUnits(profile.readStr("LinearUnits", k_defLinearUnits));
    }
    setAngularUnits(profile.readStr("AngularUnits", k_defAngularUnits));
    setInvertY(profile.readBool("InvertY", k_defInvertY));
    setSupplementalAngle(profile.readBool("SupplementalAngle", k_defSupplmentalAngle));

    const QPointF pt(profile.readDbl("OriginX", 0.0), profile.readDbl("OriginY", 0.0));
    setOrigin(unconvertPos(pt));
}

void UnitsMgr::masterReset() {
    m_haveWarned = k_defHaveWarned;

    setLinearUnits(k_defLinearUnits);
    setAngularUnits(k_defAngularUnits);

    setInvertY(k_defInvertY);
    setSupplementalAngle(k_defSupplmentalAngle);
    setOrigin(QPoint());

    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->masterReset();
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->masterReset();
    }
}

void UnitsMgr::setLinearUnits(LinearUnitsId unitsId) {
    m_currentLinearUnits = (*m_linearUnitsMap.find(unitsId)).second;

    emit linearUnitsChanged(unitsId);

    // Determine whether we need to warn the user to calibrate the screen resolution. The criteria
    // for warning the user is:
    //
    // - Resolution is not already set manually
    // - User has not already been warned
    // - The screen size has changed since last run
    // - The current units require use of the resolution
    //
    if (m_currentLinearUnits->isResRequired()
            && (m_screenInfoProvider.sizeChanged() || (!m_haveWarned && m_screenInfoProvider.isCalibrationRequired()))) {
        m_haveWarned = true;
        emit calibrationRequired();
    }
}

void UnitsMgr::setLinearUnits(const QString& unitsStr) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        if (unitsEntry.second->getUnitsStr() == unitsStr) {
            setLinearUnits(unitsEntry.first);
        }
    }
}

void UnitsMgr::setAngularUnits(AngularUnitsId unitsId) {
    m_currentAngularUnits = (*m_angularUnitsMap.find(unitsId)).second;

    emit angularUnitsChanged(unitsId);
}

void UnitsMgr::setAngularUnits(const QString& unitsStr) {
    for (const auto& unitsEntry : m_angularUnitsMap) {
        if (unitsEntry.second->getUnitsStr() == unitsStr) {
            setAngularUnits(unitsEntry.first);
        }
    }
}

QSizeF UnitsMgr::getWidthHeight(const QPoint& p1, const QPoint& p2) const {
    const int screenIndex1 = m_screenInfoProvider.screenForPoint(p1);
    const QSizeF from1 = m_currentLinearUnits->fromPixels(m_screenInfoProvider.getScreenRes(screenIndex1));

    const int screenIndex2 = m_screenInfoProvider.screenForPoint(p2);
    const QSizeF from2 = m_currentLinearUnits->fromPixels(m_screenInfoProvider.getScreenRes(screenIndex2));

    QPoint np1(p1);
    QPoint np2(p2);

    if (np1.x() < np2.x()) {
        np2.rx()++;
    } else {
        np1.rx()++;
    }

    if (np1.y() < np2.y()) {
        np2.ry()++;
    } else {
        np1.ry()++;
    }

    const QPointF cp1(from1.width() * np1.x(), from1.height() * np1.y());
    const QPointF cp2(from2.width() * np2.x(), from2.height() * np2.y());

    return { fabs(cp1.x() - cp2.x()), fabs(cp1.y() - cp2.y()) };
}

QSizeF UnitsMgr::getMinorTickIncr(const QRect& rect) const {
    // We want to ensure a minimum resolution-independent
    // separation between the minor ticks. Start by converting
    // the resolution-independent minimum separation to pixels.
    //
    const int screenIndex = m_screenInfoProvider.screenForRect(rect);
    const QSizeF res = m_screenInfoProvider.getScreenRes(screenIndex);
    const QSize sepPixels = convertToPixels(InchesId, res, k_minSepInches, k_minSepPixels);

    // Convert the minimum tick separation to the current units.
    //
    const QSizeF from = m_currentLinearUnits->fromPixels(res);
    const QSizeF sepUnits(from.width() * sepPixels.width(), from.height() * sepPixels.height());

    // The object is to find a standard minor increment (e.g. 10, 25)
    // that is the closest to the minimum increment but larger than
    // or equal to it. To achieve this, use the log of the separation
    // to normalize the separation value into a standard range
    // (i.e. 10-100).
    //
    QSizeF sepPower(log10(sepUnits.width()), log10(sepUnits.height()));
    const QSizeF delta(1.0 - floor(sepPower.width()), 1.0 - floor(sepPower.height()));

    sepPower += delta;

    const int count = k_numTickIncrements - 1;

    int idx = 0;
    while ((idx < count) && (log10(k_tickIncrements[idx]) < sepPower.width())) {
        idx++;
    }
    const double incrementX = k_tickIncrements[idx] * pow(10.0, -delta.width());

    idx = 0;
    while ((idx < count) && (log10(k_tickIncrements[idx]) < sepPower.height())) {
        idx++;
    }
    const double incrementY = k_tickIncrements[idx] * pow(10.0, -delta.height());

    return { incrementX, incrementY };
}
