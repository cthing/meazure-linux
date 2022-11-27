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

#include <QGuiApplication>
#include <QtGlobal>
#include <X11/Xlib.h>


/// Classes and functions for working with Xlib.
///
namespace Xlib {

    /// Represents a connection to the X server. The connection is opening in the constructor and closed when the
    /// instance goes out of scope or is explicitly deleted. Instances of this class can be used anywhere an X Display
    /// pointer is needed.
    ///
    class Display {
    public:
        /// Open a connection to the X server.
        ///
        /// @param[in] name Name of the display. Specify nullptr to open the default display.
        ///
        explicit Display(const char* name = nullptr) : m_display(XOpenDisplay(name)) {
            if (m_display == nullptr) {
                qFatal("Could not open X11 display for mouse tracking");
            }
        }

        ~Display() {
            XCloseDisplay(m_display);
        }

        Display(const Display&) = delete;
        Display(Display&&) = delete;
        Display& operator=(const Display&) = delete;

        /// Obtains the underlying X display.
        ///
        /// @return Underlying X display
        ///
        ::Display* display() { return m_display; }

        /// Converts an instance of this class to the underlying X display.
        ///
        /// @return Underlying X display
        ///
        operator ::Display*() { return m_display; }     // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    private:
        ::Display* m_display;
    };


    /// Obtains the X display used the by Qt application.
    ///
    /// @return Open X display used by Qt
    ///
    inline ::Display* qtDisplay() {
        return dynamic_cast<QGuiApplication*>(QGuiApplication::instance())->
                                        nativeInterface<QNativeInterface::QX11Application>()->display();
    }
}
