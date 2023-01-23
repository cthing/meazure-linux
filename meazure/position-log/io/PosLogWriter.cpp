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

#include "PosLogWriter.h"

PosLogWriter::PosLogWriter(const UnitsProvider* unitsProvider) : PosLogIO(unitsProvider) {
}

void PosLogWriter::write(std::ostream& out, const PosLogArchive& archive) {
    XMLWriter writer(out);

    writer.startDocument();
    writer.doctype(k_positionLogElem, k_dtdUrl);

    writer.startElement(k_positionLogElem).addAttribute(k_versionAttr, archive.getVersion());

    writeInfoSection(writer, archive);
    writeDesktopsSection(writer, archive);
    writePositionsSection(writer, archive);

    writer.endElement();            // positionLog
    writer.endDocument();
}

void PosLogWriter::writeInfoSection(XMLWriter& writer, const PosLogArchive& archive) {
    const PosLogInfo& info = archive.getInfo();

    writer.startElement(k_infoElem);

    writer.startElement(k_titleElem)
          .characters(info.getTitle())
          .endElement();
    writer.startElement(k_createdElem)
          .addAttribute(k_dateAttr, info.getCreated().toString(Qt::ISODate))
          .endElement();
    writer.startElement(k_generatorElem)
          .addAttribute(k_nameAttr, info.getAppName())
          .addAttribute(k_versionAttr, info.getAppVersion())
          .addAttribute(k_buildAttr, info.getAppBuild())
          .endElement();
    writer.startElement(k_machineElem)
          .addAttribute(k_nameAttr, info.getMachineName())
          .endElement();
    if (!info.getDescription().isEmpty()) {
        writer.startElement(k_descElem)
              .characters(info.getDescription())
              .endElement();
    }

    writer.endElement();    // info
}

void PosLogWriter::writeDesktopsSection(XMLWriter& writer, const PosLogArchive& archive) {
    writer.startElement(k_desktopsElem);

    for (PosLogDesktopSharedPtr desktop : archive.getDesktops()) {      // NOLINT(misc-const-correctness,performance-for-range-copy)
        writer.startElement(k_desktopElem)
              .addAttribute(k_idAttr, desktop->getId());

        writer.startElement(k_unitsElem)
              .addAttribute(k_lengthAttr, m_units->getLinearUnits(desktop->getLinearUnitsId())->getUnitsStr())
              .addAttribute(k_angleAttr, m_units->getAngularUnits(desktop->getAngularUnitsId())->getUnitsStr())
              .endElement();

        if (desktop->getLinearUnitsId() == CustomId) {
            const PosLogCustomUnits& customUnits = desktop->getCustomUnits();
            writer.startElement(k_customUnitsElem)
                  .addAttribute(k_nameAttr, customUnits.getName())
                  .addAttribute(k_abbrevAttr, customUnits.getAbbrev())
                  .addAttribute(k_scaleBasisAttr, customUnits.getScaleBasisStr())
                  .addAttribute(k_scaleFactorAttr, customUnits.getScaleFactor())
                  .endElement();
        }

        writer.startElement(k_originElem)
              .addAttribute(k_xoffsetAttr, desktop->getOrigin().x())
              .addAttribute(k_yoffsetAttr, desktop->getOrigin().y())
              .addAttribute(k_invertYAttr, (desktop->isInvertY() ? "true" : "false"))
              .endElement();

        writer.startElement(k_sizeElem)
              .addAttribute(k_xAttr, desktop->getSize().width())
              .addAttribute(k_yAttr, desktop->getSize().height())
              .endElement();

        writer.startElement(k_screensElem);
        for (const PosLogScreen& screen : desktop->getScreens()) {
            writer.startElement(k_screenElem)
                  .addAttribute(k_descAttr, screen.getDescription())
                  .addAttribute(k_primaryAttr, (screen.isPrimary() ? "true" : "false"));

            writer.startElement(k_rectElem)
                  .addAttribute(k_topAttr, screen.getRect().top())
                  .addAttribute(k_bottomAttr, screen.getRect().bottom())
                  .addAttribute(k_leftAttr, screen.getRect().left())
                  .addAttribute(k_rightAttr, screen.getRect().right())
                  .endElement();

            writer.startElement(k_resolutionElem)
                  .addAttribute(k_xAttr, screen.getRes().width())
                  .addAttribute(k_yAttr, screen.getRes().height())
                  .addAttribute(k_manualAttr, (screen.isManualRes() ? "true" : "false"))
                  .endElement();

            writer.endElement();        // screen
        }
        writer.endElement();        // screens

        if (desktop->getLinearUnitsId() == CustomId) {
            const LinearUnits* customUnits = m_units->getLinearUnits(desktop->getLinearUnitsId());

            writer.startElement(k_displayPrecisionsElem);

            writer.startElement(k_displayPrecisionElem)
                  .addAttribute(k_unitsAttr, customUnits->getUnitsStr());

            const Units::DisplayPrecisionNames& precisionNames = customUnits->getDisplayPrecisionNames();
            const Units::DisplayPrecisions  precisions = customUnits->getDisplayPrecisions();

            for (unsigned int i = 0; i < precisions.size(); i++) {
                writer.startElement(k_measurementElem)
                      .addAttribute(k_nameAttr, precisionNames[i])
                      .addAttribute(k_decimalPlacesAttr, precisions[i])
                      .endElement();
            }

            writer.endElement();        // displayPrecision

            writer.endElement();    // displayPrecisions
        }

        writer.endElement();        // desktop
    }

    writer.endElement();    // desktops
}

