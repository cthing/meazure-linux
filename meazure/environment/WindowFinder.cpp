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

#include "WindowFinder.h"
#include <meazure/utils/XcbUtils.h>
#include <meazure/graphics/Graphic.h>
#include <algorithm>
#include <memory>


class BaseCommand {
public:
    BaseCommand() = default;
    virtual ~BaseCommand() = default;

    BaseCommand(const BaseCommand&) = default;
    BaseCommand(BaseCommand&&) noexcept = default;
    BaseCommand& operator=(const BaseCommand&) = default;
    BaseCommand& operator=(BaseCommand&&) noexcept = default;

    virtual int16_t x() = 0;
    virtual int16_t y() = 0;
    virtual uint16_t width() = 0;
    virtual uint16_t height() = 0;
};

class GeometryCommand : public BaseCommand {
public:
    GeometryCommand(xcb_connection_t* conn, xcb_window_t child) : m_geom(conn, child) {
    }

    int16_t x() override { return m_geom->x; }
    int16_t y() override { return m_geom->y; }
    uint16_t width() override { return m_geom->width; }
    uint16_t height() override { return m_geom->height; }

protected:
    Xcb::Geometry m_geom; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes,misc-non-private-member-variables-in-classes)
};


class TranslatedGeometryCommand : public GeometryCommand {
public:
    TranslatedGeometryCommand(xcb_connection_t* conn, xcb_window_t root, xcb_window_t child, int16_t x, int16_t y) :
            GeometryCommand(conn, child), m_translate(conn, child, root, x, y) {
    }

    int16_t x() override { return m_translate->dst_x; }
    int16_t y() override { return m_translate->dst_y; }

private:
    Xcb::TranslateCoordinates m_translate;
};


/// Performs the scan for visible top level windows.
///
class Finder {
public:
    Finder() :
            m_setup(xcb_get_setup(m_conn)),
            m_wmStateAtom(m_conn, false, "WM_STATE"),
            m_netWMStateAtom(m_conn, false, "_NET_WM_STATE"),
            m_netWMStateHiddenAtom(m_conn, false, "_NET_WM_STATE_HIDDEN") {
    }

    std::vector<QRect> scan() {
        Commands commands;
        for (xcb_screen_iterator_t screen = xcb_setup_roots_iterator(m_setup); screen.rem > 0; xcb_screen_next(&screen)) {
            processRootWindow(screen.data->root, commands);
        }

        // X reports windows from the bottom to the top. We want the windows reported from top to bottom. Therefore,
        // traverse the commands backwards to generate the window rectangles.
        std::vector<QRect> windows;
        windows.reserve(commands.size());
        std::for_each(commands.rbegin(), commands.rend(), [&windows](const auto& command) {
            windows.emplace_back(command->x(), command->y(), command->width(), command->height());
        });

        return windows;
    }

private:
    using Commands = std::vector<std::unique_ptr<BaseCommand>>;

    bool hasWMState(xcb_window_t win) {
        Xcb::Property prop(m_conn, false, win, m_wmStateAtom->atom, XCB_GET_PROPERTY_TYPE_ANY, 0, 0);
        return prop->type != XCB_ATOM_NONE;
    }

    bool isHidden(xcb_window_t win) {
        Xcb::Property state(m_conn, false, win, m_netWMStateAtom->atom, XCB_ATOM_ATOM, 0, 1024);
        if (state->type == XCB_ATOM_ATOM) {
            const int len = state.length();
            const xcb_atom_t* values = static_cast<xcb_atom_t*>(state.value());
            for (int i = 0; i < len; i++) {
                if (values[i] == m_netWMStateHiddenAtom->atom) { // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
                    return true;
                }
            }
        }

        return false;
    }

    bool isViewable(xcb_window_t win, Xcb::Attributes& attrs) {
        return (attrs->_class == XCB_WINDOW_CLASS_INPUT_OUTPUT)
                && (attrs->map_state == XCB_MAP_STATE_VIEWABLE)
                && !isHidden(win);
    }

    void processChildWindow(xcb_window_t root, xcb_window_t parent, Commands& commands) {
        Xcb::QueryTree tree(m_conn, parent);
        const xcb_window_t* children = tree.children();
        const int numChildren = tree.numChildren();

        for (int i = 0; i < numChildren; i++) {
            const xcb_window_t child = children[i];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            Xcb::Attributes attrs(m_conn, child);
            // Top level windows have WM_STATE set on them.
            if (isViewable(child, attrs) && hasWMState(child)) {
                // Exclude our own graphic element windows (e.g. crosshair, lines, rectangles)
                if (!Graphic::isGraphicWindow(child)) {
                    commands.emplace_back(new TranslatedGeometryCommand(m_conn, root, child, 0, 0));
                }
                return;
            }
        }
    }

    void processRootWindow(xcb_window_t root, Commands& commands) {
        Xcb::QueryTree tree(m_conn, root);
        const xcb_window_t* children = tree.children();
        const int numChildren = tree.numChildren();

        std::vector<Xcb::Attributes> attrs;
        attrs.reserve(numChildren);

        for (int i = 0; i < numChildren; i++) {
            attrs.emplace_back(m_conn, children[i]);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }

        for (int i = 0; i < numChildren; i++) {
            const xcb_window_t child = children[i];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            // Only report on visible windows.
            if (isViewable(child, attrs.at(i))) {
                // Top level windows have WM_STATE set on them.
                if (hasWMState(child)) {
                    // Exclude our own graphic element windows (e.g. crosshair, lines, rectangles)
                    if (!Graphic::isGraphicWindow(child)) {
                        commands.emplace_back(new GeometryCommand(m_conn, child));
                    }
                } else {
                    processChildWindow(root, child, commands);
                }
            }
        }
    }


    xcb_connection_t* m_conn { Xcb::qtConnection() };
    const xcb_setup_t* m_setup;
    Xcb::Atom m_wmStateAtom;
    Xcb::Atom m_netWMStateAtom;
    Xcb::Atom m_netWMStateHiddenAtom;
};


WindowFinder::WindowFinder() : m_updater(new Finder) {
}

WindowFinder::~WindowFinder() {
    delete m_updater;
}

void WindowFinder::refresh() {
    m_firstUpdate = true;
    m_windows = m_updater->scan();
}

QRect WindowFinder::find(const QPoint& position) {
    if (!m_firstUpdate) {
        refresh();
    }

    for (const QRect& winRect : m_windows) {
        if (winRect.contains(position, false)) {
            return winRect;
        }
    }

    return {};
}
