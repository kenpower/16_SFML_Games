# Lab: Test and refactor SFML game part 1

Get your starter code here: https://classroom.github.com/a/qObzKUBL

You work **in pairs**, in **two lab sessions**. All the work happens in the labs. Don't work on it between sessions.

**Deadline:** the end of your second lab session. Work pushed after that is not marked.

## What you will learn

By the end of this lab you should be able to:

1. Build, run and debug an unfamiliar C++ project with CMake.
2. Read unfamiliar code systematically and explain what it does.
3. Describe the current behaviour of code *before* changing it.
4. Refactor in small, safe, behaviour-preserving steps, with one refactoring per commit.
5. Recognise and name common code smells.
6. Separate game logic from its dependencies (graphics, input, time, randomness) so that it can be tested.

## Overview

| Session | Step | What you do | What you produce |
|---------|------|-------------|------------------|
| 1 | 0 | Build, run and debug | A TA sees your breakpoint hit |
| 1 | 1 | Choose a game | — |
| 1 | 2 | Read and map the code | `NOTES.md`, and `docs:` commits with comments in your game's code |
| 1 | 3 | Write a manual test script | **Manual test script** and **Known issues** sections in `NOTES.md` |
| 1 → 2 | 4 | Refactor for readability | At least 5 `refactor(...)` commits. Start in session 1. |
| 1 | — | End of session 1 | Push, and push the `session1` tag |
| 2 | 5 | Extract a testable class | A class with no SFML in its own header, and one trivial test in `Tests/test.cpp` |
| 2 | 6 | Reflect | One `REFLECTION-<yourname>.md` each, written on your own |
| 2 | — | Viva | 3 minutes each with a TA, during session 2 |

Rough timing: session 1 is steps 0–3 and your first one or two refactorings. Session 2 is the rest of step 4, then step 5, then step 6. If you are still on step 4 halfway through session 2, move on to step 5. Step 5 is worth more.

---

## Working as a pair

You share one repository (GitHub Classroom makes it for your team) and work at one machine.

- **Driver and navigator.** The driver types. The navigator reads, checks, and asks "why is that safe?". **Swap after every commit.** During steps 1–3, which have few commits, swap every 15 minutes.
- **The driver is the author of the commit.** Before committing, the driver sets themselves as the author and names the navigator as co-author:

  ```sh
  git commit --author="Ann Smith <ann@example.com>" -m "refactor(rename): rename field to board

  Co-authored-by: Bob Jones <bob@example.com>"
  ```

  Use the email address on your GitHub account.
- **No more than two commits in a row by the same author.** Your commit history shows who drove. It is checked when marking.
- **Both of you must understand every commit.** In the viva you can be asked about any commit, including ones your partner drove.

### If your partner is missing

- **Tell a TA at the start of the session**, so the absence is recorded.
- **Don't wait for them.** Carry on alone in your shared repository.
- If you are alone for **session 2**, you have a **reduced spec**: at least **3** refactorings (of 3 types) instead of 5, and a step 5 class with **2** public methods that contain rules instead of 3–4. You are marked against the reduced spec, so you are not penalised for doing the work of two.
- If you missed **session 1**, join your partner's repository at the start of session 2. Read the session 1 commits and `NOTES.md` before you start driving.

---

## Step 0 – Build, run and debug

