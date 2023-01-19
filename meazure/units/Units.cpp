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

#include "Units.h"
#include <utility>
#include <QtMath>
#include <QRect>
#include <cmath>


//*************************************************************************
// Units
//*************************************************************************

Units::Units(QString unitsStr) : m_unitsStr(std::move(unitsStr)) {}

void Units::saveProfile(Profile& profile) {
    if (!profile.userInitiated()) {
        savePrecision(profile);
    }
}

void Units::loadProfile(Profile& profile) {
    if (!profile.userInitiated()) {
        loadPrecision(profile);
    }
}

void Units::savePrecision(Profile& profile) {
    for (unsigned int i = 0; i < m_displayPrecisions.size(); i++) {
        const QString tag = QString("Precision-%1-%2").arg(m_displayPrecisionNames[i]).arg(m_unitsStr);
        profile.writeInt(tag, m_displayPrecisions[i]);
    }
}

void Units::loadPrecision(Profile& profile) {
    for (unsigned int i = 0; i < m_displayPrecisions.size(); i++) {
        const QString tag = QString("Precision-%1-%2").arg(m_displayPrecisionNames[i]).arg(m_unitsStr);
        m_displayPrecisions[i] = profile.readInt(tag, m_displayPrecisions[i]);
    }
}

void Units::hardReset() {
    restoreDefaultPrecisions();
}


//*************************************************************************
// AngularUnits
//*************************************************************************

AngularUnits::AngularUnits(AngularUnitsId unitsId, const QString& unitsStr) :
        Units(unitsStr),
        m_unitsId(unitsId) {
    addPrecisionName("angle");
}

QString AngularUnits::format(AngularMeasurementId id, double value) const {
    return QString::number(value, 'f', getDisplayPrecisions().at(id));
}


//*************************************************************************
// DegreeUnits
//*************************************************************************

DegreeUnits::DegreeUnits() : AngularUnits(DegreesId, "deg") {
    addPrecision(1);
}

QString DegreeUnits::getLabel() const {
    return "deg";
}

double DegreeUnits::convertAngle(double angle) const {
    const double theta = isSupplementalAngle() ? std::copysign(M_PI, angle) - angle : angle;
    return qRadiansToDegrees(theta);
}


//*************************************************************************
// MeaRadianUnits
//*************************************************************************

RadianUnits::RadianUnits() : AngularUnits(RadiansId, "rad") {
    addPrecision(3);
}

QString RadianUnits::getLabel() const {
    return "rad";
}

double RadianUnits::convertAngle(double angle) const {
    return isSupplementalAngle() ? std::copysign(M_PI, angle) - angle : angle;
}


//*************************************************************************
// LinearUnits
//*************************************************************************

LinearUnits::LinearUnits(LinearUnitsId unitsId, const QString& unitsStr, const ScreenInfoProvider& screenInfoProvider) :
        Units(unitsStr), m_screenInfoProvider(screenInfoProvider), m_unitsId(unitsId) {
    addPrecisionName("x");
    addPrecisionName("y");
    addPrecisionName("w");
    addPrecisionName("h");
    addPrecisionName("d");
    addPrecisionName("area");
    addPrecisionName("rx");
    addPrecisionName("ry");
}

bool LinearUnits::isResRequired() const {
    return true;
}

QString LinearUnits::format(LinearMeasurementId id, double value) const {
    return QString::number(value, 'f', getDisplayPrecisions().at(id));
}

QString LinearUnits::getAreaLabel() const {
    return QString("sq. %1").arg(getLengthLabel());
}

QString LinearUnits::getResLabel() const {
    return QString("px/%1").arg(getLengthLabel());
}

