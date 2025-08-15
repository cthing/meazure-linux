# Building Meazure From Source

Meazure is a Qt program and must be built on Linux. These instructions describe how to build Meazure
for Ubuntu 20.04 using CMake.

1. Download and install CMake 3.24 or newer for Linux x86_64 from <https://cmake.org/download/>
2. Download and install Ninja 1.11.0 or newer for Linux <https://github.com/ninja-build/ninja/releases>
3. Ensure that `cmake` and `ninja` are in the PATH
4. Install Python
   ```shell
   sudo apt-get install python3
   sudo apt-get install python3-pip
   ```
5. Install the [Conan C/C++ Package Manager](https://conan.io/) version 2.*
   ```shell
   pip install conan
   ```
6. Install the [cmakelang tools](https://pypi.org/project/cmakelang/)
   ```shell
   pip install cmakelang
   ```
7. Install [clang-tidy-15](https://apt.llvm.org/)
8. Install [Doxygen](https://doxygen.nl/index.html)
   ```shell
   sudo apt-get install doxygen
   sudo apt-get install doxygen-latex
   sudo apt-get install graphviz
   ```
9. Meazure uses the [Qt Framework](https://www.qt.io/). This dependency is obtained and built locally by
   the Conan package manager. Becausse it is built locally from source, Qt requires a number of development
   packages to be installed on the system. Conan lists the missing dependencies during the build. Besides the
   compiler and its toolchain, you are given the option to either install the development packages manually
   or have Conan do it automatically. The following are the packages that need to be installed on the system
   (note that the actual packages may vary).
   ```shell
   sudo apt-get install libfontenc-dev libice-dev libgl-dev libsm-dev libx11-xcb-dev \
                        libxaw7-dev libxcb-cursor-dev libxcb-dri2-0-dev libxcb-dri3-dev \
                        libxcb-glx0-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev \
                        libxcb-present-dev libxcb-randr0-dev libxcb-render-util0-dev \
                        libxcb-render0-dev libxcb-res0-dev libxcb-shape0-dev libxcb-sync-dev \
                        libxcb-util-dev libxcb-util0-dev libxcb-xfixes0-dev libxcb-xinerama0-dev \
                        libxcb-xkb-dev libxcomposite-dev libxcursor-dev libxdamage-dev libxext-dev \
                        libxfixes-dev libxi-dev libxinerama-dev libxkbfile-dev libxmu-dev \
                        libxmuu-dev libxpm-dev libxrandr-dev libxrender-dev libxres-dev libxss-dev \
                        libxt-dev libxtst-dev libxv-dev libxvmc-dev libxxf86vm-dev uuid-dev
   ```
10. The following targets are of interest:
    - **project_full_build** - Builds the Meazure executable, runs all tests, generates API documentation, lints the
      CMakeLists.txt files and creates Debian package.
    - **meazure** - Builds the Meazure executable.
    - **project_test** - Runs all unit tests.
    - **project_package** - Creates the Debian package.
    - **clean** - Deletes all build artifacts.
    - **cmakelint** - Lints the CMakeLists.txt files.
    - **apidocs** - Generates the API documentation.

    To run these targets, use the following command:
    ```shell
    ./build.sh -t [release|debug] <target>
    ```

### CLion IDE Configuration

This section describes the configuration of the [JetBrains CLion IDE](https://www.jetbrains.com/clion/) so that it
can be used to develop, run and debug the Meazure project. The primary goal of this configuration is to ensure that
the IDE uses the same toolchain as the command line build. In addition, to avoid conflicts, this configuration ensures
that the IDE output directory is separate from the command line build output directory.

Note that the tool installation location may vary from those shown.

1. Start the CLion IDE
2. Open the Meazure project
3. Configurea a toolchain
   1. Select **Settings > Build, Execution, Deployment > Toolchains**
   2. Press **+** and **System** to add a new toolchain
   3. Specify **Name: Meazure**
   4. Specify **CMake: /opt/cmake/bin/cmake**
   5. Specify **Build Tool: /opt/ninja/ninja**
   6. Press **OK**
4. Configure CMake profiles
   1. Select **Settings > Build, Execution, Deployment > CMake**
   2. Select the **Debug** profile
   3. For **Toolchain** select **Meazure**
   4. Verify the name of the profile is changed to **Release-Meazure**
   5. For **Build directory** specify **out/debug**
   6. Press **Apply**
   7. Press **+**
   8. Select the **Release** profile
   9. For **Toolchain** select **Meazure**
   10. Verify the name of the profile is changed to **Release-Meazure**
   11. For **Build directory** specify **out/release**
   12. Press **OK**
   13. Delete any build directories other than **out**
5. Configure clang-tidy
   1. Select **Settings > Languages and Frameworks > C/C++ > Clang-Tidy**
   2. Check the box **Use external Clang-Tidy instead of the built-in one**
   3. Specify **/usr/bin/clang-tidy-15**
   4. Press **OK**
