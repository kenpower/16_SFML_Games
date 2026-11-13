# Lab: Test and refactor SFML game part 1

Get your starter code here: https://classroom.github.com/a/qObzKUBL

**Deadline:** Wednesday 25th Nov 23:59
**Recommended checkpoint (not graded):** Steps 0–3 finished by the end of week 1. Most people who run out of time started the class extraction in step 5 too late.

## What you will learn

By the end of this lab you should be able to:

1. Build, run and debug an unfamiliar C++ project with CMake.
2. Read unfamiliar code systematically and explain what it does.
3. Describe the current behaviour of code *before* changing it.
4. Refactor in small, safe, behaviour-preserving steps, with one refactoring per commit.
5. Recognise and name common code smells.
6. Separate game logic from its dependencies (graphics, input, time, randomness) so that it can be tested.

## Overview

| Step | What you do | What you produce |
|------|-------------|------------------|
| 0 | Build, run and debug | Screenshot of a breakpoint hit in your game |
| 1 | Choose a game | — |
| 2 | Read and map the code | `NOTES.md` |
| 3 | Write a manual test script | Section in `NOTES.md` |
| 4 | Refactor for readability | At least 5 `refactor:` commits |
| 5 | Extract a testable class | A `refactor:` commit adding a class with no SFML |
| 6 | Reflect | Section in `NOTES.md` |

---

## Step 0 – Build, run and debug

