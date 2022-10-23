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

#include "ScreenInfo.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <QScreen>
#include <QRect>
#include <QSizeF>
#include <QApplication>
#include <QMainWindow>
#include <limits>
#include <algorithm>


class ScreenInfo::Screen : public QRect {

public:
    Screen(const QScreen* screen, bool primary) :
            QRect(screen->geometry()),
            m_name(screen->name()),
            m_primary(primary),
            m_platformRes(QSizeF(screen->physicalDotsPerInchX(), screen->physicalDotsPerInchY())),
            m_useManualRes(ScreenInfo::defUseManualRes),
            m_calInInches(ScreenInfo::defCalInInches),
            m_currentRes(m_platformRes) {
    }

    /// Returns the descriptive name for the screen.
    ///
    /// @return Name for the screen or the empty string if there is no name.
    ///
    [[nodiscard]] const QString& getName() const { return m_name; }

    /// Indicates if this is the primary screen.
    ///
    /// @return true if this is the primary screen.
    ///
    [[nodiscard]] bool isPrimary() const { return m_primary; }

    /// Sets the resolution of the screen, in pixels per inch.
    ///
    /// @param[in] useManualRes Specifies whether the manually calibrated resolution should be used (true) or the
    ///                         operating system resolution (false).
    /// @param[in] manualRes If non-nullptr, specifies the manually calibrated resolution.
    ///
    void setScreenRes(bool useManualRes, const QSizeF* manualRes = nullptr) {
        m_useManualRes = useManualRes;

        if (manualRes != nullptr) {
            m_manualRes = *manualRes;
        }

        if (m_manualRes.width() < std::numeric_limits<double>::min() ||
                m_manualRes.height() < std::numeric_limits<double>::min()) {
            m_manualRes = m_platformRes;
        }

        m_currentRes = m_useManualRes ? m_manualRes : m_platformRes;
    }

    /// Obtains the currently set manual screen resolution, if set.
    ///
    /// @param[out] useManualRes Indicates whether the manually calibrated resolution is used (true) or the operating
    ///                         system resolution (false).
    /// @param[out] manualRes Manually calibrated resolution, in pixels per inch.
    ///
    void getScreenRes(bool& useManualRes, QSizeF& manualRes) const {
        useManualRes = m_useManualRes;
        manualRes = m_manualRes;
    }

    /// Obtains the current screen resolution. If the resolution has been manually calibrated, it is returned.
    /// Otherwise the operating system reported resolution is returned.
    ///
    /// @return Screen resolution, in pixels per inch.
    ///
    [[nodiscard]] const QSizeF& getRes() const { return m_currentRes; }

    /// Indicates if the screen resolution has been manually set.
    ///
    /// @return true if the screen resolution has been set manually.
    ///
    [[nodiscard]] bool isManualRes() const { return m_useManualRes; }

    /// Specifies whether the resolution calibration has been performed in inches or centimeters.
    ///
    /// @param[in] calInInches true if the calibration has been performed in inches.
    ///
    void setCalInInches(bool calInInches) { m_calInInches = calInInches; }

    /// Indicates whether the resolution calibration has been performed in inches or centimeters.
    ///
    /// @return true if the calibration has been performed in inches.
    ///
    [[nodiscard]] bool isCalInInches() const { return m_calInInches; }

private:
    QString m_name;         ///< Displayable name for the screen
    bool m_primary;         ///< Indicates if the screen is the primary display
    QSizeF m_platformRes;   ///< Resolution reported by the window system.
    QSizeF m_manualRes;     ///< Manually calibrated resolution, pixels per inch.
    bool m_useManualRes;    ///< Indicates if manually calibrated resolution is used.
    bool m_calInInches;     ///< Indicates if calibration in inches or centimeters.
    QSizeF m_currentRes;    ///< Current screen resolution, pixels per inch.
};


ScreenInfo::ScreenInfo(const QList<QScreen*>& screens) : m_numScreens(static_cast<int>(screens.size())) {
    for (int i = 0; i < m_numScreens; i++) {
        // Qt orders the screen list such that the primary screen is first.
        auto* screen = new Screen(screens[i], i == 0);
        m_screens.push_back(screen);
    }

    m_virtualGeometry = screens[0]->virtualGeometry();
}

ScreenInfo::~ScreenInfo() {
    for (int i = 0; i < m_numScreens; i++) {
        delete m_screens[i];
    }
}

void ScreenInfo::saveProfile(Profile& profile) const {
    if (!profile.userInitiated()) {
        profile.writeInt("ScreenW", m_virtualGeometry.width());
        profile.writeInt("ScreenH", m_virtualGeometry.height());

        profile.writeInt("NumScreens", m_numScreens);
        for (int i = 0; i < m_numScreens; i++) {
            Screen* screen = m_screens[i];
            bool useManualRes = false;
            QSizeF manualRes;
            screen->getScreenRes(useManualRes, manualRes);

            const QString tag = QString("Screen%1-").arg(i);
            profile.writeInt(tag + "CenterX", screen->center().x());
            profile.writeInt(tag + "CenterY", screen->center().y());
            profile.writeBool(tag + "UseManualRes", useManualRes);
            profile.writeStr(tag + "ManualResX", StringUtils::dblToStr(manualRes.width()));
            profile.writeStr(tag + "ManualResY", StringUtils::dblToStr(manualRes.height()));
            profile.writeBool(tag + "CalInInches", screen->isCalInInches());
        }
    }
}

