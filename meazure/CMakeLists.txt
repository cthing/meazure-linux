set(TOP_SOURCES
    App.cpp
    App.h)
source_group(App FILES ${TOP_SOURCES})

set(UI_SOURCES
    ui/AboutDialog.cpp
    ui/AboutDialog.h
    ui/ColorDisplay.cpp
    ui/ColorDisplay.h
    ui/fields/AbstractDataField.h
    ui/fields/DoubleDataField.cpp
    ui/fields/DoubleDataField.h
    ui/fields/IntegerDataField.cpp
    ui/fields/IntegerDataField.h
    ui/fields/LineEdit.cpp
    ui/fields/LineEdit.h
    ui/fields/TextEdit.cpp
    ui/fields/TextEdit.h
    ui/GlobalShortcuts.cpp
    ui/GlobalShortcuts.h
    ui/GridDialog.cpp
    ui/GridDialog.h
    ui/Magnifier.cpp
    ui/Magnifier.h
    ui/MagnifierControls.cpp
    ui/MagnifierControls.h
    ui/MagnifierSection.cpp
    ui/MagnifierSection.h
    ui/MainWindow.cpp
    ui/MainWindow.h
    ui/MainWindow.cpp
    ui/MainView.cpp
    ui/MainView.h
    ui/ScreenDataSection.cpp
    ui/ScreenDataSection.h
    ui/ToolDataSection.cpp
    ui/ToolDataSection.h
    ui/ToolDataWindow.cpp
    ui/ToolDataWindow.h
    ui/WaylandAlert.cpp
    ui/WaylandAlert.h)
source_group(UI FILES ${UI_SOURCES})

set(PREFS_SOURCES
    prefs/ui/CalibrationPrefsPage.cpp
    prefs/ui/CalibrationPrefsPage.h
    prefs/ui/Caliper.cpp
    prefs/ui/Caliper.h
    prefs/ui/CheckerBoard.cpp
    prefs/ui/CheckerBoard.h
    prefs/ui/PrecisionPrefsPage.cpp
    prefs/ui/PrecisionPrefsPage.h
    prefs/ui/PrefsDialog.cpp
    prefs/ui/PrefsDialog.h
    prefs/ui/PrefsPage.h
    prefs/ui/RulerPrefsPage.cpp
    prefs/ui/RulerPrefsPage.h
    prefs/ui/ToolPrefsPage.cpp
    prefs/ui/ToolPrefsPage.h
    prefs/ui/UnitsPrefsPage.cpp
    prefs/ui/UnitsPrefsPage.h
    prefs/models/CalibrationPrefsModel.cpp
    prefs/models/CalibrationPrefsModel.h
    prefs/models/PrecisionPrefsModel.cpp
    prefs/models/PrecisionPrefsModel.h
    prefs/models/Preference.h
    prefs/models/RulerPrefsModel.cpp
    prefs/models/RulerPrefsModel.h
    prefs/models/ToolPrefsModel.cpp
    prefs/models/ToolPrefsModel.h
    prefs/models/UnitsPrefsModel.cpp
    prefs/models/UnitsPrefsModel.h)
source_group(PREFS FILES ${PREFS_SOURCES})

set(POSITION_LOG_SOURCES
    position-log/io/PosLogIO.h
    position-log/io/PosLogReader.cpp
    position-log/io/PosLogReader.h
    position-log/io/PosLogWriter.cpp
    position-log/io/PosLogWriter.h
    position-log/model/PosLogArchive.h
    position-log/model/PosLogCustomUnits.h
    position-log/model/PosLogDesktop.h
    position-log/model/PosLogInfo.h
    position-log/model/PosLogPosition.h
    position-log/model/PosLogToolData.h
    position-log/model/PosLogScreen.h
    position-log/PosLogManageDlg.cpp
    position-log/PosLogManageDlg.h
    position-log/PosLogMgr.cpp
    position-log/PosLogMgr.h)
source_group(POSITION_LOG FILES ${POSITION_LOG_SOURCES})

