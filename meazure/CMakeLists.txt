set(TOP_SOURCES
    App.cpp
    App.h
    )
source_group(App FILES ${TOP_SOURCES})

set(UI_SOURCES
    ui/ColorData.cpp
    ui/ColorData.h
    ui/DataField.cpp
    ui/DataField.h
    ui/Magnifier.cpp
    ui/Magnifier.h
    ui/MagnifierSection.cpp
    ui/MagnifierSection.h
    ui/MagnifierZoom.cpp
    ui/MagnifierZoom.h
    ui/MainWindow.cpp
    ui/MainWindow.h
    ui/MainWindow.cpp
    ui/MainView.cpp
    ui/MainView.h
    ui/ScreenDataSection.cpp
    ui/ScreenDataSection.h
    ui/ToolDataSection.cpp
    ui/ToolDataSection.h
    )
source_group(UI FILES ${UI_SOURCES})

set(GRAPHICS_SOURCES
    graphics/Colors.cpp
    graphics/Colors.h
    )
source_group(GRAPHICS FILES ${GRAPHICS_SOURCES})

set(PROFILE_SOURCES
    profile/FileProfile.cpp
    profile/FileProfile.h
    profile/Profile.h
    profile/SettingsProfile.cpp
    profile/SettingsProfile.h
    )
source_group(Profiles FILES ${PROFILE_SOURCES})

set(XML_SOURCES
    xml/XMLParser.cpp
    xml/XMLParser.h
    xml/XMLWriter.cpp
    xml/XMLWriter.h
    )
source_group(XML FILES ${XML_SOURCES})

set(UTILS_SOURCES
    utils/MathUtils.h
    utils/StringUtils.cpp
    utils/StringUtils.h
    )
source_group(UTILS FILES ${UTILS_SOURCES})

set(MEAZURE_SOURCES
    ${TOP_SOURCES}
    ${UI_SOURCES}
    ${GRAPHICS_SOURCES}
    ${PROFILE_SOURCES}
    ${XML_SOURCES}
    ${UTILS_SOURCES}
    )

set(MEAZURE_RESOURCES
    resources/images/CalWarning.svg
    resources/images/CircleTool.svg
    resources/images/Clipboard.svg
    resources/images/CursorTool.svg
    resources/images/GridTool.svg
    resources/images/LineTool.svg
    resources/images/Pause.svg
    resources/images/PointTool.svg
    resources/images/RectangleTool.svg
    resources/images/RulerTool.svg
    resources/images/WindowTool.svg
    )

qt6_add_library(libmeazure STATIC ${MEAZURE_SOURCES})
target_compile_options(libmeazure PRIVATE -fPIC)
target_include_directories(libmeazure PRIVATE
                           ${Qt6_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS}
                           "${CMAKE_SOURCE_DIR}"
                           "${CMAKE_CURRENT_BINARY_DIR}"
                           )
qt6_add_resources(libmeazure "resources" PREFIX "/" BASE "resources" FILES ${MEAZURE_RESOURCES})

qt6_add_executable(meazure main.cpp)
target_include_directories(meazure PRIVATE
                           ${Qt6_INCLUDE_DIRS}
                           ${XercesC_INCLUDE_DIRS}
                           "${CMAKE_SOURCE_DIR}"
                           "${CMAKE_CURRENT_BINARY_DIR}"
                           )
target_link_directories(meazure PRIVATE
                        "${Qt6_RES_DIRS}/archdatadir/plugins/platforms"
                        "${Qt6_RES_DIRS}/archdatadir/plugins/iconengines"
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
                      XercesC::XercesC
                      )

# Replace the version variables with the actual values from the project version.
configure_file(AppVersion.h.in AppVersion.h @ONLY)