QPointF LinearUnits::convertCoord(const QPoint& pos) const {
    const int screenIndex = m_screenInfoProvider.screenForPoint(pos);
    const QSizeF from = fromPixels(m_screenInfoProvider.getScreenRes(screenIndex));
    QPointF fpos;

    fpos.rx() = from.width() * (pos.x() - m_originOffset.x());

    if (m_invertY) {
        if ((m_originOffset.x() == 0) && (m_originOffset.y() == 0)) {
            fpos.ry() = from.height() * (m_screenInfoProvider.getVirtualRect().height() - 1 - pos.y());
        } else {
            fpos.ry() = from.height() * (m_originOffset.y() - pos.y());
        }
    } else {
        fpos.ry() = from.height() * (pos.y() - m_originOffset.y());
    }

    return fpos;
}

QPointF LinearUnits::convertPos(const QPoint& pos) const {
    const int screenIndex = m_screenInfoProvider.screenForPoint(pos);
    const QSizeF from = fromPixels(m_screenInfoProvider.getScreenRes(screenIndex));
    const double x = from.width() * pos.x();
    const double y = from.height() * pos.y();
    return { x, y };
}

QSizeF LinearUnits::convertRes(const QSizeF& res) const {
    return { 1.0 / getResFromPixels(res).width(), 1.0 / getResFromPixels(res).height() };
}

double LinearUnits::unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos) const {
    const int screenIndex = m_screenInfoProvider.screenForWindow(wnd);
    const QSizeF from = fromPixels(m_screenInfoProvider.getScreenRes(screenIndex));

    if (dir == ConvertX) {
        return pos / from.width() + m_originOffset.x();
    }

    if (m_invertY) {
        if ((m_originOffset.x() == 0) && (m_originOffset.y() == 0)) {
            return  (m_screenInfoProvider.getVirtualRect().height() - 1) - pos / from.height();
        }
        return  m_originOffset.y() - pos / from.height();
    }
    return pos / from.height() + m_originOffset.y();
}

bool LinearUnits::unconvertCoord(ConvertDir dir, const QWidget* wnd, double pos, int& c1, int& c2) const {
    const int p1 = static_cast<int>(unconvertCoord(dir, wnd, pos));
    const int p2 = p1 - 1;
    const int p3 = p1 + 1;

    const double l1 = convertCoord(dir, wnd, p1);
    const double l2 = convertCoord(dir, wnd, p2);
    const double l3 = convertCoord(dir, wnd, p3);

    const double d1 = fabs(pos - l1);
    const double d2 = fabs(pos - l2);
    const double d3 = fabs(pos - l3);

    double la;      // NOLINT(cppcoreguidelines-init-variables)
    if (d1 < d2) {
        if (d1 < d3) {
            c1 = p1;
            c2 = p3;
            la = l1;
        } else {
            c1 = p3;
            c2 = p1;
            la = l3;
        }
    } else {
        if (d2 < d3) {
            c1 = p2;
            c2 = p3;
            la = l2;
        } else {
            c1 = p3;
            c2 = p2;
            la = l3;
        }
    }

    return (format(XCoord, pos) == format(XCoord, la));
}

QPoint LinearUnits::unconvertCoord(const QPointF& pos) const {
    const QSizeF from = fromPixels(findResFromCoord(pos));

    QPoint point;
    point.rx() = qRound(pos.x() / from.width() + m_originOffset.x());

    if (m_invertY) {
        if ((m_originOffset.x() == 0) && (m_originOffset.y() == 0)) {
            point.ry() = qRound((m_screenInfoProvider.getVirtualRect().height() - 1) - pos.y() / from.height());
        } else {
            point.ry() = qRound(m_originOffset.y() - pos.y() / from.height());
        }
    } else {
        point.ry() = qRound(pos.y() / from.height() + m_originOffset.y());
    }

    return point;
}

QPoint LinearUnits::unconvertPos(const QPointF& pos) const {
    const QSizeF from = fromPixels(findResFromPos(pos));
    const int x = static_cast<int>(pos.x() / from.width());
    const int y = static_cast<int>(pos.y() / from.height());
    return { x, y };
}

