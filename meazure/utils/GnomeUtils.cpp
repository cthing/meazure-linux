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

#include "GnomeUtils.h"
#include <gio/gio.h>


static constexpr const char* k_gnomeSchemaId = "org.gnome.desktop.interface";
static constexpr const char* k_cursorSizeKey = "cursor-size";


int GnomeUtils::cursorSize() {
    GSettingsSchemaSource* schemaSource = g_settings_schema_source_get_default();
    if (schemaSource == nullptr) {
        return -1;
    }

    GSettingsSchema* schema = g_settings_schema_source_lookup(schemaSource, k_gnomeSchemaId, FALSE);
    if (schema == nullptr) {
        return -1;
    }

    if (g_settings_schema_has_key(schema, k_cursorSizeKey) == FALSE) {
        g_settings_schema_unref(schema);
        return -1;
    }

    GSettings* gSettings = g_settings_new_full(schema, nullptr, nullptr);
    const int cursorSize = g_settings_get_int(gSettings, k_cursorSizeKey);

    g_settings_schema_unref(schema);
    g_object_unref(gSettings);

    return cursorSize;
}
