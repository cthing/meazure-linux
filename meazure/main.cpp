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

#include "App.h"


int main(int argc, char *argv[]) {
    // Because Meazure relies on the interaction with physical pixels, disable the Qt logical pixels. Handling
    // HiDPI in the user interface is addressed on a case by case basis.
    qputenv("QT_USE_PHYSICAL_DPI", "1");

    // Due to a bug in the fontconfig Conan recipe, the library is built with a package relative font configuration
    // path. This must be overridden to point to the system font directory. If this directory does not exist, the
    // program will use a fallback monospace font.
    qputenv("FONTCONFIG_PATH", "/etc/fonts");

    const App app(argc, argv);
    return App::exec();
}
