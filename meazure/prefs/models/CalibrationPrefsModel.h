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

#pragma once

#include "Preference.h"
#include <QObject>
#include <QSizeF>
#include <QVariant>
#include <vector>
#include <algorithm>


/// Calibration preferences model.
///
class CalibrationPrefsModel : public QObject {

    Q_OBJECT

public:
    struct ScreenModel {
        [[nodiscard]] bool isDirty() const {
            return m_useManualRes->isDirty()
                || m_calInInches->isDirty()
                || m_manualRes->isDirty();
        }

        Preference<bool>* m_useManualRes;        ///< Indicates if manually calibrated resolution is used.
        Preference<bool>* m_calInInches;         ///< Indicates if calibration in inches or centimeters.
        Preference<QSizeF>* m_manualRes;         ///< Manually calibrated resolution, pixels per inch.
    };


    using ScreenModels = std::vector<ScreenModel>;


    explicit CalibrationPrefsModel(QObject* parent);

    void initialize(int screenIndex);

    void apply() const;

    [[nodiscard]] bool isDirty() const;

    [[nodiscard]] bool isUseManualRes() const;

    [[nodiscard]] bool isCalInInches() const;

    void setCalInInches(bool useInches);

    [[nodiscard]] double getManualResX() const;

    void setManualResX(double resX);

    [[nodiscard]] double getManualResY() const;

    void setManualResY(double resY);

    [[nodiscard]] double getManualWidth() const;

    void setManualWidth(double width);

    [[nodiscard]] double getManualHeight() const;

    void setManualHeight(double height);

    Preference<int>* m_screenIndex;
    ScreenModels m_screens;

public slots:
    void setUseManualRes(bool useManual);

signals:
    void screenIndexChanged(int screenIndex);
    void useManualResChanged(bool useManualRes);
    void calInInchesChanged(bool calInInches);
    void manualResChanged(QSizeF res);
    void dirtyChanged(bool dirty);
};
