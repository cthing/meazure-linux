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

#include "ScreenInfo.h"
#include <meazure/utils/Geometry.h>
#include <meazure/utils/StringUtils.h>
#include <QScreen>
#include <QRect>
#include <QSizeF>
#include <QApplication>
#include <QMainWindow>
#include <QPixmap>
#include <qpa/qplatformcursor.h>
#include <limits>
#include <algorithm>


class ScreenInfo::Screen : public QRect {

public:
    // Default implementation of QPlatformCursor::size() returns QSize(16, 16) which is too small.
    static constexpr int k_minCursorSize = 24;
    static constexpr QSize k_defaultCursorSize = QSize(k_minCursorSize, k_minCursorSize);

    Screen(const QScreen* screen, bool primary) :
            QRect(screen->geometry()),
            m_availableGeom(screen->availableGeometry()),
            m_name(screen->name()),
            m_primary(primary),
            m_platformScale(screen->logicalDotsPerInchX() / screen->physicalDotsPerInchX(),
                            screen->logicalDotsPerInchY() / screen->physicalDotsPerInchY()),
            m_platformRes(QSizeF(screen->physicalDotsPerInchX(), screen->physicalDotsPerInchY())),
            m_useManualRes(ScreenInfo::k_defUseManualRes),
            m_calInInches(ScreenInfo::k_defCalInInches),
            m_currentRes(m_platformRes) {
        const QPlatformCursor* platformCursor = screen->handle()->cursor();
        m_cursorSize = (platformCursor == nullptr) ? k_defaultCursorSize : platformCursor->size();
        m_cursorSize.rwidth() = std::min(k_minCursorSize, m_cursorSize.width());
        m_cursorSize.rheight() = std::min(k_minCursorSize, m_cursorSize.height());
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

    /// Returns the scale factor applied by the window system to compensate for high DPI screens.
    ///
    /// @return Scale factor to accommodate high DPI screens.
    ///
    [[nodiscard]] const QSizeF& getPlatformScale() const { return m_platformScale; }

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

    /// Provides the size of the system cursor.
    ///
    /// @return Size of the system cursor
    ///
    [[nodiscard]] QSize getCursorSize() const { return m_cursorSize; }

private:
    QRect m_availableGeom;  ///< Usable screen geometry (i.e. not used by window manager)
    QString m_name;         ///< Displayable name for the screen
    bool m_primary;         ///< Indicates if the screen is the primary display
    QSizeF m_platformScale; ///< Window system scaling to compensate for high DPI
    QSizeF m_platformRes;   ///< Resolution reported by the window system.
    QSizeF m_manualRes;     ///< Manually calibrated resolution, pixels per inch.
    bool m_useManualRes;    ///< Indicates if manually calibrated resolution is used.
    bool m_calInInches;     ///< Indicates if calibration in inches or centimeters.
    QSizeF m_currentRes;    ///< Current screen resolution, pixels per inch.
    QSize m_cursorSize;     ///< Size of the screen's system cursor.
};


ScreenInfo::ScreenInfo(const QList<QScreen*>& screens) : m_numScreens(static_cast<int>(screens.size())) {
    for (int i = 0; i < m_numScreens; i++) {
        // Qt orders the screen list such that the primary screen is first.
        auto* screen = new Screen(screens[i], i == 0);
        m_screens.push_back(screen);
    }

    m_virtualGeometry = screens[0]->virtualGeometry();
    m_availableVirtualGeometry = screens[0]->availableVirtualGeometry();
}

ScreenInfo::~ScreenInfo() {
    for (int i = 0; i < m_numScreens; i++) {
        delete m_screens[i];
    }
}

void ScreenInfo::writeConfig(Config& config) const {
    config.writeInt("ScreenW", m_virtualGeometry.width());
    config.writeInt("ScreenH", m_virtualGeometry.height());

    config.writeInt("NumScreens", m_numScreens);
    for (int i = 0; i < m_numScreens; i++) {
        Screen* screen = m_screens[i];
        bool useManualRes = false;
        QSizeF manualRes;
        screen->getScreenRes(useManualRes, manualRes);

        const QString tag = QString("Screen%1-").arg(i);
        config.writeInt(tag + "CenterX", screen->center().x());
        config.writeInt(tag + "CenterY", screen->center().y());
        config.writeBool(tag + "UseManualRes", useManualRes);
        config.writeStr(tag + "ManualResX", StringUtils::dblToStr(manualRes.width()));
        config.writeStr(tag + "ManualResY", StringUtils::dblToStr(manualRes.height()));
        config.writeBool(tag + "CalInInches", screen->isCalInInches());
    }
}

void ScreenInfo::readConfig(const Config& config) {
    const int w = config.readInt("ScreenW", m_virtualGeometry.width());
    const int h = config.readInt("ScreenH", m_virtualGeometry.height());
    const int numScreens = config.readInt("NumScreens", 0);
    m_sizeChanged = ((w != m_virtualGeometry.width()) || (h != m_virtualGeometry.height()));

    if ((config.getVersion() == 1) || (numScreens == 0)) {
        QSizeF manualRes;
        const bool useManualRes = config.readBool("UseManualRes", k_defUseManualRes);
        manualRes.rwidth() = config.readDbl("ManualResX", 0.0);
        manualRes.rheight() = config.readDbl("ManualResY", 0.0);

        for (int i = 0; i < m_numScreens; i++) {
            m_screens[i]->setScreenRes(useManualRes, &manualRes);
        }
    } else {
        for (int i = 0; i < numScreens; i++) {
            const QString tag = QString("Screen%1-").arg(i);
            const QPoint center(config.readInt(tag + "CenterX", 0), config.readInt(tag + "CenterY", 0));

            const int screenIndex = screenForPoint(center);
            if (screenIndex != -1) {
                Screen* screen = m_screens[screenIndex];
                QSizeF manualRes;

                const bool useManualRes = config.readBool(tag + "UseManualRes", k_defUseManualRes);
                manualRes.rwidth() = config.readDbl(tag + "ManualResX", 0.0);
                manualRes.rheight() = config.readDbl(tag + "ManualResY", 0.0);
                screen->setScreenRes(useManualRes, &manualRes);

                const bool calInInches = config.readBool(tag + "CalInInches", k_defCalInInches);
                screen->setCalInInches(calInInches);
            }
        }
    }
}

void ScreenInfo::hardReset() {
    for (int idx = 0; idx < m_numScreens; idx++) {
        const QSizeF manualRes(0.0, 0.0);
        setScreenRes(idx, k_defUseManualRes, &manualRes);
        setCalInInches(idx, k_defCalInInches);
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
    const QPoint topLeftGlobal = wnd->parent() == nullptr ? geom.topLeft() : wnd->mapToGlobal(geom.topLeft());

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

QRect ScreenInfo::getVirtualRect() const {
    return m_virtualGeometry;
}

QRect ScreenInfo::getAvailableVirtualRect() const {
    return m_availableVirtualGeometry;
}

QRect ScreenInfo::getScreenRect(int screenIndex) const {
    return isValidScreen(screenIndex) ? *dynamic_cast<QRect*>(m_screens[screenIndex]) : QRect();
}

QSizeF ScreenInfo::getPlatformScale(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->getPlatformScale() : QSizeF();
}

void ScreenInfo::getScreenRes(int screenIndex, bool& useManualRes, QSizeF& manualRes) const {
    if (isValidScreen(screenIndex)) {
        m_screens[screenIndex]->getScreenRes(useManualRes, manualRes);
    }
}

QSizeF ScreenInfo::getScreenRes(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->getRes() : QSizeF(0.0, 0.0);
}

void ScreenInfo::setScreenRes(int screenIndex, bool useManualRes, const QSizeF* manualRes) {
    if (isValidScreen(screenIndex)) {
        m_screens[screenIndex]->setScreenRes(useManualRes, manualRes);

        emit resolutionChanged();
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

QSize ScreenInfo::getCursorSize(int screenIndex) const {
    return isValidScreen(screenIndex) ? m_screens[screenIndex]->getCursorSize() : Screen::k_defaultCursorSize;
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

QImage ScreenInfo::grabScreen(int x, int y, int width, int height) const {
    return QGuiApplication::primaryScreen()->grabWindow(0, x, y, width, height).toImage();
}
