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

By default CMake looks for an installed SFML 3 first and only downloads one if none is found. To point it at a specific install:

```sh
cmake -S . -B build -DSFML_DIR=/path/to/sfml/lib/cmake/SFML
# or
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/sfml
```

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

The unit tests (GoogleTest, for the xonix `Grid` and `Player` classes) are built by default; disable them with `-DSFML_GAMES_BUILD_TESTS=OFF`.

```sh
ctest --test-dir build -C Release --output-on-failure
```
