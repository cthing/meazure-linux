# C Thing Software wrapper around the conan.cmake functionality.

include(${PROJECT_SOURCE_DIR}/dev/conan/conan.cmake)

set(CTHING_CONAN_GENERATOR cmake_find_package cmake_paths)
set(CTHING_FIND_PACKAGE_OPT "")

macro(CTHING_CONAN_INSTALL)
    conan_cmake_autodetect(settings)
    conan_cmake_install(PATH_OR_REFERENCE . BUILD missing REMOTE conancenter SETTINGS ${settings})

    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
endmacro()