set(GRAPHICS_SOURCES
    graphics/Circle.cpp
    graphics/Circle.h
    graphics/Colors.cpp
    graphics/Colors.h
    graphics/Crosshair.cpp
    graphics/Crosshair.h
    graphics/Dimensions.cpp
    graphics/Dimensions.h
    graphics/Graphic.cpp
    graphics/Graphic.h
    graphics/Grid.cpp
    graphics/Grid.h
    graphics/Handle.cpp
    graphics/Handle.h
    graphics/Line.cpp
    graphics/Line.h
    graphics/OriginMarker.cpp
    graphics/OriginMarker.h
    graphics/Plotter.h
    graphics/Rectangle.cpp
    graphics/Rectangle.h
    graphics/Ruler.cpp
    graphics/Ruler.h)
source_group(GRAPHICS FILES ${GRAPHICS_SOURCES})

set(X11_GRAPHICS_SOURCES
    graphics/x11/X11GraphicTag.cpp
    graphics/x11/X11GraphicTag.h)
source_group(X11_GRAPHICS FILES ${X11_GRAPHICS_SOURCES})

set(TOOLS_SOURCES
    tools/AngleTool.cpp
    tools/AngleTool.h
    tools/CircleTool.cpp
    tools/CircleTool.h
    tools/CursorTool.cpp
    tools/CursorTool.h
    tools/GridTool.cpp
    tools/GridTool.h
    tools/LineTool.cpp
    tools/LineTool.h
    tools/OriginTool.cpp
    tools/OriginTool.h
    tools/PointTool.cpp
    tools/PointTool.h
    tools/RadioTool.cpp
    tools/RadioTool.h
    tools/RadioToolTraits.h
    tools/RectangleTool.cpp
    tools/RectangleTool.h
    tools/RulerTool.cpp
    tools/RulerTool.h
    tools/Tool.cpp
    tools/Tool.h
    tools/ToolMgr.cpp
    tools/ToolMgr.h
    tools/WindowTool.cpp
    tools/WindowTool.h)
source_group(TOOLS FILES ${TOOLS_SOURCES})

set(CONFIG_SOURCES
    config/Config.h
    config/ConfigMgr.cpp
    config/ConfigMgr.h
    config/ExportedConfig.cpp
    config/ExportedConfig.h
    config/PersistentConfig.cpp
    config/PersistentConfig.h)
source_group(CONFIG FILES ${CONFIG_SOURCES})

set(XML_SOURCES
    xml/XMLParser.cpp
    xml/XMLParser.h
    xml/XMLWriter.cpp
    xml/XMLWriter.h)
source_group(XML FILES ${XML_SOURCES})

set(UTILS_SOURCES
    utils/Cloaker.h
    utils/EnumIterator.h
    utils/Geometry.h
    utils/HelpUtils.h
    utils/LayoutUtils.h
    utils/MathUtils.h
    utils/PlatformUtils.h
    utils/StringUtils.cpp
    utils/StringUtils.h
    utils/TimedEventLoop.cpp
    utils/TimedEventLoop.h)
source_group(UTILS FILES ${UTILS_SOURCES})

set(X11_UTILS_SOURCES
    utils/x11/XcbUtils.h
    utils/x11/XlibUtils.h
    utils/x11/XRecordUtils.h)
source_group(X11_UTILS FILES ${X11_UTILS_SOURCES})

set(ENVIRONMENT_SOURCES
    environment/CursorTracker.cpp
    environment/CursorTracker.h
    environment/ScreenInfo.cpp
    environment/ScreenInfo.h
    environment/ScreenInfoProvider.h
    environment/WindowFinder.h
    environment/WindowTracker.h
    environment/noop/NoopWindowFinder.cpp
    environment/noop/NoopWindowFinder.h
    environment/noop/NoopWindowTracker.cpp
    environment/noop/NoopWindowTracker.h)
source_group(ENVIRONMENT FILES ${ENVIRONMENT_SOURCES})

set(X11_ENVIRONMENT_SOURCES
    environment/x11/X11WindowFinder.cpp
    environment/x11/X11WindowFinder.h
    environment/x11/X11WindowTracker.cpp
    environment/x11/X11WindowTracker.h)
source_group(X11_ENVIRONMENT FILES ${X11_ENVIRONMENT_SOURCES})

set(UNITS_SOURCES
    units/CustomUnits.cpp
    units/CustomUnits.h
    units/Units.cpp
    units/Units.h
    units/UnitsMgr.cpp
    units/UnitsMgr.h
    units/UnitsProvider.h)
source_group(UNITS FILES ${UNITS_SOURCES})

