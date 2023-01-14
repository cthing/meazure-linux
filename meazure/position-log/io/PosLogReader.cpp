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

#include "PosLogReader.h"
#include <meazure/position-log/model/PosLogCustomUnits.h>
#include <QResource>
#include <QDateTime>
#include <QPointF>
#include <QSizeF>
#include <xercesc/framework/MemBufInputSource.hpp>


const PosLogReader::ElementMap PosLogReader::elementMap {
        { k_angleElem, ElementId::angle },
        { k_areaElem, ElementId::area },
        { k_createdElem, ElementId::created },
        { k_customUnitsElem, ElementId::customUnits },
        { k_descElem, ElementId::desc },
        { k_desktopElem, ElementId::desktop },
        { k_desktopsElem, ElementId::desktops },
        { k_displayPrecisionElem, ElementId::displayPrecision },
        { k_displayPrecisionsElem, ElementId::displayPrecisions },
        { k_distanceElem, ElementId::distance },
        { k_generatorElem, ElementId::generator },
        { k_heightElem, ElementId::height },
        { k_infoElem, ElementId::info },
        { k_machineElem, ElementId::machine },
        { k_measurementElem, ElementId::measurement },
        { k_originElem, ElementId::origin },
        { k_pointElem, ElementId::point },
        { k_pointsElem, ElementId::points },
        { k_positionElem, ElementId::position },
        { k_positionLogElem, ElementId::positionLog },
        { k_positionsElem, ElementId::positions },
        { k_propertiesElem, ElementId::properties },
        { k_rectElem, ElementId::rect },
        { k_resolutionElem, ElementId::resolution },
        { k_screenElem, ElementId::screen },
        { k_screensElem, ElementId::screens },
        { k_sizeElem, ElementId::size },
        { k_titleElem, ElementId::title },
        { k_unitsElem, ElementId::units },
        { k_widthElem, ElementId::width }
};

const PosLogReader::AttrMap PosLogReader::attrMap {
        { k_abbrevAttr, AttrId::abbrev },
        { k_angleAttr, AttrId::angle },
        { k_bottomAttr, AttrId::bottom },
        { k_buildAttr, AttrId::build },
        { k_dateAttr, AttrId::date },
        { k_decimalPlacesAttr, AttrId::decimalPlaces },
        { k_descAttr, AttrId::desc },
        { k_desktopRefAttr, AttrId::desktopRef },
        { k_idAttr, AttrId::id },
        { k_invertYAttr, AttrId::invertY },
        { k_leftAttr, AttrId::left },
        { k_lengthAttr, AttrId::length },
        { k_manualAttr, AttrId::manual },
        { k_nameAttr, AttrId::name },
        { k_oneAttr, AttrId::one },
        { k_primaryAttr, AttrId::primary },
        { k_rightAttr, AttrId::right },
        { k_scaleBasisAttr, AttrId::scaleBasis },
        { k_scaleFactorAttr, AttrId::scaleFactor},
        { k_toolAttr, AttrId::tool },
        { k_topAttr, AttrId::top },
        { k_twoAttr, AttrId::two },
        { k_unitsAttr, AttrId::units },
        { k_vAttr, AttrId::v },
        { k_valueAttr, AttrId::value },
        { k_versionAttr, AttrId::version },
        { k_xAttr, AttrId::x },
        { k_xoffsetAttr, AttrId::xoffset },
        { k_yAttr, AttrId::y },
        { k_yoffsetAttr, AttrId::yoffset }
};

const PosLogReader::LinearMesurementMap PosLogReader::linearMesurementMap {
        { "x", LinearMeasurementId::XCoord },
        { "y", LinearMeasurementId::YCoord },
        { "w", LinearMeasurementId::Width },
        { "h", LinearMeasurementId::Height },
        { "d", LinearMeasurementId::Distance },
        { "area", LinearMeasurementId::Area },
        { "rx", LinearMeasurementId::ResX },
        { "ry", LinearMeasurementId::ResY },
};


