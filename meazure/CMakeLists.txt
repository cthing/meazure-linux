set(TOP_SOURCES
    App.cpp
    App.h
    )
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
    ui/fields/TextField.cpp
    ui/fields/TextField.h
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
    )
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
    prefs/models/UnitsPrefsModel.h
    )
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
    position-log/PosLogMgr.h
    )
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
    graphics/Ruler.h
    )
source_group(GRAPHICS FILES ${GRAPHICS_SOURCES})

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
    tools/WindowTool.h
    )
source_group(TOOLS FILES ${TOOLS_SOURCES})

set(CONFIG_SOURCES
    config/Config.h
    config/ConfigMgr.cpp
    config/ConfigMgr.h
    config/ExportedConfig.cpp
    config/ExportedConfig.h
    config/PersistentConfig.cpp
    config/PersistentConfig.h
    )
source_group(CONFIG FILES ${CONFIG_SOURCES})

set(XML_SOURCES
    xml/XMLParser.cpp
    xml/XMLParser.h
    xml/XMLWriter.cpp
    xml/XMLWriter.h
    )
source_group(XML FILES ${XML_SOURCES})

set(UTILS_SOURCES
    utils/Cloaker.h
    utils/EnumIterator.h
    utils/Geometry.h
    utils/GnomeUtils.cpp
    utils/GnomeUtils.h
    utils/LayoutUtils.h
    utils/MathUtils.h
    utils/StringUtils.cpp
    utils/StringUtils.h
    utils/TimedEventLoop.cpp
    utils/TimedEventLoop.h
    utils/XcbUtils.h
    utils/XlibUtils.h
    utils/XRecordUtils.h
    )
source_group(UTILS FILES ${UTILS_SOURCES})

set(ENVIRONMENT_SOURCES
    environment/DesktopDetector.cpp
    environment/DesktopDetector.h
    environment/PointerTracker.cpp
    environment/PointerTracker.h
    environment/ScreenInfo.cpp
    environment/ScreenInfo.h
    environment/ScreenInfoProvider.h
    environment/WindowFinder.cpp
    environment/WindowFinder.h
    environment/WindowTracker.cpp
    environment/WindowTracker.h
    )
source_group(ENVIRONMENT FILES ${ENVIRONMENT_SOURCES})

set(UNITS_SOURCES
    units/CustomUnits.cpp
    units/CustomUnits.h
    units/Units.cpp
    units/Units.h
    units/UnitsMgr.cpp
    units/UnitsMgr.h
    units/UnitsProvider.h
    )
source_group(UNITS FILES ${UNITS_SOURCES})

set(MEAZURE_SOURCES
    ${TOP_SOURCES}
    ${UI_SOURCES}
    ${POSITION_LOG_SOURCES}
    ${PREFS_SOURCES}
    ${GRAPHICS_SOURCES}
    ${TOOLS_SOURCES}
    ${CONFIG_SOURCES}
    ${XML_SOURCES}
    ${UTILS_SOURCES}
    ${ENVIRONMENT_SOURCES}
    ${UNITS_SOURCES}
    )

set(MEAZURE_RESOURCES
    resources/dtd/PositionLog1.dtd
    resources/images/AngleTool.svg
    resources/images/CalWarning.svg
    resources/images/CircleTool.svg
    resources/images/Clipboard.svg
    resources/images/CThingSoftware.svg
    resources/images/CursorTool.svg
    resources/images/GridTool.svg
    resources/images/LineTool.svg
    resources/images/Meazure.png
    resources/images/PauseOff.svg
    resources/images/PauseOn.svg
    resources/images/PointTool.svg
    resources/images/RectangleTool.svg
    resources/images/RulerTool.svg
    resources/images/WindowTool.svg
    )

# To provide access to the QPlatform* classes
set(Qt6_Gui_Platform_INCLUDE_DIRS ${Qt6_Gui_INCLUDE_DIRS})
list(TRANSFORM Qt6_Gui_Platform_INCLUDE_DIRS APPEND "/${Qt6_VERSION}/QtGui")

qt6_add_library(libmeazure STATIC ${MEAZURE_SOURCES})
target_compile_options(libmeazure PRIVATE -fPIC)
target_include_directories(libmeazure PRIVATE
                           ${Qt6_INCLUDE_DIRS}
                           ${Qt6_Gui_Platform_INCLUDE_DIRS}
                           ${glib_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS}
                           ${ICU_INCLUDE_DIRS}
                           "${CMAKE_SOURCE_DIR}"
                           "${CMAKE_CURRENT_BINARY_DIR}"
                           )
qt6_add_resources(libmeazure "resources" PREFIX "/" BASE "resources" FILES ${MEAZURE_RESOURCES})

qt6_add_executable(meazure main.cpp)
target_include_directories(meazure PRIVATE
                           ${Qt6_INCLUDE_DIRS}
                           ${Qt6_Gui_Platform_INCLUDE_DIRS}
                           ${glib_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS}
                           ${ICU_INCLUDE_DIRS}
                           "${CMAKE_SOURCE_DIR}"
                           "${CMAKE_CURRENT_BINARY_DIR}"
                           )

cmake_path(GET Qt6_QXcbIntegrationPlugin_LIB_DIRS PARENT_PATH PLUGINS_DIR)

target_link_directories(meazure PRIVATE
                        "${Qt6_QXcbIntegrationPlugin_LIB_DIRS}"
                        "${PLUGINS_DIR}/iconengines"
                        ${Qt6_Core_LIB_DIRS}
                        )
target_link_libraries(meazure PRIVATE
                      libmeazure
                      Qt6::Widgets
                      Qt6::Gui
                      Qt6::Svg
                      Qt6::Network
                      qxcb
                      qsvgicon
                      Qt6XcbQpa
                      Qt6::Core
                      ${ICU_uc_LIBS}
                      XercesC::XercesC
                      )

# Replace the version variables with the actual values from the project version.
configure_file(AppVersion.h.in AppVersion.h @ONLY)

# Because the ICU library is statically compiled, its data file is not available at runtime.
# Copy the data file to the output directory. The directory will be included in the distribution
# package and ICU will be configured to use that directory.
add_custom_command(TARGET meazure POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E make_directory icu
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICU_RES_DIRS}/*" icu
                   )

# When the app is run from the build directory, it should run in development mode. One aspect of running in
# development mode is that the application state is persisted in the build directory rather than the user's home
# directory or registry. The app will run in development mode if the "meadevmode" marker file is present in the
# same directory as the executable. In addition, regardless of the presence or absence of the marker file,
# development mode can be explicitly enabled or disabled using the "devmode" command line option.
add_custom_command(TARGET libmeazure POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E touch meadevmode
                   )
