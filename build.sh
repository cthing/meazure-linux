#!/bin/bash -e

# Script to build Meazure using CMake and Conan.

SCRIPT_DIR="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"
BUILD_BASE_DIR="$SCRIPT_DIR/build"

function usage {
        echo "Usage: $(basename "$0") [-t release|debug] [target]" 2>&1
        echo '   -t release|debug   Specifies the type of build'
        echo '   target             Build target'
        exit 1
}

TYPE=release

while getopts "t:" arg; do
  case ${arg} in
    t)
      TYPE=$OPTARG
      ;;
    *)
      echo
      usage
      ;;
  esac
done
shift "$((OPTIND-1))"

# Determine build information based on the build type.

if [ "$TYPE" = 'release' ]; then
  BUILD_TYPE=Release
  BUILD_DIR="$BUILD_BASE_DIR/$TYPE"
elif [ "$TYPE" = 'debug' ]; then
  BUILD_TYPE=Debug
  BUILD_DIR="$BUILD_BASE_DIR/$TYPE"
else
  echo "Invalid build type: ${TYPE}."
  echo
  usage
fi

# Locate the CMake program

CMAKE_PROG=/opt/cmake/bin/cmake
if [ ! -x $CMAKE_PROG ]; then
  CMAKE_PROG=$(which cmake)
  if [ -z "$CMAKE_PROG" ]; then
    echo "Could not find cmake"
    exit 1
  fi
fi

# Locate the Ninja program

NINJA_PROG=/opt/ninja/ninja
if [ ! -x $NINJA_PROG ]; then
  NINJA_PROG=$(which ninja)
  if [ -z "$NINJA_PROG" ]; then
    echo "Could not find ninja"
    exit 1
  fi
fi

# Locate the Conan program

CONAN_PROG=/usr/local/bin/conan
if [ ! -x CONAN_PROG ]; then
  CONAN_PROG=$(which conan)
  if [ -z "$CONAN_PROG" ]; then
    echo "Could not find conan"
    exit 1
  fi
fi

# Detect or create a default Conan profile

CONAN_PROFILE="${CONAN_PROFILE:-default}"
if ! "$CONAN_PROG" profile list | grep -q "^${CONAN_PROFILE}\$"; then
    echo "Creating conan profile '${CONAN_PROFILE}'"
    "$CONAN_PROG" profile detect --force --name="${CONAN_PROFILE}"
fi

# Install dependencies using Conan

mkdir -p "$BUILD_DIR"

TOOLCHAIN_FILE="$BUILD_DIR/build/$BUILD_TYPE/generators/conan_toolchain.cmake"

if [ ! -f "$TOOLCHAIN_FILE" ] || [ "$SCRIPT_DIR/conanfile.txt" -nt "$TOOLCHAIN_FILE" ]; then
    echo "Installing dependencies"
    "$CONAN_PROG" install "$SCRIPT_DIR" \
        --output-folder="$BUILD_DIR" \
        --build=missing \
        --profile="$CONAN_PROFILE" \
        -s build_type="$BUILD_TYPE"
fi

# Configure CMake

"$CMAKE_PROG" -B "$BUILD_DIR" \
    -S "$SCRIPT_DIR" \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_MAKE_PROGRAM="$NINJA_PROG" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -G Ninja

# Run the build

if [ $# -eq 0 ]; then
  exec "$CMAKE_PROG" --build "$BUILD_DIR"
else
  exec "$CMAKE_PROG" --build "$BUILD_DIR" --target "$@"
fi
