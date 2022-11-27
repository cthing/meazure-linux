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

#include <X11/Xlib.h>
#include <X11/extensions/record.h>


/// Classes and functions for working with the X Record extension (see
/// https://www.x.org/releases/X11R7.6/doc/libXtst/recordlib.html). The extension is used to obtain the pointer
/// location regardless of whether the application has focus and to track changes to all windows on the screen.
///
namespace XRecord {

    /// Indicates whether the XRecord extension is available.
    ///
    /// @param[in] display Connection to the X server
    /// @return true if the XRecord extension is available.
    ///
    inline bool hasExtension(Display* display) {
        int major;      // NOLINT(cppcoreguidelines-init-variables)
        int minor;      // NOLINT(cppcoreguidelines-init-variables)
        int firstEvent; // NOLINT(cppcoreguidelines-init-variables)
        int firstError; // NOLINT(cppcoreguidelines-init-variables)
        return (XQueryExtension(display, "RECORD", &major, &firstEvent, &firstError) == True)
                && (XRecordQueryVersion(display, &major, &minor) != 0);
    }


    /// Represents a range of events to record. Instances of this class can be passed anywhere an XRecordRange is
    /// expected.
    ///
    class Range {
    public:
        Range() : m_range(XRecordAllocRange()) {
            if (m_range == nullptr) {
                qFatal("Could not allocate XRecord range object");
            }
        }

        ~Range() {
            XFree(m_range);
        }

        Range(const Range&) = delete;
        Range(Range&&) = delete;
        Range& operator=(const Range&) = delete;

        /// Sets the low and upper bounds for device events to record.
        ///
        /// @param[in] first Low bound for device events to record
        /// @param[in] last Upper bound for device events to record. Can be set to the same value as the lower
        ///     bound if only a single event is to be recorded.
        ///
        void setDeviceEvents(unsigned char first, unsigned char last) {
            m_range->device_events.first = first;
            m_range->device_events.last = last;
        }

        /// Sets the device event to record.
        ///
        /// @param[in] event Device events to record
        ///
        void setDeviceEvent(unsigned char event) {
            setDeviceEvents(event, event);
        }

        /// Sets the low and upper bounds for core and extension events to record.
        ///
        /// @param[in] first Low bound for core and extension events to record
        /// @param[in] last Upper bound for core and extension events to record. Can be set to the same value as the
        ///     lower bound if only a single event is to be recorded.
        ///
        void setDeliveredEvents(unsigned char first, unsigned char last) {
            m_range->delivered_events.first = first;
            m_range->delivered_events.last = last;
        }

        /// Sets the core or extension event to record.
        ///
        /// @param[in] event Core or extension event to record
        ///
        void setDeliveredEvent(unsigned char event) {
            setDeliveredEvents(event, event);
        }

        /// Converts this range to the corresponding X structure.
        ///
        /// @return X range structure corresponding to this range.
        ///
        operator XRecordRange*() const { return m_range; }     // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    private:
        XRecordRange* m_range;
    };


    /// Represents an XRecord context.
    ///
    class Context {
    public:
        /// Constructs a recording context. Two display connections are required to properly manage recording.
        ///
        /// @param[in] controlDisplay Connection to the X server to be used for controlling the recording
        /// @param[in] dataDisplay Connections to the X server to be used to obtain the recording data
        /// @param[in] flags Specifies whether detailed time or sequence info should be sent
        /// @param[in] clientSpec Specifies the clients to record or XRecordAllClients
        /// @param[in] numClients Specifies the number of clients or 0 if XRecordAllClients
        /// @param[in] ranges Ranges specifying what to record
        /// @param[in] numRanges Number of ranges specified
        ///
        Context(Display* controlDisplay, Display* dataDisplay, int flags, XRecordClientSpec clientSpec,
                int numClients, XRecordRange* ranges, int numRanges) :
                m_controlDisplay(controlDisplay),
                m_dataDisplay(dataDisplay),
                m_context(XRecordCreateContext(controlDisplay, flags, &clientSpec, numClients, &ranges, numRanges)) {
            if (m_context == 0) {
                qFatal("Could not create XRecord context");
            }
        }

        ~Context() {
            disable();
            XRecordFreeContext(m_controlDisplay, m_context);
        }

        Context(const Context&) = delete;
        Context(Context&&) = delete;
        Context& operator=(const Context&) = delete;

        /// Enables asynchronous recording.
        ///
        /// @param[in] callback Function or lambda to be called with the recorded data
        /// @param[in] closure User defined data (e.g. this pointer)
        ///
        void enableAsync(XRecordInterceptProc callback, XPointer closure) const {
            if (XRecordEnableContextAsync(m_dataDisplay, m_context, callback, closure) == 0) {
                qFatal("Could not enable XRecord context");
            }
        }

        /// Disables recording.
        ///
        void disable() const {
            XRecordDisableContext(m_controlDisplay, m_context);
        }

        /// Processes recording replies while in the data callback.
        ///
        void processReplies() const {
            XRecordProcessReplies(m_dataDisplay);
        }

    private:
        Display* m_controlDisplay;
        Display* m_dataDisplay;
        XRecordContext m_context;
    };
}
