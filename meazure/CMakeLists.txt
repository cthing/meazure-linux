set(TOP_SRCS
    App.cpp
)
source_group(App FILES ${TOP_SRCS})

set(MEAZURE_SRCS
    ${TOP_SRCS}
)

qt6_add_executable(meazure ${MEAZURE_SRCS})
target_link_directories(meazure PRIVATE "${Qt6_RES_DIRS}/archdatadir/plugins/platforms")
target_link_directories(meazure PRIVATE ${Qt6_Core_LIB_DIRS})
target_link_libraries(meazure PRIVATE Qt6::Widgets Qt6::Gui qxcb Qt6XcbQpa Qt6::Core)
