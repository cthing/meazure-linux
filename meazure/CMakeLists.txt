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

set(MEAZURE_SOURCES
    ${TOP_SOURCES}
    ${UI_SOURCES}
    )

set(MEAZURE_RESOURCES
    resources/images/CursorTool.svg
    resources/images/PointTool.svg
    resources/images/LineTool.svg
    resources/images/RectangleTool.svg
    resources/images/CircleTool.svg
    resources/images/WindowTool.svg
    resources/images/RulerTool.svg
    resources/images/GridTool.svg
    resources/images/CalWarning.svg
    resources/images/Clipboard.svg
    resources/images/Pause.svg
    )

qt6_add_executable(meazure ${MEAZURE_SOURCES})
qt6_add_resources(meazure "resources" PREFIX "/" BASE "resources" FILES ${MEAZURE_RESOURCES})
target_include_directories(meazure PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_directories(meazure PRIVATE
                        "${Qt6_RES_DIRS}/archdatadir/plugins/platforms"
                        "${Qt6_RES_DIRS}/archdatadir/plugins/iconengines"
                        ${Qt6_Core_LIB_DIRS}
                        )
target_link_libraries(meazure PRIVATE Qt6::Widgets Qt6::Gui Qt6::Svg qxcb qsvgicon Qt6XcbQpa Qt6::Core)

# Replace the version variables with the actual values from the project version.
configure_file(AppVersion.h.in AppVersion.h @ONLY)
