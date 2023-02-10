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

#include <QtGlobal>
#include <QString>
#include <QGuiApplication>
#include <xcb/xcb.h>
#include <memory>
#include <cstring>
#include <cstdlib>


/// Classes and functions for working with the X protocol C-language Binding (XCB) library (see
/// https://xcb.freedesktop.org/). The classes make it easier to manage the cookie/reply pattern
/// and take advantage of latency hiding.
///
namespace Xcb {

    /// Obtains the XCB connection used the by Qt application.
    ///
    /// @return Open XCB connection used by Qt
    ///
    inline xcb_connection_t* qtConnection() {
        return dynamic_cast<QGuiApplication*>(QGuiApplication::instance())->
                                        nativeInterface<QNativeInterface::QX11Application>()->connection();
    }


    /// Base class for the XCB wrapper classes.
    ///
    /// @tparam COOKIE Type of the cookie returned by an XCB function
    /// @tparam REPLY Type of the reply returned by an XCB reply function
    ///
    template<typename COOKIE, typename REPLY>
    class Base {
    public:
        virtual ~Base() = default;

        Base(const Base&) = default;
        Base(Base&&) noexcept = default;
        Base& operator=(const Base&) = default;
        Base& operator=(Base&&) noexcept = default;

        /// Calling the reply function is deferred until the first use of the arrow operator to access a member of the
        /// reply structure. Wrapping the reply structure in a shared pointer ensure it will be deallocated when it is
        /// no longer referenced.
        ///
        /// @return Access to the reply structure.
        ///
        virtual std::shared_ptr<REPLY> operator->() {
            return get();
        }

    protected:
        using ReplyPtr = std::shared_ptr<REPLY>;
        using ReplyFunc = REPLY* (*)(xcb_connection_t* conn, COOKIE cookie, xcb_generic_error_t **err);

        explicit Base(xcb_connection_t* connection, const COOKIE& cookie, const ReplyFunc& replyFunc) :
                m_connection(connection), m_cookie(cookie), m_replyFunc(replyFunc) {
        }

        /// Performs the lazy call to the reply function.
        ///
        /// @return Shared pointer to the reply structure. Because the structure is allocated using malloc by the
        ///         XCB library, it must be freed using the free function rather than delete. Any error will cause
        ///         an abort.
        ///
        ReplyPtr get() {
            if (!m_reply) {
                xcb_generic_error_t* error = nullptr;
                m_reply = ReplyPtr(m_replyFunc(m_connection, m_cookie, &error), std::free);
                if (error != nullptr) {
                    const int errorCode = error->error_code;
                    std::free(error);       // NOLINT(cppcoreguidelines-no-malloc,hicpp-no-malloc)
                    qFatal("Could not get reply (xcb error %d)", errorCode);
                }
            }

            return m_reply;
        }


        xcb_connection_t* m_connection;
        COOKIE m_cookie;
        ReplyFunc m_replyFunc;
        ReplyPtr m_reply;
    };


    /// Represents an X11 atom. Instances of this class can be used wherever an atom parameter is expected in an XCB
    /// function.
    ///
    class Atom : public Base<xcb_intern_atom_cookie_t, xcb_intern_atom_reply_t> {
    public:
        /// Interns an atom with the specified name.
        ///
        /// @param[in] connection Connection to the X server
        /// @param[in] onlyIfExists Intern the atom only if it already exists
        /// @param[in] atomName Name of the atom
        ///
        Atom(xcb_connection_t* connection, bool onlyIfExists, const char* atomName) :
                Base(connection,
                     xcb_intern_atom(connection, onlyIfExists ? 1 : 0, std::strlen(atomName), atomName),
                     xcb_intern_atom_reply) {
        }

        operator xcb_atom_t() { return get()->atom; }     // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    };