Follow the [README](https://github.com/kenpower/16_SFML_Games/blob/main/README.md) to configure and build the project with CMake, on the machine you will pair on. Then:

- Run the launcher **from its own folder** (`build/bin`, or `build/bin/Debug` with Visual Studio). The games load images using relative paths, so if you run it from anywhere else the textures won't load.
- Open the project in your IDE (Visual Studio: *File → Open → Folder*; VS Code: the CMake Tools extension). Build the **Debug** configuration.
- Set a breakpoint inside the game loop of any game, start it under the debugger, and confirm the breakpoint is hit. Make sure the debugger's working directory is the executable's folder.
- Build the `Tests` target and run the tests (`ctest --test-dir build -C Debug --output-on-failure`). The Xonix tests should pass.

**Show a TA your breakpoint being hit.** If you can't get this working within the first 15 minutes, ask a TA straight away. Every later step depends on it.

## Step 1 – Choose a game

The repository contains 16 classic arcade games. Play some of them and look at the source code of a few. Choose **one** game together.

I recommend one of **Tetris, Minesweeper, Arkanoid, Racing, Tron or Netwalk**. You can choose another game, but it may be too complex for this assignment.

You **cannot** choose:

- **Xonix**: it is the worked example of refactoring and testing (see `Grid.h`, `Player.h` and `Tests/test.cpp`).
- **Fifteen Puzzle, Snake or Doodle Jump**: they are too simple. Their logic is already almost separate from SFML, so there is not enough to extract.

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

Commit your comments with a message starting `docs:`, e.g. `docs: comment the line-clearing loop`. Comment commits are not refactorings, so they don't count towards step 4.

### Deliverable: `NOTES.md`

Create `NOTES.md` in the root of your repository with:

1. The 3–4 big jobs of the game loop.
2. A table of the important state variables and what they mean.
3. A list of **questions you still have** about the code. It is fine (and normal) for this list to be long at first. Cross items off as you answer them.

## Step 3 – Write a manual test script

There are no automated tests for your game yet, so before you change anything, write down how you will check that its behaviour hasn't changed.

Add a **Manual test script** section to `NOTES.md` with 4–5 short, repeatable scenarios. Each one needs the steps and the expected result. For example:

> **Line clear:** Fill the bottom row by dropping pieces. *Expected:* the row disappears and everything above it moves down one row.

Choose scenarios that exercise the important rules, not just "the window opens". Run the whole script after every refactoring in step 4.

### Found something odd?

While reading and testing you may find behaviour that looks like a bug. **Do not fix it during a refactoring.** A refactoring must not change behaviour. Instead, record it in a **Known issues** section of `NOTES.md`, including how you found it (which scenario, or what you saw in the debugger). If you want to fix it, do that in a separate commit prefixed with `fix:`. Fix commits are not counted as refactorings.

## Step 4 – Refactor for readability

Refactor the code to make it more readable and maintainable. Start in session 1, as soon as step 3 is done.

**Aim your refactorings at step 5.** Use the steps from the lecture on splitting logic from SFML: taking state out of SFML objects, naming magic numbers, turning input into plain data, and passing in time and randomness. Each one makes the code easier to read *and* makes step 5 smaller.

### Rules

- **One refactoring per commit.** Start each message with `refactor(<type>):`, using a type from the table below, e.g. `refactor(rename): rename field to board`.
- **After every refactoring**, build, run your manual test script, and only then commit.
- Do **at least 5 refactorings, of at least 4 different types**.
- Keep each commit small enough to review in a minute or two. "Rename `a` to `currentPiece`" is one commit. "Rename everything, extract three functions and add a class" is too much for one commit.

### Suggested refactorings

| Type (for the commit message) | Refactoring | Smell it fixes | Example |
|-------------------------------|-------------|----------------|---------|
| `rename` | Rename variable or function | Unclear names | `a`, `b`, `k`, `t1`, `s` → names that say what they hold |
| `constant` | Replace magic number with named constant | Magic numbers | `18` → `TILE_SIZE`, `9` → `MINE` |
| `explain-var` | Introduce explanatory variable | Complex expression | `bool hitLeftWall = x < 0;` |
| `extract-function` | Extract function | Long function | `handleInput()`, `update()`, `draw()`, `setup()` |
| `remove-flag` | Remove control flag | Flag variable controlling a loop | Use `break`, `continue` or `return` instead |
| `struct` | Introduce struct or class for a data clump | Data clump | Variables that always appear together, e.g. `x, y, dx, dy` |
| `dedupe` | Remove duplicate code | Duplicated code | Two nearly identical loops → one function with a parameter |
| `std-array` | Replace C array with `std::array` | Primitive obsession | `int field[M][N]` → `std::array<std::array<int, N>, M>` |
| `enum` | Replace `int` codes with `enum class` | Primitive obsession | `0`, `9`, `10`, `11` → `Cell::Empty`, `Cell::Mine`, … |
| `state-out-of-sfml` | Keep game state in your own variables, not in SFML objects | Hidden state | A position read back with `sprite.getPosition()` → your own `x`, `y` |
| `input-data` | Turn input into plain data | Input mixed with rules | `Keyboard::isKeyPressed` inside the rules → `bool left` read once per frame |
| `inject` | Pass in time or randomness | Hidden dependency | `rand()` or `clock` inside the rules → a parameter |
| `other` | Other sensible refactorings | — | It still counts even if you don't know the refactoring's name |

A struct or class you introduce here (for example for a data clump) does not count as your step 5 class unless it meets the step 5 definition of done.

## Step 5 – Extract a testable class

This is the most important step and it is worth the most marks. **Start it by the middle of session 2 at the latest.**

Extract a piece of game logic into its own class, in its own header file (e.g. `16_SFML_Games/TetrisBoard.h`). This class will be unit tested in part 2.

**Keep it small.** One class with 3–4 public methods that contain real rules is enough. For Tetris that could be the board and line clearing, not the whole game. It must be **non-trivial**: it contains at least one rule that changes state based on a condition (a collision, a line clear, a reveal, a crash).

Several small commits are fine, and better than one big one. Start each message with `refactor(extract-class):`.

### Definition of done

Your class must:

- [ ] Be in its own file (header-only is fine, like `Grid.h` in the Xonix example).
- [ ] **Not include SFML**, directly or through another header. Separating logic from its dependencies is an important programming skill. If your class includes SFML, testing it becomes much harder.
- [ ] **Not use global variables.** Everything it needs is passed to its constructor or methods, or stored as a member.
- [ ] Not call `rand()` or read the clock directly. If it needs randomness or time, take it as a parameter (e.g. `spawnPiece(int shapeIndex)` or `update(float dt)`). This makes the class predictable and therefore testable.
- [ ] Be used by the game, which still plays exactly as before.
- [ ] Compile in the test project: add `#include "YourClass.h"` and one trivial test to `Tests/test.cpp`, and check the `Tests` target builds and passes.

You can check the first four yourself. Search your header for `SFML`, `rand`, `time` and `Clock`. None of them should appear.

Then add a **Tests I would write** section to `NOTES.md`: at least three tests for your class, one line each (what you set up, what you do, what you expect). You will write them in part 2.

### Where to look

| Game | Logic that doesn't need SFML |
|------|------------------------------|
| Tetris | The board, collision checking, rotation, line clearing |
| Minesweeper | Placing mines, counting neighbours, revealing cells |
| Arkanoid | Ball movement and bouncing, brick collisions (use your own simple rectangle struct, not `sf::FloatRect`) |
| Racing | Car movement, AI steering towards checkpoints, car-to-car collision |
| Tron | The bikes, the trail map, steering, crash detection |
| Netwalk | The pipe grid, rotating pipes, finding which cells connect to the server, puzzle generation |

Drawing and input stay in the game file, because they are the parts that need SFML. The game file reads from your class to draw, and calls your class's methods when input happens.

## Step 6 – Reflect (on your own)

At the end of session 2, each of you writes your **own** `REFLECTION-<yourname>.md` (half a page at most). Don't write it together.

- Which refactoring was hardest, and why?
- Did anything break during a refactoring? Give the commit, and say how you noticed.
- Which parts of the original code made extracting a testable class difficult (globals, SFML types, `rand()`, …)?
- One thing your partner explained to you, or you explained to them.
- Which questions from your step 2 list are still unanswered?

Before you leave, you will also each fill in a short **private form** saying roughly how the work was split between you. Your partner does not see it.

## Viva

During session 2, a TA will spend about 3 minutes with each of you **separately**. They will pick one or two of your commits, possibly ones your partner drove, and ask questions like:

- Why does this commit not change the game's behaviour?
- Where did `rand()` (or the clock, or the keyboard) go, and why isn't it in your class?
- Show me, in the debugger, where the game calls your class.

You can't prepare a script for this. Understanding every commit is the preparation.

---

## Marking

| Part | Marks | Who gets them |
|------|------:|---------------|
| Session 1 work (up to the `session1` tag): breakpoint check-off, `NOTES.md` map, manual test script, known issues, comments | 25 | Each partner who was in session 1 |
| Session 2 work: refactorings, extracted class, **Tests I would write**, `Tests` builds and passes | 35 | Each partner who was in session 2 |
| Viva | 25 | Each of you separately |
| Your own reflection | 15 | Each of you separately |

An unexcused absence from a session means no marks for that session's work. The private form is used only if it agrees with the commit history or the viva.

## Rules

- Only change your chosen game's `.cpp` file. You may **add** new files for extracted classes, and edit `Tests/test.cpp` to compile your class in the test project. Don't delete any other files.
- You don't need to edit `CMakeLists.txt`. A header-only class (like `Grid.h`) in the `16_SFML_Games/` folder is already found by both the game and the `Tests` project. If you add a `.cpp` file for your class, you will need to add it to `CMakeLists.txt`.
- Use AI tools to *explain* code if they help, but don't use them to write your changes. Every commit must be one both of you can explain in your own words. The viva checks this.

## Submission

There is nothing to upload. Your repository is marked as it is at the end of session 2.

**At the end of session 1**, push your work and tag it:

```sh
git push
git tag session1
git push origin session1
```

**At the end of session 2**, push again.

### Checklist

- [ ] Session 1: TA has seen your breakpoint hit
- [ ] Session 1: `session1` tag pushed
- [ ] `NOTES.md` has the game loop summary, state table, questions, manual test script, known issues and tests I would write
- [ ] At least 5 `refactor(<type>):` commits, of at least 4 different types (3 of 3 types if you were alone in session 2)
- [ ] Commits alternate between partners, never more than two in a row by the same author
- [ ] A class that meets the definition of done
- [ ] The game still builds and plays exactly as before
- [ ] The `Tests` target builds and passes
- [ ] Each of you has pushed your own `REFLECTION-<yourname>.md`
- [ ] Each of you has done your viva and filled in the private form
