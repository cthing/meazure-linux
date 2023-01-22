/*
 * Copyright 2023 C Thing Software
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

#include "PosLogIO.h"
#include <meazure/position-log/model/PosLogArchive.h>
#include <meazure/position-log/model/PosLogInfo.h>
#include <meazure/position-log/model/PosLogDesktop.h>
#include <meazure/position-log/model/PosLogPosition.h>
#include <meazure/position-log/model/PosLogScreen.h>
#include <meazure/position-log/model/PosLogToolData.h>
#include <QByteArray>
#include <QString>
#include <meazure/xml/XMLParser.h>
#include <memory>
#include <map>


/// Reads a position log XML file.
///
class PosLogReader : public PosLogIO, public XMLParserHandler {

public:
    explicit PosLogReader(const UnitsProvider& unitsProvider);

    PosLogArchiveSharedPtr readFile(const QString& pathname);

    PosLogArchiveSharedPtr readString(const QString& content);

    void startElement(const QString &container, const QString &elementName, const XMLAttributes &attrs) override;

    void endElement(const QString &container, const QString &elementName) override;

    void characterData(const QString &container, const QString &data) override;

    xercesc::InputSource* resolveEntity(const QString& systemId) override;

    QString getFilePathname() override;

private:
    enum class ElementId {
        angle,
        area,
        created,
        customUnits,
        desc,
        desktop,
        desktops,
        displayPrecision,
        displayPrecisions,
        distance,
        generator,
        height,
        info,
        machine,
        measurement,
        origin,
        point,
        points,
        position,
        positionLog,
        positions,
        properties,
        rect,
        resolution,
        screen,
        screens,
        size,
        title,
        units,
        width
    };

    enum class AttrId {
        abbrev,
        angle,
        bottom,
        build,
        date,
        decimalPlaces,
        desc,
        desktopRef,
        id,
        invertY,
        left,
        length,
        manual,
        name,
        one,
        primary,
        right,
        scaleBasis,
        scaleFactor,
        tool,
        top,
        two,
        units,
        v,
        value,
        version,
        x,
        xoffset,
        y,
        yoffset
    };

    using ElementMap = std::map<QString, ElementId>;
    using AttrMap = std::map<QString, AttrId>;
    using LinearMesurementMap = std::map<QString, LinearMeasurementId>;

    using InfoUniquePtr = std::unique_ptr<PosLogInfo>;
    using ScreenUniquePtr = std::unique_ptr<PosLogScreen>;
    using CustomUnitsUniquePtr = std::unique_ptr<PosLogCustomUnits>;
    using PositionUniquePtr = std::unique_ptr<PosLogPosition>;

    static const ElementMap elementMap ;
    static const LinearMesurementMap linearMesurementMap;

    PosLogArchiveSharedPtr m_archive;
    QString m_pathname;
    QByteArray m_dtdContents;
    QString m_characters;
    InfoUniquePtr m_currentInfo;
    PosLogDesktopSharedPtr m_currentDesktop;
    CustomUnitsUniquePtr m_currentCustomUnits;
    ScreenUniquePtr m_currentScreen;
    PositionUniquePtr m_currentPosition;
};