Follow the [README](https://github.com/kenpower/16_SFML_Games/blob/master/README.md) to configure and build the project with CMake. Then:

- Run the launcher **from its own folder** (`build/bin`, or `build/bin/Debug` with Visual Studio). The games load images using relative paths, so if you run it from anywhere else the textures won't load.
- Open the project in your IDE (Visual Studio: *File → Open → Folder*; VS Code: the CMake Tools extension). Build the **Debug** configuration.
- Set a breakpoint inside the game loop of any game, start it under the debugger, and confirm the breakpoint is hit. Make sure the debugger's working directory is the executable's folder.
- Build the `Tests` target and run the tests (`ctest --test-dir build -C Debug --output-on-failure`). The Xonix tests should pass.

Take a screenshot of the breakpoint being hit. If you can't get this working, ask for help **this week**. Every later step depends on it.

## Step 1 – Choose a game

The repository contains 16 classic arcade games. Play some of them and look at the source code of a few. Choose **one** game to work on.

I recommend one of **Tetris, Minesweeper, Arkanoid, Racing or Bejeweled**. You can choose another game, but it may be too simple or too complex for this assignment. You **cannot** choose Xonix: it is the worked example of refactoring and testing (see `Grid.h`, `Player.h` and `Tests/test.cpp`).

## Step 2 – Read and map the code

Understanding an unfamiliar codebase is hard, and you are not expected to understand most of the code the first time you read it. You will read it several times, and each time more of it will make sense.

### Read with a strategy

- **Top-down:** start at the game's entry function (e.g. `int tetris()`) and follow the main loop. What are the 3 or 4 "big" jobs the loop does every frame? (Hint: look for input handling, updating, collision or rules, and drawing.)
- **Follow the data:** list every variable that survives between frames (globals, and locals declared before the loop). For each one, write down what you think it represents and which parts of the loop read or change it.
- **Use the debugger:** step through one full frame. Put a breakpoint on an `if` and check whether the condition is true when you expected it to be. Use watches to see the data change.

### Add comments

Put comments on lines you think you understand. Focus on:

- **Loops:** what is each loop for, even if you don't yet know exactly how it works?
- **`if` statements:** what decision is being made, and why?

Good comments explain *why* or *what it means*, not what the syntax does. `// shift every row above the cleared line down by one` is useful. `// decrement k` is not.

### Deliverable: `NOTES.md`

Create `NOTES.md` in the root of your repository with:

1. The 3–4 big jobs of the game loop.
2. A table of the important state variables and what they mean.
3. A list of **questions you still have** about the code. It is fine (and normal) for this list to be long at first. Cross items off as you answer them.

## Step 3 – Write a manual test script

There are no automated tests for your game yet, so before you change anything, write down how you will check that its behaviour hasn't changed.

Add a **Manual test script** section to `NOTES.md` with 5–8 short, repeatable scenarios. Each one needs the steps and the expected result. For example:

> **Line clear:** Fill the bottom row by dropping pieces. *Expected:* the row disappears and everything above it moves down one row.

Choose scenarios that exercise the important rules, not just "the window opens". Run the whole script after every refactoring in step 4.

### Found something odd?

While reading and testing you may find behaviour that looks like a bug. **Do not fix it during a refactoring.** A refactoring must not change behaviour. Instead, record it in a **Known issues** section of `NOTES.md`. If you want to fix it, do that in a separate commit prefixed with `fix:`. Fix commits are not counted as refactorings.

## Step 4 – Refactor for readability

Refactor the code to make it more readable and maintainable.

### Rules

- **One refactoring per commit.** Prefix each message with `refactor:` followed by a description, e.g. `refactor: rename field to board`.
- **After every refactoring**, build, run your manual test script, and only then commit.
- Do **at least 5 different types** of refactoring.
- Keep each commit small enough to review in a minute or two. "Rename `a` to `currentPiece`" is one commit. "Rename everything, extract three functions and add a class" is too much for one commit.

### Suggested refactorings

| Refactoring | Smell it fixes | Example |
|-------------|----------------|---------|
| Rename variable or function | Unclear names | `a`, `b`, `k`, `t1`, `s` → names that say what they hold |
| Replace magic number with named constant | Magic numbers | `18` → `TILE_SIZE`, `9` → `MINE` |
| Introduce explanatory variable | Complex expression | `bool hitLeftWall = x < 0;` |
| Extract function | Long function | `handleInput()`, `update()`, `draw()`, `setup()` |
| Remove control flag | Flag variable controlling a loop | Use `break`, `continue` or `return` instead |
| Introduce struct or class for a data clump | Data clump | Variables that always appear together, e.g. `x, y, dx, dy` |
| Remove duplicate code | Duplicated code | Two nearly identical loops → one function with a parameter |
| Replace C array with `std::array` | Primitive obsession | `int field[M][N]` → `std::array<std::array<int, N>, M>` |
| Replace `int` codes with `enum class` | Primitive obsession | `0`, `9`, `10`, `11` → `Cell::Empty`, `Cell::Mine`, … |
| Other sensible refactorings | — | It still counts even if you don't know the refactoring's name |

## Step 5 – Extract a testable class

This is the most important step. **Start it early.**

Extract at least one **non-trivial** piece of game logic into its own class, in its own header file (e.g. `16_SFML_Games/TetrisBoard.h`). This class will be unit tested in part 2.

### Definition of done

Your class must:

- [ ] Be in its own file (header-only is fine, like `Grid.h` in the Xonix example).
- [ ] **Not include SFML**, directly or through another header. Separating logic from its dependencies is an important programming skill. If your class includes SFML, testing it becomes much harder.
- [ ] **Not use global variables.** Everything it needs is passed to its constructor or methods, or stored as a member.
- [ ] Not call `rand()` or read the clock directly. If it needs randomness or time, take it as a parameter (e.g. `spawnPiece(int shapeIndex)` or `update(float dt)`). This makes the class predictable and therefore testable.
- [ ] Be used by the game, which still plays exactly as before.
- [ ] Compile in the test project: add `#include "YourClass.h"` and one trivial test to `Tests/test.cpp`, and check the `Tests` target builds and passes.

### Where to look

| Game | Logic that doesn't need SFML |
|------|------------------------------|
| Tetris | The board, collision checking, rotation, line clearing |
| Minesweeper | Placing mines, counting neighbours, revealing cells |
| Arkanoid | Ball movement and bouncing, brick collisions (use your own simple rectangle struct, not `sf::FloatRect`) |
| Racing | Car movement, AI steering towards checkpoints, car-to-car collision |
| Bejeweled | The gem grid, finding matches, swapping, gravity and refill |

Drawing and input stay in the game file, because they are the parts that need SFML. The game file reads from your class to draw, and calls your class's methods when input happens.

## Step 6 – Reflect

Add a short **Reflection** section (half a page at most) to `NOTES.md`:

- Which refactoring was hardest, and why?
- Did anything break during a refactoring? How did you notice?
- Which parts of the original code made extracting a testable class difficult (globals, SFML types, `rand()`, …)?
- Which questions from your step 2 list are still unanswered?

---

## Rules

- Only change your chosen game's `.cpp` file. You may **add** new files for extracted classes, and edit `Tests/test.cpp` and `CMakeLists.txt` as needed to compile your class in the test project. Don't delete any other files.
- Use AI tools to *explain* code if they help, but every change you commit must be one you can explain in your own words. You may be asked to walk through any commit.

## Submission

Submission is a slightly complex process, so read carefully and compare your submission to the screenshot at the end.

At the end of this process you should have several commits, each containing exactly one refactoring. Copy a link to each commit you want to submit and paste the links into the "Create Submission" section for this assignment. Each pasted link should automatically display as shown below (the commit message is highlighted). Check that your submission looks like mine; the only differences should be the commit message, repository name and commit ID.

Also make sure `NOTES.md` and your step 0 screenshot are committed to your repository.

### Checklist

- [ ] Step 0 screenshot committed
- [ ] `NOTES.md` has the game loop summary, state table, questions, manual test script, known issues and reflection
- [ ] At least 5 `refactor:` commits, each a different type of refactoring
- [ ] One commit extracting a class that meets the definition of done
- [ ] The game still builds and plays exactly as before
- [ ] The `Tests` target builds and passes
