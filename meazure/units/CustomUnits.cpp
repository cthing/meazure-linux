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

#include "CustomUnits.h"


CustomUnits::CustomUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(CustomId, "custom", screenInfoProvider),
        m_scaleBasis(k_defScaleBasis),
        m_scaleFactor(k_defScaleFactor) {
    addPrecision(0);        // XCoord
    addPrecision(0);        // YCoord
    addPrecision(0);        // Width
    addPrecision(0);        // Height
    addPrecision(1);        // Distance
    addPrecision(0);        // Area
    addPrecision(1);        // ResX
    addPrecision(1);        // ResY
}

void CustomUnits::saveProfile(Profile& profile) {
    profile.writeStr("CustomName", m_name);
    profile.writeStr("CustomAbbrev", m_abbrev);

    profile.writeStr("CustomScaleBasis", getScaleBasisStr());
    profile.writeDbl("CustomScaleFactor", m_scaleFactor);

    savePrecision(profile);
}

void CustomUnits::loadProfile(Profile& profile) {
    setName(profile.readStr("CustomName", m_name));
    setAbbrev(profile.readStr("CustomAbbrev", m_abbrev));

    setScaleBasis(profile.readStr("CustomScaleBasis", ""));
    setScaleFactor(profile.readDbl("CustomScaleFactor", m_scaleFactor));

    loadPrecision(profile);

    emit customUnitsChanged(this);
}

void CustomUnits::masterReset() {
    LinearUnits::masterReset();

    setName("");
    setAbbrev("");
    setScaleFactor(k_defScaleFactor);
    setScaleBasis(k_defScaleBasis);

    emit customUnitsChanged(this);
}

bool CustomUnits::isResRequired() const {
    return (m_scaleBasis != PixelBasis);
}

QSizeF CustomUnits::fromPixels(const QSizeF& res) const {
    QSizeF from;

    switch (m_scaleBasis) {
        default:
        case PixelBasis:
            from.setWidth(1.0 / m_scaleFactor);
            from.setHeight(1.0 / m_scaleFactor);
            break;
        case InchBasis:
            from.setWidth(1.0 / (res.width() * m_scaleFactor));
            from.setHeight(1.0 / (res.height() * m_scaleFactor));
            break;
        case CentimeterBasis:
            from.setWidth(2.54 / (res.width() * m_scaleFactor));
            from.setHeight(2.54 / (res.height() * m_scaleFactor));
            break;
    }

    return from;
}

void CustomUnits::setName(const QString& name) {
    m_name = name.trimmed();

    emit customUnitsChanged(this);
}

void CustomUnits::setAbbrev(const QString& abbrev) {
    m_abbrev = abbrev.trimmed();

    emit customUnitsChanged(this);
}

QString CustomUnits::getLengthLabel() const {
    return m_abbrev;
}

QString CustomUnits::getScaleBasisStr() const {
    switch (m_scaleBasis) {
        case PixelBasis:
            return "px";
        case InchBasis:
            return "in";
        case CentimeterBasis:
            return "cm";
    }

    return "";
}

void CustomUnits::setScaleBasis(const QString& scaleBasisStr) {
    if (scaleBasisStr == "px") {
        setScaleBasis(PixelBasis);
    } else if (scaleBasisStr == "in") {
        setScaleBasis(InchBasis);
    } else if (scaleBasisStr == "cm") {
        setScaleBasis(CentimeterBasis);
    }
}

void CustomUnits::setScaleBasis(ScaleBasis scaleBasis) {
    m_scaleBasis = scaleBasis;

    emit customUnitsChanged(this);
}

void CustomUnits::setScaleFactor(double scaleFactor) {
    m_scaleFactor = scaleFactor;

    emit customUnitsChanged(this);
}
