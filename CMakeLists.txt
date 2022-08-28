cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(meazure_linux
        VERSION 1.0.0.1
        DESCRIPTION "A tool for easily measuring and capturing portions of the screen."
        HOMEPAGE_URL "https://github.com/cthing/meazure-linux")

if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    message(FATAL_ERROR "Meazure only builds on Linux")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(MAIN_DIR "${SRC_DIR}/main")
set(TEST_DIR "${SRC_DIR}/test")
set(DEV_DIR "${PROJECT_SOURCE_DIR}/dev")

include(${DEV_DIR}/conan/cthing-conan.cmake)

conan_cmake_configure(REQUIRES
                      qt/6.3.1
                      GENERATORS ${CTHING_CONAN_GENERATOR})

CTHING_CONAN_INSTALL()

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_program(CMAKE_LINT_PROGRAM "cmake-lint" REQUIRED)

qt_standard_project_setup()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}")

file(GLOB_RECURSE BUILD_FILES CMakeLists.txt)
add_custom_target(cmakelint
                  COMMAND ${CMAKE_LINT_PROGRAM} ${BUILD_FILES} --config ${DEV_DIR}/cmake-lint/config.py
                  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

add_subdirectory(meazure)
add_subdirectory(test)
