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

#include "UnitsMgr.h"
#include <meazure/utils/StringUtils.h>
#include <QPoint>
#include <QSizeF>
#include <cmath>


UnitsMgr::UnitsMgr(const ScreenInfoProvider* screenInfoProvider) :
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

    connect(&m_customUnits, SIGNAL(customUnitsChanged()), this, SIGNAL(customUnitsChanged()));
}

void UnitsMgr::writeConfig(Config& config) const {
    config.writeStr("LinearUnits", getLinearUnitsStr());
    config.writeStr("AngularUnits", getAngularUnitsStr());
    config.writeBool("InvertY", isInvertY());
    config.writeBool("SupplementalAngle", isSupplementalAngle());

    const QPointF pt = convertPos(getOrigin());
    config.writeStr("OriginX", StringUtils::dblToStr(pt.x()));
    config.writeStr("OriginY", StringUtils::dblToStr(pt.y()));

    config.writeBool("HaveWarned", m_haveWarned);

    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->writeConfig(config);
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->writeConfig(config);
    }
}

void UnitsMgr::readConfig(const Config& config) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->readConfig(config);
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->readConfig(config);
    }

    m_haveWarned = config.readBool("HaveWarned", m_haveWarned);

    if (config.getVersion() == 1) {
        setLinearUnits(static_cast<LinearUnitsId>(config.readInt("Units", PixelsId)));
    } else {
        setLinearUnits(config.readStr("LinearUnits", k_defLinearUnits));
    }
    setAngularUnits(config.readStr("AngularUnits", k_defAngularUnits));
    setInvertY(config.readBool("InvertY", k_defInvertY));
    setSupplementalAngle(config.readBool("SupplementalAngle", k_defSupplementalAngle));

    const QPointF pt(config.readDbl("OriginX", 0.0), config.readDbl("OriginY", 0.0));
    setOrigin(unconvertPos(pt));
}

void UnitsMgr::hardReset() {
    m_haveWarned = k_defHaveWarned;

    setLinearUnits(k_defLinearUnits);
    setAngularUnits(k_defAngularUnits);

    setInvertY(k_defInvertY);
    setSupplementalAngle(k_defSupplementalAngle);
    setOrigin(QPoint());

    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->hardReset();
    }

    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->hardReset();
    }

    emit precisionsChanged();
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
            && (m_screenInfoProvider->sizeChanged()
                || (!m_haveWarned && m_screenInfoProvider->isCalibrationRequired()))) {
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

void UnitsMgr::setOrigin(const QPoint& origin) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->setOrigin(origin);
    }

    emit originChanged(origin);
}

QPoint UnitsMgr::getOrigin() const {
    return m_inchUnits.getOrigin();
}

void UnitsMgr::setInvertY(bool invertY) {
    for (const auto& unitsEntry : m_linearUnitsMap) {
        unitsEntry.second->setInvertY(invertY);
    }

    emit invertYChanged(invertY);
}

bool UnitsMgr::isInvertY() const {
    return m_inchUnits.isInvertY();
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

void UnitsMgr::setSupplementalAngle(bool showSupplemental) {
    for (const auto& unitsEntry : m_angularUnitsMap) {
        unitsEntry.second->setSupplementalAngle(showSupplemental);
    }

    emit supplementalAngleChanged(showSupplemental);
}

bool UnitsMgr::isSupplementalAngle() const {
    return m_degreeUnits.isSupplementalAngle();
}

QSizeF UnitsMgr::getWidthHeight(const QPoint& p1, const QPoint& p2) const {
    const int screenIndex1 = m_screenInfoProvider->screenForPoint(p1);
    const QSizeF from1 = m_currentLinearUnits->fromPixels(m_screenInfoProvider->getScreenRes(screenIndex1));

    const int screenIndex2 = m_screenInfoProvider->screenForPoint(p2);
    const QSizeF from2 = m_currentLinearUnits->fromPixels(m_screenInfoProvider->getScreenRes(screenIndex2));

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
    const int screenIndex = m_screenInfoProvider->screenForRect(rect);
    const QSizeF res = m_screenInfoProvider->getScreenRes(screenIndex);
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
