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

#pragma once

#include <QRectF>
#include <QSizeF>
#include <QString>
#include <vector>


/// Represents a single monitor attached to the system. There is an instance of this class per monitor.
///
class PosLogScreen {

public:
    [[nodiscard]] bool isPrimary() const {
        return m_primary;
    }

    void setPrimary(bool primary) {
        m_primary = primary;
    }

    [[nodiscard]] const QRectF &getRect() const {
        return m_rect;
    }

    void setRect(const QRectF &rect) {
        m_rect = rect;
    }

    [[nodiscard]] const QSizeF &getRes() const {
        return m_res;
    }

    void setRes(const QSizeF &res) {
        m_res = res;
    }

    [[nodiscard]] bool isManualRes() const {
        return m_manualRes;
    }

    void setManualRes(bool manualRes) {
        m_manualRes = manualRes;
    }

    [[nodiscard]] const QString &getDescription() const {
        return m_description;
    }

    void setDescription(const QString &description) {
        m_description = description;
    }

    bool operator==(const PosLogScreen &rhs) const {
        return m_primary == rhs.m_primary &&
               m_rect == rhs.m_rect &&
               m_res == rhs.m_res &&
               m_manualRes == rhs.m_manualRes &&
               m_description == rhs.m_description;
    }

    bool operator!=(const PosLogScreen &rhs) const {
        return !(rhs == *this);
    }

private:
    bool m_primary { false };
    QRectF m_rect;
    QSizeF m_res;
    bool m_manualRes { false };
    QString m_description;
};


using PosLogScreenVector = std::vector<PosLogScreen>;