set(MEAZURE_SOURCES
    ${TOP_SOURCES}
    ${UI_SOURCES}
    ${POSITION_LOG_SOURCES}
    ${PREFS_SOURCES}
    ${GRAPHICS_SOURCES}
    ${X11_GRAPHICS_SOURCES}
    ${TOOLS_SOURCES}
    ${CONFIG_SOURCES}
    ${XML_SOURCES}
    ${UTILS_SOURCES}
    ${X11_UTILS_SOURCES}
    ${ENVIRONMENT_SOURCES}
    ${X11_ENVIRONMENT_SOURCES}
    ${UNITS_SOURCES})

list(APPEND MEAZURE_SOURCES
     ${X11_UTILS_SOURCES})

set(INTERNAL_RESOURCES
    resources/internal/dtd/PositionLog1.dtd
    resources/internal/images/AngleTool.svg
    resources/internal/images/AngleToolDark.svg
    resources/internal/images/CalWarning.svg
    resources/internal/images/CircleTool.svg
    resources/internal/images/CircleToolDark.svg
    resources/internal/images/Clipboard.svg
    resources/internal/images/ClipboardDark.svg
    resources/internal/images/CThingSoftware.svg
    resources/internal/images/CursorTool.svg
    resources/internal/images/CursorToolDark.svg
    resources/internal/images/GridTool.svg
    resources/internal/images/LineTool.svg
    resources/internal/images/LineToolDark.svg
    resources/internal/images/Meazure.png
    resources/internal/images/Meazure.svg
    resources/internal/images/PauseOff.svg
    resources/internal/images/PauseOffDark.svg
    resources/internal/images/PauseOn.svg
    resources/internal/images/PointTool.svg
    resources/internal/images/RectangleTool.svg
    resources/internal/images/RectangleToolDark.svg
    resources/internal/images/RulerTool.svg
    resources/internal/images/WindowTool.svg)

set(EXTERNAL_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources/external)

# To provide access to the QPlatform* classes
set(Qt6_Gui_Platform_INCLUDE_DIRS ${Qt6_Gui_INCLUDE_DIRS})
list(TRANSFORM Qt6_Gui_Platform_INCLUDE_DIRS APPEND "/${Qt6_VERSION}/QtGui")

qt6_add_library(libmeazure STATIC ${MEAZURE_SOURCES})
target_compile_options(libmeazure PRIVATE -fPIC)
target_include_directories(libmeazure PRIVATE
                           ${Qt6_INCLUDE_DIRS}
                           ${Qt6_Gui_Platform_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS}
                           ${ICU_INCLUDE_DIRS}
                           ${CMAKE_SOURCE_DIR}
                           ${CMAKE_CURRENT_BINARY_DIR})
qt6_add_resources(libmeazure "resources" PREFIX "/" BASE "resources/internal" FILES ${INTERNAL_RESOURCES})

set(QT_PLUGINS_DIR "${CONAN_QT_ROOT}/res/archdatadir/plugins")

set(QT_PLUGIN_PLATFORMS_DIR ${QT_PLUGINS_DIR}/platforms)
set(QT_PLUGIN_PLATFORMS_OBJECTS_DIR ${QT_PLUGIN_PLATFORMS_DIR}/objects-${CMAKE_BUILD_TYPE})
set(QT_XCB_INIT_OBJ ${QT_PLUGIN_PLATFORMS_OBJECTS_DIR}/QXcbIntegrationPlugin_init/QXcbIntegrationPlugin_init.cpp.o)
set(QT_WAYLAND_INIT_OBJ ${QT_PLUGIN_PLATFORMS_OBJECTS_DIR}/QWaylandIntegrationPlugin_init/QWaylandIntegrationPlugin_init.cpp.o)

set(QT_PLUGIN_SHELLS_DIR ${QT_PLUGINS_DIR}/wayland-shell-integration)
set(QT_PLUGIN_SHELLS_OBJECTS_DIR ${QT_PLUGIN_SHELLS_DIR}/objects-${CMAKE_BUILD_TYPE})
set(QT_XDGSHELL_INIT_OBJ ${QT_PLUGIN_SHELLS_OBJECTS_DIR}/QWaylandXdgShellIntegrationPlugin_init/QWaylandXdgShellIntegrationPlugin_init.cpp.o)

