# 16 SFML Games

Sixteen small classic games written in C++17 with [SFML 3](https://www.sfml-dev.org/), behind one text-menu launcher: Tetris, Doodle Jump, Arkanoid, Snake, Minesweeper, Fifteen Puzzle, Racing, Outrun, Xonix, Bejeweled, Netwalk, Mahjong Solitaire, Tron, Chess and Asteroids. (Volleyball is a placeholder that needs Box2D and is not built.)

The games work, but most of them are written as one long loop that mixes the game's rules with graphics, keyboard, time and random numbers. That makes them good material for learning to **read, refactor and test unfamiliar code**, which is what this repository is used for.

## Start here

| Document | What it is |
|----------|------------|
| [assignment.md](assignment.md) | **The project brief**: a three-week solo project to refactor, test and extend one game. Overview, marking, rules and tags. |
| [assignment_week1.md](assignment_week1.md) | Week 1: understand your game and refactor it towards a seam |
| [assignment_week2.md](assignment_week2.md) | Week 2 *(draft)*: split the game at the seam and test it |
| [assignment_week3.md](assignment_week3.md) | Week 3 *(draft)*: add a feature, test first |
| [seams_lecture_notes.md](seams_lecture_notes.md) | Lecture notes: what a seam is, why you split an app there, how to find it, and why `rand()`, the clock and the keyboard stay outside |
| [howto_split.md](howto_split.md) | Step-by-step worked example: splitting **Catcher**, a small game made for this purpose, into an SFML-free class and testing it |
| [NOTES.md](NOTES.md) | The template students fill in during the project, with a section for every note the briefs ask for |
| [GIT_CHEATSHEET.md](GIT_CHEATSHEET.md) | One-page git card: commit, push, tags, undo, in Visual Studio and on the command line |
| [BUILDING.md](BUILDING.md) | Full setup, build, run and test instructions for Windows, Linux and macOS |
| [16_SFML_Games/REFACTORING_GUIDE.md](16_SFML_Games/REFACTORING_GUIDE.md) | The games by difficulty, and common refactorings with examples |

## The Catcher example

Catcher (move a basket, catch falling fruit) is not one of the 16 games. It exists to show the method without giving away an answer:

| Folder | What's in it |
|--------|--------------|
| [howto_split/before/](howto_split/before/) | Everything tangled in one loop. Every line has a **seam map** comment (`// [RULE+RAND+DRAW]`) showing what it depends on. |
| [howto_split/steps/](howto_split/steps/) | One complete program per refactoring step, so you can compare consecutive steps |
| [howto_split/after/](howto_split/after/) | The finished split: `CatcherGame.h` (rules, no SFML), a thin `catcher.cpp`, and unit tests |

Build targets: `catcher_before`, `catcher_step1` … `catcher_step6`, `catcher_after`, and `CatcherTests`.

## Build and test (quick version)

SFML 3 and GoogleTest are downloaded on the first configure if they aren't installed. See [BUILDING.md](BUILDING.md) for details, including Linux packages.

### Visual Studio (Windows)

Use the IDE, not the command line. You need Visual Studio 2022 with the **Desktop development with C++** workload, which includes CMake and the GoogleTest test adapter.

1. **Open:** *File → Open → Folder…* and pick this repository's folder. Visual Studio reads `CMakePresets.json` and configures the project. Wait for "CMake generation finished" in the Output window.
2. **Choose a configuration:** pick **Debug** in the configuration drop-down on the toolbar.
3. **Build:** *Build → Build All*.
4. **Run or debug a game:** choose **16_SFML_Games.exe** (or `catcher_before.exe`, …) in the *Select Startup Item* drop-down next to the green ▶ button, then press **F5**. If the game's images don't load, the debugger is running it from the wrong folder (see [BUILDING.md](BUILDING.md#visual-studio)).
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
| `16_SFML_Games/` | One `.cpp` file per game, the launcher (`16_SFML_Games.cpp`), and SFML-free classes that tests can use: `Grid.h` and `Player.h` (the Xonix worked example) and `PlainTypes.h` (`Point`, `Vec2f`, `Rect` for your own classes) |
| `16_SFML_Games/images/` | Game assets, copied next to the executable when you build |
| `Tests/` | Unit tests. `test.cpp` tests the Xonix classes. There is one file for each of the four assignment games (`tetris_test.cpp`, `minesweeper_test.cpp`, `tron_test.cpp`, `netwalk_test.cpp`). Each one tests the placeholder `name()` of the game's empty class (`TetrisBoard.h`, `MineField.h`, `TronArena.h`, `PipeGrid.h`). The tests don't link SFML. |
| `howto_split/` | The Catcher example |

### Reading the game code

The game files carry signpost comments to help you find your way around:

- `// ===== GUIDE` at the top: where to start reading and what state survives between frames
- `// ---- SETUP / INPUT / UPDATE / DRAW ----`: the big jobs in each game loop
- `// HINT:` things worth looking at closely, often with the debugger

### Chess

Chess plays against the [Stockfish](https://stockfishchess.org/) engine (press Space for the computer's move). On Windows it is bundled. On Linux and macOS, install `stockfish` with your package manager before configuring. See [BUILDING.md](BUILDING.md#chess-and-stockfish).

### Continuous integration

Every push runs two GitHub checks: the unit tests on Linux, without SFML, and a full game build on Windows. You can see the result as a ✔ or ✘ next to each commit.
