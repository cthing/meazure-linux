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

#include "CalibrationPrefsModel.h"


CalibrationPrefsModel::CalibrationPrefsModel(ScreenInfo* screenInfo, QObject *parent) :
        QObject(parent),
        m_screenInfo(screenInfo) {
    for (int screenIndex = 0; screenIndex < m_screenInfo->getNumScreens(); screenIndex++) {
        ScreenModel screenModel;        // NOLINT(cppcoreguidelines-pro-type-member-init)
        screenModel.m_calInInches = new Preference<bool>(m_screenInfo->isCalInInches(screenIndex), this);
        connect(screenModel.m_calInInches, &PreferenceBase::valueChanged, this,
                [this, screenIndex](const QVariant& value) {
            if (screenIndex == m_screenIndex->getValue()) {
                emit calInInchesChanged(value.toBool());
            }
        });
        connect(screenModel.m_calInInches, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

        QSizeF manualRes;
        bool useManualRes = false;
        m_screenInfo->getScreenRes(screenIndex, useManualRes, manualRes);
        screenModel.m_useManualRes = new Preference<bool>(useManualRes, this);
        connect(screenModel.m_useManualRes, &PreferenceBase::valueChanged, this,
                [this, screenIndex](const QVariant& value) {
            if (screenIndex == m_screenIndex->getValue()) {
                emit useManualResChanged(value.toBool());
            }
        });
        connect(screenModel.m_useManualRes, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

        manualRes = useManualRes ? manualRes : m_screenInfo->getScreenRes(screenIndex);
        screenModel.m_manualRes = new Preference<QSizeF>(manualRes, this);
        connect(screenModel.m_manualRes, &PreferenceBase::valueChanged, this,
                [this, screenIndex](const QVariant& value) {
            if (screenIndex == m_screenIndex->getValue()) {
                emit manualResChanged(value.toSizeF());
            }
        });
        connect(screenModel.m_manualRes, SIGNAL(dirtyChanged(bool)), this, SIGNAL(dirtyChanged(bool)));

        m_screens.push_back(screenModel);
    }

    m_screenIndex = new Preference(0, this);
    connect(m_screenIndex, &PreferenceBase::valueChanged, this, [this](const QVariant& value) {
        emit screenIndexChanged(value.toInt());
    });
}

void CalibrationPrefsModel::initialize(int screenIndex) {
    for (int i = 0; i < static_cast<int>(m_screens.size()); i++) {
        ScreenModel& screenModel = m_screens[i];
        screenModel.m_calInInches->initialize(m_screenInfo->isCalInInches(i));

        QSizeF manualRes;
        bool useManualRes = false;
        m_screenInfo->getScreenRes(i, useManualRes, manualRes);
        screenModel.m_useManualRes->initialize(useManualRes);

        manualRes = useManualRes ? manualRes : m_screenInfo->getScreenRes(i);
        screenModel.m_manualRes->initialize(manualRes);
    }

    m_screenIndex->initialize(screenIndex);
}

void CalibrationPrefsModel::apply() const {
    for (int i = 0; i < static_cast<int>(m_screens.size()); i++) {
        const ScreenModel& screenModel = m_screens[i];
        m_screenInfo->setCalInInches(i, screenModel.m_calInInches->getValue());

        const bool useManualRes = screenModel.m_useManualRes->getValue();
        if (useManualRes) {
            const QSizeF manualRes = screenModel.m_manualRes->getValue();
            m_screenInfo->setScreenRes(i, useManualRes, &manualRes);
        } else {
            m_screenInfo->setScreenRes(i, useManualRes, nullptr);
        }
    }
}

bool CalibrationPrefsModel::isDirty() const {
    return std::any_of(m_screens.begin(), m_screens.end(), [](const ScreenModel& screenModel) {
        return screenModel.isDirty();
    });
}

bool CalibrationPrefsModel::isUseManualRes() const {
    return m_screens[m_screenIndex->getValue()].m_useManualRes->getValue();
}

void CalibrationPrefsModel::setUseManualRes(bool useManual) {
    m_screens[m_screenIndex->getValue()].m_useManualRes->setValue(useManual);
}

bool CalibrationPrefsModel::isCalInInches() const {
    return m_screens[m_screenIndex->getValue()].m_calInInches->getValue();
}

void CalibrationPrefsModel::setCalInInches(bool useInches) {
    m_screens[m_screenIndex->getValue()].m_calInInches->setValue(useInches);
}

double CalibrationPrefsModel::getManualResX() const {
    return m_screens[m_screenIndex->getValue()].m_manualRes->getValue().width();
}

void CalibrationPrefsModel::setManualResX(double resX) {
    QSizeF res(m_screens[m_screenIndex->getValue()].m_manualRes->getValue());
    res.setWidth(resX);
    m_screens[m_screenIndex->getValue()].m_manualRes->setValue(res);
}

double CalibrationPrefsModel::getManualResY() const {
    return m_screens[m_screenIndex->getValue()].m_manualRes->getValue().height();
}

void CalibrationPrefsModel::setManualResY(double resY) {
    QSizeF res(m_screens[m_screenIndex->getValue()].m_manualRes->getValue());
    res.setHeight(resY);
    m_screens[m_screenIndex->getValue()].m_manualRes->setValue(res);
}

double CalibrationPrefsModel::getManualWidth() const {
    const int screenIndex = m_screenIndex->getValue();
    return m_screenInfo->getScreenRect(screenIndex).width() / m_screens[screenIndex].m_manualRes->getValue().width();
}

void CalibrationPrefsModel::setManualWidth(double width) {
    setManualResX(m_screenInfo->getScreenRect(m_screenIndex->getValue()).width() / width);
}

double CalibrationPrefsModel::getManualHeight() const {
    const int screenIndex = m_screenIndex->getValue();
    return m_screenInfo->getScreenRect(screenIndex).height() / m_screens[screenIndex].m_manualRes->getValue().height();
}

void CalibrationPrefsModel::setManualHeight(double height) {
    setManualResY(m_screenInfo->getScreenRect(m_screenIndex->getValue()).height() / height);
}