set(QT_PLUGIN_ICONENGINES_DIR ${QT_PLUGINS_DIR}/iconengines)
set(QT_PLUGIN_ICONENGINES_OBJECTS_DIR ${QT_PLUGIN_ICONENGINES_DIR}/objects-${CMAKE_BUILD_TYPE})
set(QT_PLUGINS_SVGICON_INIT_OBJ ${QT_PLUGIN_ICONENGINES_OBJECTS_DIR}/QSvgIconPlugin_init/QSvgIconPlugin_init.cpp.o)

add_library(pluginInits OBJECT IMPORTED)
set_property(TARGET pluginInits PROPERTY IMPORTED_OBJECTS
             ${QT_XCB_INIT_OBJ}
             ${QT_WAYLAND_INIT_OBJ}
             ${QT_XDGSHELL_INIT_OBJ}
             ${QT_PLUGINS_SVGICON_INIT_OBJ})

set(CMAKE_SKIP_RPATH TRUE)

qt6_add_executable(meazure main.cpp $<TARGET_OBJECTS:pluginInits>)
target_include_directories(meazure PRIVATE
                           ${Qt6_INCLUDE_DIRS}
                           ${Qt6_Gui_Platform_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS}
                           ${ICU_INCLUDE_DIRS}
                           ${CMAKE_SOURCE_DIR}
                           ${CMAKE_CURRENT_BINARY_DIR})

target_link_directories(meazure PRIVATE
                        ${QT_PLUGIN_PLATFORMS_DIR}
                        ${QT_PLUGIN_SHELLS_DIR}
                        ${QT_PLUGIN_ICONENGINES_DIR}
                        ${Qt6_Core_LIB_DIRS})
target_link_libraries(meazure PRIVATE
                      libmeazure
                      Qt6::Widgets
                      Qt6::Gui
                      Qt6::Svg
                      Qt6::Network
                      qxcb
                      qwayland-generic
                      qsvgicon
                      xdg-shell
                      Qt6XcbQpa
                      Qt6WaylandClient
                      Qt6::Core
                      ${ICU_uc_LIBS}
                      XercesC::XercesC)

# Replace the version variables with the actual values from the project version.
configure_file(AppVersion.h.in AppVersion.h @ONLY)

# Because the ICU library is statically compiled, its data file is not available at runtime.
# Copy the data file to the output directory. The directory will be included in the distribution
# package and ICU will be configured to use that directory.
add_custom_target(copy_icu
                  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ICU_RES_DIRS}" "${CMAKE_CURRENT_BINARY_DIR}/icu"
                  COMMENT "Copying icu files")
add_dependencies(libmeazure copy_icu)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_CURRENT_BINARY_DIR}/icu")