QSize LinearUnits::convertToPixels(const QSizeF& res, double value, int minPixels) const {
    const QSizeF from = fromPixels(res);
    QSize pixels(static_cast<int>(value / from.width()), static_cast<int>(value / from.height()));

    // In case the resolution is small enough that the pixel becomes too small, set the
    // separation to the pixel minimum.
    if (pixels.width() < minPixels) {
        pixels.setWidth(minPixels);
    }
    if (pixels.height() < minPixels) {
        pixels.setHeight(minPixels);
    }

    return pixels;
}

QSizeF LinearUnits::getResFromPixels(const QSizeF& res) const {
    return fromPixels(res);
}

double LinearUnits::convertCoord(ConvertDir dir, const QWidget* wnd, int pos) const {
    const int screenIndex = m_screenInfoProvider.screenForWindow(wnd);
    const QSizeF from = fromPixels(m_screenInfoProvider.getScreenRes(screenIndex));

    if (dir == ConvertX) {
        return from.width() * (pos - m_originOffset.x());
    }

    if (m_invertY) {
        if ((m_originOffset.x() == 0) && (m_originOffset.y() == 0)) {
            return from.height() * (m_screenInfoProvider.getVirtualRect().height() - 1 - pos);
        }
        return from.height() * (m_originOffset.y() - pos);
    }
    return from.height() * (pos - m_originOffset.y());
}

QSizeF LinearUnits::findResFromCoord(const QPointF& pos) const {
    for (int i = 0; i < m_screenInfoProvider.getNumScreens(); i++) {
        const QRect screenRect = m_screenInfoProvider.getScreenRect(i);
        const QPointF tl = convertCoord(screenRect.topLeft());
        const QPointF br = convertCoord(screenRect.bottomRight());
        const QRectF convertedScreenRect = QRectF(tl, br).normalized();

        if (convertedScreenRect.contains(pos)) {
            return m_screenInfoProvider.getScreenRes(i);
        }
    }

    return { 0.0, 0.0 };
}

QSizeF LinearUnits::findResFromPos(const QPointF& pos) const {
    for (int i = 0; i < m_screenInfoProvider.getNumScreens(); i++) {
        const QRect screenRect = m_screenInfoProvider.getScreenRect(i);
        const QPointF tl = convertPos(screenRect.topLeft());
        const QPointF br = convertPos(screenRect.bottomRight());
        const QRectF convertedScreenRect = QRectF(tl, br).normalized();

        if (convertedScreenRect.contains(pos)) {
            return m_screenInfoProvider.getScreenRes(i);
        }
    }

    return { 0.0, 0.0 };
}


//*************************************************************************
// PixelUnits
//*************************************************************************

PixelUnits::PixelUnits(const ScreenInfoProvider& screenInfoProvider) : LinearUnits(PixelsId, "px", screenInfoProvider) {
    addPrecision(0);        // XCoord
    addPrecision(0);        // YCoord
    addPrecision(0);        // Width
    addPrecision(0);        // Height
    addPrecision(1);        // Distance
    addPrecision(0);        // Area
    addPrecision(1);        // ResX
    addPrecision(1);        // ResY
}

QString PixelUnits::getLengthLabel() const {
    return "px";
}

QString PixelUnits::getResLabel() const {
    return "px/in";
}

bool PixelUnits::isResRequired() const {
    return false;
}

QSizeF PixelUnits::getResFromPixels(const QSizeF& res) const {
    return { 1.0 / res.width(), 1.0 / res.height() };
}

QSizeF PixelUnits::fromPixels(const QSizeF& /*res*/) const {
    return { 1.0, 1.0 };
}


//*************************************************************************
// PointUnits
//*************************************************************************

PointUnits::PointUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(PointsId, "pt", screenInfoProvider) {
    addPrecision(1);        // XCoord
    addPrecision(1);        // YCoord
    addPrecision(1);        // Width
    addPrecision(1);        // Height
    addPrecision(1);        // Distance
    addPrecision(1);        // Area
    addPrecision(2);        // ResX
    addPrecision(2);        // ResY
}