void PosLogWriter::writePositionsSection(XMLWriter& writer, const PosLogArchive& archive) {
    writer.startElement(k_positionsElem);

    for (const PosLogPosition& position : archive.getPositions()) {
        writer.startElement(k_positionElem)
              .addAttribute(k_desktopRefAttr, position.getDesktop()->getId())
              .addAttribute(k_toolAttr, position.getToolName())
              .addAttribute(k_dateAttr, position.getRecorded().toString(Qt::ISODate));

        if (!position.getDescription().isEmpty()) {
            writer.startElement(k_descElem)
                  .characters(position.getDescription())
                  .endElement();
        }

        const RadioToolTraits toolTraits = position.getToolTraits();
        const PosLogToolData toolData = position.getToolData();

        auto writePoint = [toolTraits, &writer](RadioToolTrait trait, const char* pointName, const QPointF& point) {
            if ((toolTraits & trait) != 0) {
                writer.startElement(k_pointElem)
                      .addAttribute(k_nameAttr, pointName)
                      .addAttribute(k_xAttr, point.x())
                      .addAttribute(k_yAttr, point.y())
                      .endElement();
            }
        };

        writer.startElement(k_pointsElem);
        writePoint(RadioToolTrait::XY1Available, k_oneAttr, toolData.getPoint1());
        writePoint(RadioToolTrait::XY2Available, k_twoAttr, toolData.getPoint2());
        writePoint(RadioToolTrait::XYVAvailable, k_vAttr, toolData.getPointV());
        writer.endElement();        // points

        auto writeProperty = [toolTraits, &writer](RadioToolTrait trait, const char* propertyName, double value) {
            if ((toolTraits & trait) != 0) {
                writer.startElement(propertyName)
                      .addAttribute(k_valueAttr, value)
                      .endElement();
            }
        };

        writer.startElement(k_propertiesElem);
        writeProperty(RadioToolTrait::WHAvailable, k_widthElem, toolData.getWidthHeight().width());
        writeProperty(RadioToolTrait::WHAvailable, k_heightElem, toolData.getWidthHeight().height());
        writeProperty(RadioToolTrait::DistAvailable, k_distanceElem, toolData.getDistance());
        writeProperty(RadioToolTrait::AreaAvailable, k_areaElem, toolData.getArea());
        writeProperty(RadioToolTrait::AngleAvailable, k_angleElem, toolData.getAngle());
        writer.endElement();        // properties

        writer.endElement();        // position
    }

    writer.endElement();    // positions
}