    /// Represents a coordinate translation.
    ///
    class TranslateCoordinates : public Base<xcb_translate_coordinates_cookie_t, xcb_translate_coordinates_reply_t> {
    public:
        /// Translates the specified coordinates from being relative to the specified source window to being relative
        /// to the specified destination window.
        ///
        /// @param[in] connection Connection to the X server
        /// @param[in] srcWin Source window containing the specified coordinates
        /// @param[in] dstWin Destination window to which the coordinates should be translated
        /// @param[in] x X-axis coordinate relative to the source window origin
        /// @param[in] y Y-axis coordinate relative to the source window origin
        ///
        TranslateCoordinates(xcb_connection_t* connection, xcb_window_t srcWin, xcb_window_t dstWin,
                             int16_t x, int16_t y) :
                Base(connection,
                     xcb_translate_coordinates(connection, srcWin, dstWin, x, y),
                     xcb_translate_coordinates_reply) {
        }
    };


    /// Represents a window's location and dimensions.
    ///
    class Geometry : public Base<xcb_get_geometry_cookie_t, xcb_get_geometry_reply_t> {
    public:
        /// Obtains the location and dimensions of the specified window
        ///
        /// @param[in] connection Connection to the X server
        /// @param[in] window Window whose location and dimension are to be obtained
        ///
        Geometry(xcb_connection_t* connection, xcb_window_t window) :
                Base(connection,
                     xcb_get_geometry(connection, window),
                     xcb_get_geometry_reply) {
        }
    };


    /// Represents a window's attributes.
    ///
    class Attributes : public Base<xcb_get_window_attributes_cookie_t, xcb_get_window_attributes_reply_t> {
    public:
        /// Obtains the attributes of the specified window.
        ///
        /// @param[in] connection Connection to the X server
        /// @param[in] window Window whose attributes are to be obtained
        ///
        Attributes(xcb_connection_t* connection, xcb_window_t window) :
                Base(connection,
                     xcb_get_window_attributes(connection, window),
                     xcb_get_window_attributes_reply) {
        }
    };


    /// Represents the child windows of the specified window.
    ///
    class QueryTree : public Base<xcb_query_tree_cookie_t, xcb_query_tree_reply_t> {
    public:
        /// Obtains the child windows of the specified window.
        ///
        /// @param[in] connection Connection to the X server
        /// @param window Parent window whose child windows are to be obtained
        ///
        QueryTree(xcb_connection_t* connection, xcb_window_t window) :
                Base(connection,
                     xcb_query_tree(connection, window),
                     xcb_query_tree_reply) {
        }

        /// Obtains the child windows.
        ///
        /// @return Child windows
        ///
        const xcb_window_t* children() {
            return xcb_query_tree_children(get().get());
        }

        /// Obtains the number of child windows.
        ///
        /// @return Number of child windows.
        ///
        int numChildren() {
            return xcb_query_tree_children_length(get().get());
        }
    };


    /// Represents a window property.
    ///
    class Property : public Base<xcb_get_property_cookie_t, xcb_get_property_reply_t> {
    public:
        /// Obtains the specified window property.
        ///
        /// @param[in] connection Connection to the X server
        /// @param[in] deleteProperty Indicates if the property should be deleted
        /// @param[in] window Window whose property is to be read
        /// @param[in] atom Atom representing the name of the property to read
        /// @param[in] type Type of the property to read
        /// @param[in] offset Offset into the property data
        /// @param[in] length Amount of property data to read in 32-bit multiples (e.g. 3 = 12 bytes)
        ///
        Property(xcb_connection_t* connection, bool deleteProperty, xcb_window_t window, xcb_atom_t atom,
                 xcb_atom_t type, int offset, int length) :
                Base(connection,
                     xcb_get_property(connection, deleteProperty ? 1 : 0, window, atom, type, offset, length),
                     xcb_get_property_reply) {
        }

        /// Obtains the data for the property.
        ///
        /// @return Property data
        ///
        void* value() {
            return xcb_get_property_value(get().get());
        }

        /// Obtains the actual amount of data in the value. The interpretation of the length depends on the property
        /// type and format.
        ///
        int length() {
            return xcb_get_property_value_length(get().get());
        }
    };
}