PosLogReader::PosLogReader(const UnitsProvider& unitsProvider) :
        PosLogIO(unitsProvider),
        m_dtdContents(QResource(":/dtd/PositionLog1.dtd").uncompressedData()) {
}

PosLogArchiveSharedPtr PosLogReader::readFile(const QString& pathname) {
    m_pathname = pathname;
    m_archive = std::make_shared<PosLogArchive>();

    XMLParser parser(this);
    parser.parseFile(pathname);

    return m_archive;
}

PosLogArchiveSharedPtr PosLogReader::readString(const QString& content) {
    m_pathname = "[string]";
    m_archive = std::make_shared<PosLogArchive>();

    XMLParser parser(this);
    parser.parseString(content);

    return m_archive;
}

xercesc::InputSource* PosLogReader::resolveEntity(const QString& systemId) {
    if (systemId == k_dtdUrl || systemId == k_dtdUrlOld) {
        return new xercesc::MemBufInputSource(reinterpret_cast<const XMLByte*>(m_dtdContents.constData()),
                                              m_dtdContents.size(), "XMLBuf");
    }

    return XMLParserHandler::resolveEntity(systemId);
}

QString PosLogReader::getFilePathname() {
    return m_pathname;
}

void PosLogReader::startElement(const QString &, const QString &elementName, const XMLAttributes &attrs) {
    const ElementId elementId = elementMap.at(elementName);

    switch(elementId) {
        case ElementId::angle: {
            double angle = 0.0;
            attrs.getValueDbl(k_valueAttr, angle);

            PosLogToolData toolData = m_currentPosition->getToolData();
            toolData.setAngle(angle);
            m_currentPosition->setToolData(toolData);
            m_currentPosition->addToolTrait(RadioToolTrait::AngleAvailable);
            break;
        }
        case ElementId::area: {
            double area = 0.0;
            attrs.getValueDbl(k_valueAttr, area);

            PosLogToolData toolData = m_currentPosition->getToolData();
            toolData.setArea(area);
            m_currentPosition->setToolData(toolData);
            m_currentPosition->addToolTrait(RadioToolTrait::AreaAvailable);
            break;
        }
        case ElementId::created:
            attrs.getValueStr(k_dateAttr, [this](const QString& timestamp) {
                m_currentInfo->setCreated(QDateTime::fromString(timestamp, Qt::ISODate));
            });
            break;
        case ElementId::customUnits:
            m_currentCustomUnits = std::make_unique<PosLogCustomUnits>();
            attrs.getValueStr(k_nameAttr, [this](const QString& name) { m_currentCustomUnits->setName(name); });
            attrs.getValueStr(k_abbrevAttr, [this](const QString& abbrev) { m_currentCustomUnits->setAbbrev(abbrev); });
            attrs.getValueStr(k_scaleBasisAttr, [this](const QString& customUnitsStr) {
                m_currentCustomUnits->setScaleBasisStr(customUnitsStr);
            });
            attrs.getValueDbl(k_scaleFactorAttr, [this](double factor) {
                m_currentCustomUnits->setScaleFactor(factor);
            });
            break;
        case ElementId::desc:
            m_characters.clear();
            break;
        case ElementId::desktop: {
            QString desktopId;
            attrs.getValueStr(k_idAttr, desktopId);
            m_currentDesktop = m_archive->getOrCreateDesktop(desktopId);
            break;
        }
        case ElementId::distance: {
            double distance = 0.0;
            attrs.getValueDbl(k_valueAttr, distance);

            PosLogToolData toolData = m_currentPosition->getToolData();
            toolData.setDistance(distance);
            m_currentPosition->setToolData(toolData);
            m_currentPosition->addToolTrait(RadioToolTrait::DistAvailable);
            break;
        }
        case ElementId::generator:
            attrs.getValueStr(k_nameAttr, [this](const QString& name) { m_currentInfo->setAppName(name); });
            attrs.getValueStr(k_versionAttr, [this](const QString& version) { m_currentInfo->setAppVersion(version); });
            attrs.getValueStr(k_buildAttr, [this](const QString& build) { m_currentInfo->setAppBuild(build); });
            break;
        case ElementId::height: {
            double height = 0.0;
            attrs.getValueDbl(k_valueAttr, height);

            PosLogToolData toolData = m_currentPosition->getToolData();
            QSizeF widthHeight = toolData.getWidthHeight();
            widthHeight.setHeight(height);
            toolData.setWidthHeight(widthHeight);
            m_currentPosition->setToolData(toolData);
            m_currentPosition->addToolTrait(RadioToolTrait::WHAvailable);
            break;
        }
        case ElementId::info:
            m_currentInfo = std::make_unique<PosLogInfo>();
            break;
        case ElementId::machine:
            attrs.getValueStr(k_nameAttr, [this](const QString& name) { m_currentInfo->setMachineName(name); });
            break;
        case ElementId::measurement: {
            QString measurementIdStr;
            int decimalPlaces = 0;
            attrs.getValueStr(k_nameAttr, measurementIdStr);
            attrs.getValueInt(k_decimalPlacesAttr, decimalPlaces);
            const LinearMeasurementId measurementId = linearMesurementMap.at(measurementIdStr);
            m_currentCustomUnits->setDisplayPrecision(measurementId, decimalPlaces);
            break;
        }
        case ElementId::origin: {
            QPointF origin;
            attrs.getValueDbl(k_xoffsetAttr, [&origin](double xoffset) { origin.setX(xoffset); });
            attrs.getValueDbl(k_yoffsetAttr, [&origin](double yoffset) { origin.setY(yoffset); });
            m_currentDesktop->setOrigin(origin);

            attrs.getValueBool(k_invertYAttr, [this](bool invertY) { m_currentDesktop->setInvertY(invertY); });
            break;
        }
        case ElementId::point: {
            QString pointName;
            attrs.getValueStr(k_nameAttr, pointName);

            QPointF point;
            attrs.getValueDbl(k_xAttr, [&point](double x) { point.setX(x); });
            attrs.getValueDbl(k_yAttr, [&point](double y) { point.setY(y); });

            PosLogToolData toolData = m_currentPosition->getToolData();
            RadioToolTraits toolTraits = m_currentPosition->getToolTraits();

            if (pointName == k_oneAttr) {
                toolData.setPoint1(point);
                toolTraits |= RadioToolTrait::XY1Available;
            } else if (pointName == k_twoAttr) {
                toolData.setPoint2(point);
                toolTraits |= RadioToolTrait::XY2Available;
            } else if (pointName == k_vAttr) {
                toolData.setPointV(point);
                toolTraits |= RadioToolTrait::XYVAvailable;
            }

            m_currentPosition->setToolData(toolData);
            m_currentPosition->setToolTraits(toolTraits);
            break;
        }
        case ElementId::position: {
            m_currentPosition = std::make_unique<PosLogPosition>();
            attrs.getValueStr(k_dateAttr, [this](const QString& timestamp) {
                m_currentPosition->setRecorded(QDateTime::fromString(timestamp, Qt::ISODate));
            });
            attrs.getValueStr(k_toolAttr, [this](const QString& toolName) {
                m_currentPosition->setToolName(toolName);
            });

            QString desktopId;
            attrs.getValueStr(k_desktopRefAttr, desktopId);
            m_currentPosition->setDesktop(m_archive->getOrCreateDesktop(desktopId));
            break;
        }
        case ElementId::positionLog:
            attrs.getValueInt(k_versionAttr, [this](int version) { m_archive->setVersion(version); });
            break;
        case ElementId::rect: {
            QRectF screenRect;
            attrs.getValueDbl(k_topAttr, [&screenRect](double top) { screenRect.setTop(top); });
            attrs.getValueDbl(k_bottomAttr, [&screenRect](double bottom) { screenRect.setBottom(bottom); });
            attrs.getValueDbl(k_leftAttr, [&screenRect](double left) { screenRect.setLeft(left); });
            attrs.getValueDbl(k_rightAttr, [&screenRect](double right) { screenRect.setRight(right); });
            m_currentScreen->setRect(screenRect);
            break;
        }
        case ElementId::resolution: {
            QSizeF screenRes;
            attrs.getValueDbl(k_xAttr, [&screenRes](double x) { screenRes.setWidth(x); });
            attrs.getValueDbl(k_yAttr, [&screenRes](double y) { screenRes.setHeight(y); });
            m_currentScreen->setRes(screenRes);

            attrs.getValueBool(k_manualAttr, [this](bool manual) { m_currentScreen->setManualRes(manual); });
            break;
        }
        case ElementId::screen:
            m_currentScreen = std::make_unique<PosLogScreen>();
            attrs.getValueStr(k_descAttr, [this](const QString& desc) { m_currentScreen->setDescription(desc); });
            attrs.getValueBool(k_primaryAttr, [this](bool primary) { m_currentScreen->setPrimary(primary); });
            break;
        case ElementId::screens:
            break;
        case ElementId::size: {
            QSizeF size;
            attrs.getValueDbl(k_xAttr, [&size](double x) { size.setWidth(x); });
            attrs.getValueDbl(k_yAttr, [&size](double y) { size.setHeight(y); });
            m_currentDesktop->setSize(size);
            break;
        }
        case ElementId::title:
            m_characters.clear();
            break;
        case ElementId::units:
            attrs.getValueStr(k_lengthAttr, [this](const QString& unitsStr) {
                m_currentDesktop->setLinearUnitsId(m_units.getLinearUnits(unitsStr)->getUnitsId());
            });
            attrs.getValueStr(k_angleAttr, [this](const QString& unitsStr) {
                m_currentDesktop->setAngularUnitsId(m_units.getAngularUnits(unitsStr)->getUnitsId());
            });
            break;
        case ElementId::width: {
            double width = 0.0;
            attrs.getValueDbl(k_valueAttr, width);

            PosLogToolData toolData = m_currentPosition->getToolData();
            QSizeF widthHeight = toolData.getWidthHeight();
            widthHeight.setWidth(width);
            toolData.setWidthHeight(widthHeight);
            m_currentPosition->setToolData(toolData);
            m_currentPosition->addToolTrait(RadioToolTrait::WHAvailable);
            break;
        }
        default:
            break;
    }
}

void PosLogReader::endElement(const QString &container, const QString &elementName) {
    const ElementId elementId = elementMap.at(elementName);
    const ElementId containerId = container.isEmpty() ? ElementId::positionLog : elementMap.at(container);

    switch(elementId) {
        case ElementId::desc:
            if (containerId == ElementId::info) {
                m_currentInfo->setDescription(m_characters);
            } else {
                m_currentPosition->setDescription(m_characters);
            }
            break;
        case ElementId::desktop:
            if (m_currentCustomUnits) {
                m_currentDesktop->setCustomUnits(*m_currentCustomUnits);
            }
            m_archive->addDesktop(m_currentDesktop);
            break;
        case ElementId::info:
            m_archive->setInfo(*m_currentInfo);
            break;
        case ElementId::position:
            m_archive->addPosition(*m_currentPosition);
            break;
        case ElementId::screen:
            m_currentDesktop->addScreen(*m_currentScreen);
            break;
        case ElementId::title:
            m_currentInfo->setTitle(m_characters);
            break;
        default:
            break;
    }
}

void PosLogReader::characterData(const QString &, const QString &data) {
    m_characters += data;
}
