# 16 SFML Games

A launcher for sixteen small games written with [SFML 3](https://www.sfml-dev.org/): Tetris, Doodle Jump, Arkanoid, Snake, Minesweeper, Fifteen Puzzle, Racing, Outrun, Xonix, Bejeweled, Netwalk, Mahjong Solitaire, Tron, Chess, Asteroids (Volleyball is a placeholder that needs Box2D and is not built).

The project uses CMake and builds on Windows (MSVC), Linux (GCC/Clang) and macOS.

## Requirements

- CMake 3.21 or newer
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

The same commands work on every platform:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

On Windows with a Visual Studio generator (multi-config) pass `--config Release` (or `Debug`) to the build step; `CMAKE_BUILD_TYPE` is ignored there. Use `-G Ninja` for a single-config build from a "x64 Native Tools" prompt.

The executable is placed in `build/bin/` (`build/bin/Release/` with multi-config generators). The `images/` folder (and `stockfish.exe` on Windows) is copied next to it after each build.

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

A text menu asks for a letter (the uppercase letter in the game name, for example `T` for Tetris); the game then opens in its own window. Close the window to return to the menu.

### Chess and Stockfish

Chess talks to a [Stockfish](https://stockfishchess.org/) engine (press Space for the computer's move).

- Windows: the bundled `stockfish.exe` is copied next to the executable.
- Linux / macOS: install Stockfish with your package manager (`sudo apt install stockfish`, `sudo dnf install stockfish`, `sudo pacman -S stockfish`, `brew install stockfish`) **before configuring**. CMake searches `PATH` plus `/usr/games`, `/usr/local/games`, `/opt/homebrew/bin` and `/usr/local/bin`, and compiles the found path into the game. If it is not found, CMake prints a warning with these install commands and the game falls back to looking for `stockfish` on `PATH` at runtime. Override with `-DSTOCKFISH_EXECUTABLE=/path/to/stockfish`. If you install Stockfish after configuring, reconfigure with `cmake --fresh -S . -B build` (or delete `STOCKFISH_EXECUTABLE` from the cache). Without Stockfish, chess still runs but the computer's move (Space) will not work.

## Tests

The unit tests (GoogleTest, for the xonix `Grid` and `Player` classes) are built by default; disable them with `-DSFML_GAMES_BUILD_TESTS=OFF`. Every `.cpp` file in `Tests/` is part of the test program, so a new test file only needs CMake to be re-run.

```sh
ctest --test-dir build -C Release --output-on-failure
```

To build only the tests, without downloading or building SFML (this is what CI does):

```sh
cmake -S . -B build-tests -DSFML_GAMES_BUILD_GAME=OFF
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

The test program does not link SFML, so classes under test must not include it.

## Continuous integration

`.github/workflows/build-and-test.yml` runs on every push: a quick Linux job builds and runs the unit tests without SFML, and a Windows job checks the whole game builds.
