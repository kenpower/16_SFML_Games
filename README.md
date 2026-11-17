# 16 SFML Games

Sixteen small classic games written in C++17 with [SFML 3](https://www.sfml-dev.org/), behind one text-menu launcher: Tetris, Doodle Jump, Arkanoid, Snake, Minesweeper, Fifteen Puzzle, Racing, Outrun, Xonix, Bejeweled, Netwalk, Mahjong Solitaire, Tron, Chess and Asteroids. (Volleyball is a placeholder that needs Box2D and is not built.)

The games work, but most of them are written as one long loop that mixes the game's rules with graphics, keyboard, time and random numbers. That makes them good material for learning to **read, refactor and test unfamiliar code**, which is what this repository is used for.

## Start here

| Document | What it is |
|----------|------------|
| [SFML_Game_Refactor_Test_Worksheet](https://github.com/kenpower/SFML_Game_Refactor_Test_Worksheet) | **The three-week student project** (briefs, `NOTES.md` template, git cheat sheet). It uses a cut-down copy of this repository with just Tetris, Minesweeper, Tron, Netwalk and Xonix. |
| [seams_lecture_notes.md](https://github.com/kenpower/split-at-the-seam/blob/main/seams_lecture_notes.md) | Lecture notes: what a seam is, why you split an app there, how to find it, and why `rand()`, the clock and the keyboard stay outside |
| [howto_split.md](https://github.com/kenpower/split-at-the-seam/blob/main/howto_split.md) | Step-by-step worked example: splitting **Catcher**, a small game made for this purpose, into an SFML-free class and testing it |
| [BUILDING.md](https://github.com/kenpower/16_SFML_Games/blob/main/BUILDING.md) | Full setup, build, run and test instructions for Windows, Linux and macOS |

## The seams lecture notes and the Catcher example

The lecture notes on seams and the worked example are in their own repository, **[split-at-the-seam](https://github.com/kenpower/split-at-the-seam)**. Catcher (move a basket, catch falling fruit) is not one of these games. It shows the method without giving away an answer: the tangled version with a seam map in every line, one complete program per refactoring step, and the finished split with unit tests.

## Build and test (quick version)

SFML 3 and GoogleTest are downloaded on the first configure if they aren't installed. See [BUILDING.md](https://github.com/kenpower/16_SFML_Games/blob/main/BUILDING.md) for details, including Linux packages.

### Visual Studio (Windows)

Use the IDE, not the command line. You need Visual Studio 2022 with the **Desktop development with C++** workload, which includes CMake and the GoogleTest test adapter.

1. **Open:** *File → Open → Folder…* and pick this repository's folder. Visual Studio reads `CMakePresets.json` and configures the project. Wait for "CMake generation finished" in the Output window.
2. **Choose a configuration:** pick **Debug** in the configuration drop-down on the toolbar.
3. **Build:** *Build → Build All*.
4. **Run or debug a game:** choose **16_SFML_Games.exe** in the *Select Startup Item* drop-down next to the green ▶ button, then press **F5**. If the game's images don't load, the debugger is running it from the wrong folder (see [BUILDING.md](https://github.com/kenpower/16_SFML_Games/blob/main/BUILDING.md#visual-studio)).
5. **Test:** *Test → Test Explorer*, then **Run All Tests** (▶▶). Every test appears in a tree. Double-click a failing test to go to it. To debug a test, right-click it and choose **Debug**.

### Command line (Linux, macOS, or Windows without Visual Studio)

You need CMake 3.21+, a C++17 compiler and Git.

```sh
cmake --workflow --preset default    # configure, build and run the tests (Release, in build/)
cmake --workflow --preset debug      # the same for Debug, in build-debug/
ctest --test-dir build -C Release --output-on-failure    # run the tests again
```

In VS Code, use the CMake Tools extension: pick a preset, then use its Build, Debug and Test buttons in the status bar.

### Running the launcher

**Run the launcher from its own folder** (`build/bin`, or `build-debug/bin` for Debug). The games load images with relative paths. Type the capital letter in a game's name (e.g. `T` for Tetris) to start it, and close its window to return to the menu.

## Where things are

| Path | What's there |
|------|--------------|
| `16_SFML_Games/` | One `.cpp` file per game, the launcher (`16_SFML_Games.cpp`), and SFML-free classes that tests can use: `Grid.h` and `Player.h` (the Xonix worked example) and `Geometry.h` (`Point`, `Vec2f`, `Rect` for your own classes) |
| `16_SFML_Games/images/` | Game assets, copied next to the executable when you build |
| `Tests/` | Unit tests. `test.cpp` tests the Xonix classes, and `geometry_test.cpp` tests `Geometry.h`. There is one file for each of the four assignment games (`tetris_test.cpp`, `minesweeper_test.cpp`, `tron_test.cpp`, `netwalk_test.cpp`). Each one tests the placeholder `name()` of the game's empty class (`TetrisBoard.h`, `MineField.h`, `TronArena.h`, `PipeGrid.h`). The tests don't link SFML. |

### Reading the game code

The game files carry signpost comments to help you find your way around:

- `// ===== GUIDE` at the top: where to start reading and what state survives between frames
- `// ---- SETUP / INPUT / UPDATE / DRAW ----`: the big jobs in each game loop
- `// HINT:` things worth looking at closely, often with the debugger

### Chess

Chess plays against the [Stockfish](https://stockfishchess.org/) engine (press Space for the computer's move). On Windows it is bundled. On Linux and macOS, install `stockfish` with your package manager before configuring. See [BUILDING.md](https://github.com/kenpower/16_SFML_Games/blob/main/BUILDING.md#chess-and-stockfish).

### Continuous integration

Every push runs two GitHub checks: the unit tests on Linux, without SFML, and a full game build on Windows. You can see the result as a ✔ or ✘ next to each commit.
