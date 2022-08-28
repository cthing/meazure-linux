#!/bin/bash -e

# Script to build Meazure using CMake.

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

CMAKE_PROG=/opt/cmake/bin/cmake
if [ ! -x $CMAKE_PROG ]; then
  CMAKE_PROG=$(which cmake)
  if [ -z "$CMAKE_PROG" ]; then
    echo "Could not find cmake"
    exit 1
  fi
fi

NINJA_PROG=/opt/ninja/ninja
if [ ! -x $NINJA_PROG ]; then
  NINJA_PROG=$(which ninja)
  if [ -z "$NINJA_PROG" ]; then
    echo "Could not find ninja"
    exit 1
  fi
fi

"$CMAKE_PROG" -B "$BUILD_DIR" -S "$SCRIPT_DIR" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_MAKE_PROGRAM="$NINJA_PROG" -G Ninja

if [ $# -eq 0 ]; then
  exec "$CMAKE_PROG" --build "$BUILD_DIR"
else
  exec "$CMAKE_PROG" --build "$BUILD_DIR" --target "$@"
fi
