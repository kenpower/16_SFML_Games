# Building, running and testing

Full setup instructions for the 16 SFML Games project. For a short version, see the [README](https://github.com/kenpower/16_SFML_Games/blob/main/README.md).

## Requirements

- CMake 3.21 or newer (3.25 or newer to use the presets below)
- A C++17 compiler (MSVC 2019+, GCC 9+, Clang 10+, Apple Clang)
- Git and an internet connection on the first configure. If SFML 3 is not installed, CMake downloads and builds SFML 3.0.2 (static, no audio/network). GoogleTest is downloaded for the tests.

### Linux packages

SFML 3 needs the X11/OpenGL development packages when it is built from source:

```sh
# Debian / Ubuntu
sudo apt install build-essential cmake git \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev \
    libfreetype-dev libgl1-mesa-dev libegl1-mesa-dev
```

Fedora: `sudo dnf install gcc-c++ cmake git libX11-devel libXrandr-devel libXcursor-devel libXi-devel systemd-devel freetype-devel mesa-libGL-devel mesa-libEGL-devel`

### macOS

Install Xcode command line tools and CMake (`xcode-select --install`, `brew install cmake`). No other packages are needed.

## Configure and build

### Visual Studio

On Windows with Visual Studio, use the IDE rather than the command line:

- **Open:** *File → Open → Folder…* (not *Open → Project/Solution*). Visual Studio reads `CMakePresets.json` and configures the project.
- **Configuration:** choose **Debug** or **Release** in the toolbar drop-down. Debug builds go to `build-debug/`, and Release builds to `build/`.
- **Build:** *Build → Build All*, or build one target from *Solution Explorer → Switch Views → CMake Targets View*.
- **Run or debug:** choose the target (e.g. `16_SFML_Games.exe`) in *Select Startup Item* and press **F5**.
- **Tests:** *Test → Test Explorer → Run All Tests*. The GoogleTest tests are found through CTest. If Test Explorer is empty, build all targets first, then refresh it.

**If the game's images don't load**, the debugger is running the executable from the wrong folder. Right-click the target in the CMake Targets view, choose *Add Debug Configuration* (or use *Debug → Debug and Launch Settings for 16_SFML_Games*), and set `"currentDir"` to the folder that holds the `.exe` and its `images/` folder, e.g. `"currentDir": "${workspaceRoot}\\build-debug\\bin"`.

If Visual Studio says it can't find CMake or a compiler, run the Visual Studio Installer and add the **Desktop development with C++** workload.

### Command line

### Quick start (presets)

`CMakePresets.json` holds the usual options, so one command configures, builds and runs the tests:

```sh
cmake --workflow --preset default    # Release build in build/
cmake --workflow --preset debug      # Debug build in build-debug/
```

Or run the steps one at a time:

```sh
cmake --preset default               # configure
cmake --build --preset default       # build (repeat after editing code)
ctest --preset default               # run the tests
```

Visual Studio (*File → Open → Folder*) and VS Code with the CMake Tools extension read the presets too: pick **Release** or **Debug** from the preset list.

### Without presets

The same commands work on every platform:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

On Windows with a Visual Studio generator (multi-config) pass `--config Release` (or `Debug`) to the build step; `CMAKE_BUILD_TYPE` is ignored there. Use `-G Ninja` for a single-config build from a "x64 Native Tools" prompt.

### Windows: `cmake` is not recognized

Visual Studio installs its own CMake, but it is not on `PATH` in a normal terminal. Use the **Developer PowerShell for VS** (or **x64 Native Tools Command Prompt**) from the Start menu, or load it into an existing PowerShell:

```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -SkipAutomaticLocation
```

(adjust the path to your Visual Studio install). VS Code's CMake Tools extension finds Visual Studio by itself, so this is only needed for the terminal.

### Output

The executable is placed in `build/bin/` (`build/bin/Release/` with multi-config generators; `build-debug/...` for the debug preset). The `images/` folder (and `stockfish.exe` on Windows) is copied next to it after each build.

### Using an installed SFML

By default CMake looks for an installed SFML 3 first and only downloads one if none is found. It looks in:

- `-DSFML_DIR=<sfml>/lib/cmake/SFML`, `-DSFML_ROOT=<sfml>` or the `SFML_ROOT` environment variable
- `-DCMAKE_PREFIX_PATH=<sfml>` or the `CMAKE_PREFIX_PATH` environment variable
- `-DSFML_SDK=<folder>` or the `SFML_SDK` environment variable, pointing at an SFML SDK folder or at a folder containing `SFML-3*` SDKs
- on Windows, `C:\SFML-3*` (also the nested `C:\SFML-3.0.2\SFML-3.0.2` that "Extract All" creates), `C:\SFML\SFML-3*` and `C:\SFML` (newest version first)
- CMake's standard locations (`PATH`, `Program Files`, `/usr`, `/usr/local`, Homebrew)

```sh
cmake -S . -B build -DSFML_SDK=C:/SFML-3.0.2
```

Before using an installed SFML, CMake builds a tiny test program against it. If that fails (for example, a 32-bit SDK or one built for a different compiler), the install is ignored and SFML is downloaded instead. The configure output says which one was used.

To always download and build SFML instead, use `-DSFML_GAMES_FETCH_SFML=ON`. For an installed static SFML, also pass `-DSFML_STATIC_LIBRARIES=ON`. On Windows with an installed shared SFML, the DLLs are copied next to the executable automatically.

## Run

The games load their assets with relative paths (`images/...`), so **run the executable from its own folder**:

```sh
cd build/bin            # build/bin/Release on Windows with Visual Studio generators
./16_SFML_Games         # 16_SFML_Games.exe on Windows
```

In VS Code (CMake Tools) or Visual Studio, choose `16_SFML_Games` as the launch / startup target and make sure the debugger's working directory is the executable's folder.

A text menu asks for a letter (the uppercase letter in the game name, for example `T` for Tetris); the game then opens in its own window. Close the window to return to the menu.

### Chess and Stockfish

Chess talks to a [Stockfish](https://stockfishchess.org/) engine (press Space for the computer's move).

- Windows: the bundled `stockfish.exe` is copied next to the executable.
- Linux / macOS: install Stockfish with your package manager (`sudo apt install stockfish`, `sudo dnf install stockfish`, `sudo pacman -S stockfish`, `brew install stockfish`) **before configuring**. CMake searches `PATH` plus `/usr/games`, `/usr/local/games`, `/opt/homebrew/bin` and `/usr/local/bin`, and compiles the found path into the game. If it is not found, CMake prints a warning with these install commands and the game falls back to looking for `stockfish` on `PATH` at runtime. Override with `-DSTOCKFISH_EXECUTABLE=/path/to/stockfish`. If you install Stockfish after configuring, reconfigure with `cmake --fresh -S . -B build` (or delete `STOCKFISH_EXECUTABLE` from the cache). Without Stockfish, chess still runs but the computer's move (Space) will not work.

## Tests

The unit tests (GoogleTest: the Xonix `Grid` and `Player` classes, plus one test file per assignment game, `Tests/<game>_test.cpp`) are built by default; disable them with `-DSFML_GAMES_BUILD_TESTS=OFF`. Every `.cpp` file in `Tests/` is part of the test program, so a new test file only needs CMake to be re-run.

```sh
ctest --preset default                                   # with presets
ctest --test-dir build -C Release --output-on-failure    # without
```

Build everything (or at least the `Tests` target) before running ctest; a test executable that has not been built shows up as `..._NOT_BUILT` and fails.

To build only the tests, without downloading or building SFML (this is what CI does):

```sh
cmake -S . -B build-tests -DSFML_GAMES_BUILD_GAME=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

The test program does not link SFML, so classes under test must not include it.

## Continuous integration

`.github/workflows/build-and-test.yml` runs on every push: a quick Linux job builds and runs the unit tests without SFML, and a Windows job checks the whole game builds.