void ScreenInfo::loadProfile(Profile& profile) {
    if (!profile.userInitiated()) {
        const int w = profile.readInt("ScreenW", m_virtualGeometry.width());
        const int h = profile.readInt("ScreenH", m_virtualGeometry.height());
        const int numScreens = profile.readInt("NumScreens", 0);
        m_sizeChanged = ((w != m_virtualGeometry.width()) || (h != m_virtualGeometry.height()));

        if ((profile.getVersion() == 1) || (numScreens == 0)) {
            QSizeF manualRes;
            const bool useManualRes = profile.readBool("UseManualRes", defUseManualRes);
            manualRes.rwidth() = profile.readDbl("ManualResX", 0.0);
            manualRes.rheight() = profile.readDbl("ManualResY", 0.0);

            for (int i = 0; i < m_numScreens; i++) {
                m_screens[i]->setScreenRes(useManualRes, &manualRes);
            }
        } else {
            for (int i = 0; i < numScreens; i++) {
                const QString tag = QString("Screen%1-").arg(i);
                const QPoint center(profile.readInt(tag + "CenterX", 0), profile.readInt(tag + "CenterY", 0));

                const int screenIndex = screenForPoint(center);
                if (screenIndex != -1) {
                    Screen* screen = m_screens[screenIndex];
                    QSizeF manualRes;

                    const bool useManualRes = profile.readBool(tag + "UseManualRes", defUseManualRes);
                    manualRes.rwidth() = profile.readDbl(tag + "ManualResX", 0.0);
                    manualRes.rheight() = profile.readDbl(tag + "ManualResY", 0.0);
                    screen->setScreenRes(useManualRes, &manualRes);

                    const bool calInInches = profile.readBool(tag + "CalInInches", defCalInInches);
                    screen->setCalInInches(calInInches);
                }
            }
        }
    }
}

void ScreenInfo::masterReset() const {
    for (auto* screen : m_screens) {
        const QSizeF manualRes(0.0, 0.0);
        screen->setScreenRes(defUseManualRes, &manualRes);
        screen->setCalInInches(defCalInInches);
    }
}

int ScreenInfo::getNumScreens() const {
    return m_numScreens;
}

int ScreenInfo::screenForPoint(const QPoint& point) const {
    return Geometry::contains(m_screens, point);
}

int ScreenInfo::screenForRect(const QRect& rect) const {
    return Geometry::contains(m_screens, rect);
}

int ScreenInfo::screenForWindow(const QWidget* wnd) const {
    const QRect geom = wnd->geometry();
    const QPoint topLeftGlobal = wnd->mapToGlobal(geom.topLeft());

    return screenForRect(QRect(topLeftGlobal, geom.size()));
}

QPoint ScreenInfo::getCenter() const {
    for (QWidget *w : QApplication::topLevelWidgets()) {
        if (auto* mainWin = qobject_cast<QMainWindow*>(w)) {
            const int screenIndex = screenForWindow(mainWin);
            if (screenIndex == -1) {
                break;
            }
            return m_screens[screenIndex]->center();
        }
    }

    return m_screens.empty() ? QPoint() : m_screens[0]->center();
}

QPoint ScreenInfo::getOffScreen() const {
    QPoint offScreenPoint = getVirtualRect().bottomRight();
    offScreenPoint.rx() += 1000;
    offScreenPoint.ry() += 1000;
    return offScreenPoint;
}

QRect ScreenInfo::getVirtualRect() const {
    return m_virtualGeometry;
}

QRect ScreenInfo::getScreenRect(int screenIndex) const {
    return isValidScreen(screenIndex) ? *dynamic_cast<QRect*>(m_screens[screenIndex]) : QRect();
}

void ScreenInfo::getScreenRes(int screenIndex, bool& useManualRes, QSizeF& manualRes) const {
    if (isValidScreen(screenIndex)) {
        m_screens[screenIndex]->getScreenRes(useManualRes, manualRes);
    }
}

QSizeF ScreenInfo::getScreenRes(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->getRes() : QSizeF(0.0, 0.0);
}

void ScreenInfo::setScreenRes(int screenIndex, bool useManualRes, const QSizeF* manualRes) const {
    if (isValidScreen(screenIndex)) {
        m_screens[screenIndex]->setScreenRes(useManualRes, manualRes);
    }
}

bool ScreenInfo::isManualRes(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->isManualRes() : false;
}

bool ScreenInfo::isCalibrationRequired() const {
    return std::any_of(m_screens.begin(), m_screens.end(), [](Screen* screen) { return !screen->isManualRes(); });
}

bool ScreenInfo::isPrimary(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->isPrimary() : false;
}

QString ScreenInfo::getScreenName(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->getName() : QString();
}

bool ScreenInfo::isCalInInches(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->isCalInInches() : false;
}

void ScreenInfo::setCalInInches(int screenIndex, bool calInInches) {
    if (isValidScreen(screenIndex)) {
        m_screens[screenIndex]->setCalInInches(calInInches);
    }
}

bool ScreenInfo::sizeChanged() const {
    return m_sizeChanged;
}

QPoint ScreenInfo::constrainPosition(const QPoint& point) const {
    return Geometry::constrain(m_screens, point);
}