# Copy the user manual for distribution.
set(HELP_SOURCE_DIR "${EXTERNAL_RESOURCES_DIR}/help")
set(HELP_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/help")
file(GLOB_RECURSE HELP_SOURCE_FILES "${HELP_SOURCE_DIR}/*")
add_custom_target(copy_help
                  COMMAND ${CMAKE_COMMAND} -E copy_directory "${HELP_SOURCE_DIR}" "${HELP_BINARY_DIR}"
                  COMMAND ${CMAKE_COMMAND} -DINFILE=${HELP_SOURCE_DIR}/help.html -DOUTFILE=${HELP_BINARY_DIR}/help.html -DMAJOR_VERSION=${meazure_linux_VERSION_MAJOR} -DMINOR_VERSION=${meazure_linux_VERSION_MINOR} -P ${CMAKE_CURRENT_SOURCE_DIR}/configureFile.cmake
                  DEPENDS ${HELP_SOURCE_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/configureFile.cmake
                  COMMENT "Copying help files")
add_dependencies(libmeazure copy_help)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${HELP_BINARY_DIR})

# Copy the manual page for distribution.
set(MANPAGE_SOURCE_FILE "${EXTERNAL_RESOURCES_DIR}/manpage/meazure.1")
set(MANPAGE_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/manpage")
set(MANPAGE_BINARY_FILE "${MANPAGE_BINARY_DIR}/meazure.1")
add_custom_target(manpage
                  COMMAND ${CMAKE_COMMAND} -DINFILE=${MANPAGE_SOURCE_FILE} -DOUTFILE=${MANPAGE_BINARY_FILE} -DMAJOR_VERSION=${meazure_linux_VERSION_MAJOR} -DMINOR_VERSION=${meazure_linux_VERSION_MINOR} -DBUILD_DATE=${BUILD_DATE} -P ${CMAKE_CURRENT_SOURCE_DIR}/configureFile.cmake
                  COMMAND gzip -9 -n --force ${MANPAGE_BINARY_FILE}
                  DEPENDS ${MANPAGE_SOURCE_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/configureFile.cmake
                  COMMENT "Copying manual page")
add_dependencies(meazure manpage)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${MANPAGE_BINARY_DIR})

# When the app is run from the build directory, it should run in development mode. One aspect of running in
# development mode is that the application state is persisted in the build directory rather than the user's home
# directory or registry. The app will run in development mode if the "meadevmode" marker file is present in the
# same directory as the executable. In addition, regardless of the presence or absence of the marker file,
# development mode can be explicitly enabled or disabled using the "devmode" command line option.
add_custom_target(dev_mode
                  COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/meadevmode"
                  BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/meadevmode"
                  COMMENT "Creating dev mode marker")
add_dependencies(libmeazure dev_mode)

# Validate the desktop integration files
set(DESKTOP_RESOURCES_DIR ${EXTERNAL_RESOURCES_DIR}/desktop)

set(DESKTOP_FILE ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure.desktop)
add_custom_target(validate_desktop
                  COMMAND desktop-file-validate --warn-kde ${DESKTOP_FILE}
                  DEPENDS ${DESKTOP_FILE}
                  COMMENT "Validating desktop integration file")
add_dependencies(meazure validate_desktop)

set(METAINFO_FILE ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure.metainfo.xml)
add_custom_target(validate_metainfo
                  COMMAND appstreamcli validate --pedantic ${METAINFO_FILE}
                  DEPENDS ${METAINFO_FILE}
                  COMMENT "Validating metainfo file")
add_dependencies(meazure validate_metainfo)

# Install
install(TARGETS meazure RUNTIME DESTINATION "usr/bin")
install(DIRECTORY ${HELP_BINARY_DIR} DESTINATION "usr/share/meazure")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/icu" DESTINATION "usr/share/meazure")
install(FILES ${MANPAGE_BINARY_FILE}.gz DESTINATION "usr/share/man/man1")
install(FILES ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure.desktop DESTINATION "usr/share/applications")
install(FILES ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure.metainfo.xml DESTINATION "usr/share/metainfo")
install(FILES ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure.xml DESTINATION "usr/share/mime/packages")
install(FILES ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure.svg DESTINATION "usr/share/icons/hicolor/scalable/apps")
install(FILES ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure-config.svg DESTINATION "usr/share/icons/hicolor/scalable/apps")
install(FILES ${DESKTOP_RESOURCES_DIR}/org.cthing.meazure-positions.svg DESTINATION "usr/share/icons/hicolor/scalable/apps")
install(FILES ${EXTERNAL_RESOURCES_DIR}/packaging/copyright.txt DESTINATION "usr/share/doc/meazure" RENAME copyright)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/install_manifest.txt)

# Packaging
set(DEB_PACKAGE_NAME meazure_${meazure_linux_VERSION_MAJOR}.${meazure_linux_VERSION_MINOR}.${meazure_linux_VERSION_PATCH}-${meazure_linux_VERSION_TWEAK}_amd64.deb)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME "meazure")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Tool for easily measuring and capturing portions of the screen")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${EXTERNAL_RESOURCES_DIR}/packaging/description.txt)
set(CPACK_PACKAGE_VENDOR "C Thing Software")
set(CPACK_VERBATIM_VARIABLES TRUE)
set(CPACK_STRIP_FILES TRUE)
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
set(CPACK_DEBIAN_PACKAGE_RELEASE ${meazure_linux_VERSION_TWEAK})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "C Thing Software <meafeedback@cthing.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS fontconfig)
set(CPACK_DEBIAN_PACKAGE_SECTION graphics)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS YES)
include(CPack)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/${DEB_PACKAGE_NAME})

add_custom_target(package_sha256
                  COMMAND sha256sum ${CMAKE_BINARY_DIR}/${DEB_PACKAGE_NAME} > ${CMAKE_BINARY_DIR}/${DEB_PACKAGE_NAME}.sha256)
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/${DEB_PACKAGE_NAME}.sha256)

add_custom_target(lintian
                  COMMAND lintian -c --suppress-tags embedded-library,debian-changelog-file-missing -i
                        ${CMAKE_BINARY_DIR}/${DEB_PACKAGE_NAME}
                  COMMENT "Validating package ${DEB_PACKAGE_NAME}")