QString PointUnits::getLengthLabel() const {
    return "pt";
}

QSizeF PointUnits::fromPixels(const QSizeF& res) const {
    return { k_ptPerIn / res.width(), k_ptPerIn / res.height() };
}


//*************************************************************************
// PicaUnits
//*************************************************************************

PicaUnits::PicaUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(PicasId, "pc", screenInfoProvider) {
    addPrecision(2);        // XCoord
    addPrecision(2);        // YCoord
    addPrecision(2);        // Width
    addPrecision(2);        // Height
    addPrecision(2);        // Distance
    addPrecision(2);        // Area
    addPrecision(1);        // ResX
    addPrecision(1);        // ResY
}

QString PicaUnits::getLengthLabel() const {
    return "pc";
}

QSizeF PicaUnits::fromPixels(const QSizeF& res) const {
    return { k_pcPerIn / res.width(), k_pcPerIn / res.height() };
}


//*************************************************************************
// TwipUnits
//*************************************************************************

TwipUnits::TwipUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(TwipsId, "tp", screenInfoProvider) {
    addPrecision(0);        // XCoord
    addPrecision(0);        // YCoord
    addPrecision(0);        // Width
    addPrecision(0);        // Height
    addPrecision(0);        // Distance
    addPrecision(0);        // Area
    addPrecision(4);        // ResX
    addPrecision(4);        // ResY
}

QString TwipUnits::getLengthLabel() const {
    return "tp";
}

QSizeF TwipUnits::fromPixels(const QSizeF& res) const {
    return { k_tpPerIn / res.width(), k_tpPerIn / res.height() };
}


//*************************************************************************
// InchUnits
//*************************************************************************

InchUnits::InchUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(InchesId, "in", screenInfoProvider) {
    addPrecision(3);        // XCoord
    addPrecision(3);        // YCoord
    addPrecision(3);        // Width
    addPrecision(3);        // Height
    addPrecision(3);        // Distance
    addPrecision(3);        // Area
    addPrecision(1);        // ResX
    addPrecision(1);        // ResY
}

QString InchUnits::getLengthLabel() const {
    return "in";
}

QSizeF InchUnits::fromPixels(const QSizeF& res) const {
    return { 1.0 / res.width(), 1.0 / res.height() };
}


//*************************************************************************
// CentimeterUnits
//*************************************************************************

CentimeterUnits::CentimeterUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(CentimetersId, "cm", screenInfoProvider) {
    addPrecision(2);        // XCoord
    addPrecision(2);        // YCoord
    addPrecision(2);        // Width
    addPrecision(2);        // Height
    addPrecision(2);        // Distance
    addPrecision(2);        // Area
    addPrecision(1);        // ResX
    addPrecision(1);        // ResY
}

QString CentimeterUnits::getLengthLabel() const {
    return "cm";
}

QSizeF CentimeterUnits::fromPixels(const QSizeF& res) const {
    return { k_cmPerIn / res.width(), k_cmPerIn / res.height() };
}


//*************************************************************************
// MillimeterUnits
//*************************************************************************

MillimeterUnits::MillimeterUnits(const ScreenInfoProvider& screenInfoProvider) :
        LinearUnits(MillimetersId, "mm", screenInfoProvider) {
    addPrecision(1);        // XCoord
    addPrecision(1);        // YCoord
    addPrecision(1);        // Width
    addPrecision(1);        // Height
    addPrecision(1);        // Distance
    addPrecision(1);        // Area
    addPrecision(2);        // ResX
    addPrecision(2);        // ResY
}

QString MillimeterUnits::getLengthLabel() const {
    return "mm";
}

QSizeF MillimeterUnits::fromPixels(const QSizeF& res) const {
    return { k_mmPerIn / res.width(), k_mmPerIn / res.height() };
}
