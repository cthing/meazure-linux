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

#include "TimedEventLoop.h"
#include <QWidget>
#include <vector>
#include <utility>
#include <cstdarg>


/// Hides the wrapped widget during the scope of this object. When this object goes out of scope, the wrapped
/// widget is restored to visibility if it was initially visible.
///
class Cloaker : public QObject {

    Q_OBJECT

public:
    template<class ... ARGS>
    explicit Cloaker(ARGS ... widgets) {
        const std::initializer_list<QWidget*> params = { widgets... };
        for (QWidget* widget : params) {
            const bool hidden = widget->isHidden();
            m_widgetStates.emplace_back(widget, hidden);
            if (!hidden) {
                widget->hide();
            }
        }

        const TimedEventLoop loop(k_processingTime);
    }

    ~Cloaker() override {
        for (State state : m_widgetStates) {
            if (!state.second) {
                state.first->show();
            }
        }
    }

    Cloaker(const Cloaker&) = delete;
    Cloaker(Cloaker&&) = delete;
    Cloaker& operator=(const Cloaker&) = delete;

private:
    using State = std::pair<QWidget*, bool>;

    static constexpr int k_processingTime = 100;   // Time to spend processing events, milliseconds

    std::vector<State> m_widgetStates;
};
