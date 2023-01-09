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

PosLogWriter::PosLogWriter(const UnitsProvider& unitsProvider) : PosLogIO(unitsProvider) {
}

void PosLogWriter::write(std::ostream& out, const PosLogArchive& archive) {
    XMLWriter writer(out);

    writer.startDocument();
    writer.doctype("positionLog", k_dtdUrl);

    writer.startElement("positionLog").addAttribute("version", k_majorVersion);

    writeInfoSection(writer, archive);
    writeDesktopsSection(writer, archive);
    writePositionsSection(writer, archive);

    writer.endElement();            // positionLog
    writer.endDocument();
}

void PosLogWriter::writeInfoSection(XMLWriter& writer, const PosLogArchive& archive) {
    const PosLogInfo& info = archive.getInfo();

    writer.startElement("info");

    writer.startElement("title")
          .characters(info.getTitle())
          .endElement();
    writer.startElement("created")
          .addAttribute("date", info.getCreated().toString(Qt::ISODate))
          .endElement();
    writer.startElement("generator")
          .addAttribute("name", info.getAppName())
          .addAttribute("version", info.getAppVersion())
          .addAttribute("build", info.getAppBuild())
          .endElement();
    writer.startElement("machine")
          .addAttribute("name", info.getMachineName())
          .endElement();
    if (!info.getDescription().isEmpty()) {
        writer.startElement("desc")
              .characters(info.getDescription())
              .endElement();
    }

    writer.endElement();    // info
}

void PosLogWriter::writeDesktopsSection(XMLWriter& writer, const PosLogArchive& archive) {
    writer.startElement("desktops");

    for (PosLogDesktopSharedPtr desktop : archive.getDesktops()) {      // NOLINT(misc-const-correctness,performance-for-range-copy)
        writer.startElement("desktop")
              .addAttribute("id", desktop->getId());

        writer.startElement("units")
              .addAttribute("length", m_units.getLinearUnits(desktop->getLinearUnitsId())->getUnitsStr())
              .addAttribute("angle", m_units.getAngularUnits(desktop->getAngularUnitsId())->getUnitsStr())
              .endElement();

        if (desktop->getLinearUnitsId() == CustomId) {
            const PosLogCustomUnits& customUnits = desktop->getCustomUnits();
            writer.startElement("customUnits")
                  .addAttribute("name", customUnits.getName())
                  .addAttribute("abbrev", customUnits.getAbbrev())
                  .addAttribute("scaleBasis", customUnits.getScaleBasis())
                  .addAttribute("scaleFactor", customUnits.getScaleFactor())
                  .endElement();
        }

        writer.startElement("origin")
              .addAttribute("xoffset", desktop->getOrigin().x())
              .addAttribute("yoffset", desktop->getOrigin().y())
              .addAttribute("invertY", (desktop->isInvertY() ? "true" : "false"))
              .endElement();

        writer.startElement("size")
              .addAttribute("x", desktop->getSize().width())
              .addAttribute("y", desktop->getSize().height())
              .endElement();

        writer.startElement("screens");
        for (const PosLogScreen& screen : desktop->getScreens()) {
            writer.startElement("screen")
                  .addAttribute("desc", screen.getDescription())
                  .addAttribute("primary", (screen.isPrimary() ? "true" : "false"));

            writer.startElement("rect")
                  .addAttribute("top", screen.getRect().top())
                  .addAttribute("bottom", screen.getRect().bottom())
                  .addAttribute("left", screen.getRect().left())
                  .addAttribute("right", screen.getRect().right())
                  .endElement();

            writer.startElement("resolution")
                  .addAttribute("x", screen.getRes().width())
                  .addAttribute("y", screen.getRes().height())
                  .addAttribute("manual", (screen.isManualRes() ? "true" : "false"))
                  .endElement();

            writer.endElement();        // screen
        }
        writer.endElement();        // screens

        if (desktop->getLinearUnitsId() == CustomId) {
            const LinearUnits* customUnits = m_units.getLinearUnits(desktop->getLinearUnitsId());

            writer.startElement("displayPrecisions");

            writer.startElement("displayPrecision")
                  .addAttribute("units", customUnits->getUnitsStr());

            const Units::DisplayPrecisionNames& precisionNames = customUnits->getDisplayPrecisionNames();
            const Units::DisplayPrecisions  precisions = customUnits->getDisplayPrecisions();

            for (unsigned int i = 0; i < precisions.size(); i++) {
                writer.startElement("measurement")
                      .addAttribute("name", precisionNames[i])
                      .addAttribute("decimalPlaces", precisions[i])
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
    writer.startElement("positions");

    for (const PosLogPosition& position : archive.getPositions()) {
        writer.startElement("position")
              .addAttribute("desktopRef", position.getDesktop()->getId())
              .addAttribute("tool", position.getToolName())
              .addAttribute("date", position.getRecorded().toString(Qt::ISODate));

        if (!position.getDescription().isEmpty()) {
            writer.startElement("desc")
                  .characters(position.getDescription())
                  .endElement();
        }

        const RadioToolTraits toolTraits = position.getToolTraits();
        const PosLogToolData toolData = position.getToolData();

        auto writePoint = [toolTraits, &writer](RadioToolTrait trait, const char* pointName, const QPointF& point) {
            if ((toolTraits & trait) != 0) {
                writer.startElement("point")
                      .addAttribute("name", pointName)
                      .addAttribute("x", point.x())
                      .addAttribute("y", point.y())
                      .endElement();
            }
        };

        writer.startElement("points");
        writePoint(RadioToolTrait::XY1Available, "1", toolData.getPoint1());
        writePoint(RadioToolTrait::XY2Available, "2", toolData.getPoint2());
        writePoint(RadioToolTrait::XYVAvailable, "v", toolData.getPointV());
        writer.endElement();        // points

        auto writeProperty = [toolTraits, &writer](RadioToolTrait trait, const char* propertyName, double value) {
            if ((toolTraits & trait) != 0) {
                writer.startElement(propertyName)
                      .addAttribute("value", value)
                      .endElement();
            }
        };

        writer.startElement("properties");
        writeProperty(RadioToolTrait::WHAvailable, "width", toolData.getWidthHeight().width());
        writeProperty(RadioToolTrait::WHAvailable, "height", toolData.getWidthHeight().height());
        writeProperty(RadioToolTrait::DistAvailable, "distance", toolData.getDistance());
        writeProperty(RadioToolTrait::AreaAvailable, "area", toolData.getArea());
        writeProperty(RadioToolTrait::AngleAvailable, "angle", toolData.getAngle());
        writer.endElement();        // properties

        writer.endElement();        // position
    }

    writer.endElement();    // positions
}
