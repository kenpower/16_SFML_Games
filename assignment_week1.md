# Week 1 – Understand the game and refactor it towards a seam

Part of the three-week project **Refactor, test and extend an SFML game**. See [assignment.md](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md) for the overview, marking and rules.

> **Goal:** by the end of this week, *I understand my game, and I have refactored it towards a seam.*
>
> **Done when:**
> - Ken or Ben has seen your breakpoint hit (in the lab)
> - `NOTES.md` maps the game, and has a manual test script and a test list
> - you have made at least **6** refactorings, of at least **4** types, including at least **2** seam types
> - your game's rules are gathered into an `update()` function and a state `struct`, still in the game file
> - the `week1` tag is pushed **before session 2**

Read `seams_lecture_notes.md` before the lab, or at least sections 1–3.

In the lab you may use [cs50.ai/chat](https://cs50.ai/chat)[^duck] for technical questions, such as build errors or debugger problems. See the AI rule in [assignment.md](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md#rules).

## In the lab (100 minutes)

| Time | Step |
|------|------|
| 0–20 | 0 Build, run and debug. Get the check-off. |
| 20–30 | 1 Choose a game |
| 30–55 | 2 Read and map the code (start `NOTES.md`) |
| 55–70 | 3 Manual test script |
| 70–95 | 4 First refactorings: get at least 2 done and pushed |
| 95–100 | Push, and push the `session1` tag[^tag] (see [Tags](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md#tags)) |

## At home

- Finish `NOTES.md`: state table, questions, test list.
- Add the seam map to your game's code (step 5a), then do the seam refactorings (step 5b) and any more general ones.
- Finish by gathering the rules into `update()` and a state `struct` (step 5c). That makes **6+ refactorings** in total.
- Read `howto_split.md` and play `catcher_before` and `catcher_after`.
- Push the `week1` tag.

---

## Step 0 – Build, run and debug

Follow the [README](https://github.com/kenpower/16_SFML_Games/blob/main/README.md#build-and-test-quick-version) to build the project. **If you use Visual Studio, build, run and test in the IDE**, not on the command line. (Tags need the terminal: see [GIT_CHEATSHEET.md](https://github.com/kenpower/16_SFML_Games/blob/main/GIT_CHEATSHEET.md).) If you get stuck, [BUILDING.md](https://github.com/kenpower/16_SFML_Games/blob/main/BUILDING.md) has the full details.

1. **Open** the repository folder: in Visual Studio use *File → Open → Folder…*; in VS Code use the CMake Tools extension. Choose the **Debug** configuration.
2. **Build** everything (*Build → Build All* in Visual Studio).
3. **Run the launcher** under the debugger: choose `16_SFML_Games.exe` as the startup item and press **F5**. Play a game for a moment. If the images don't load, the debugger is running the game from the wrong folder: see [BUILDING.md](https://github.com/kenpower/16_SFML_Games/blob/main/BUILDING.md#visual-studio).
4. **Set a breakpoint** inside the game loop of any game, start it under the debugger, and check that the breakpoint is hit.
5. **Run the tests:** *Test → Test Explorer → Run All Tests* in Visual Studio (on the command line, `ctest --test-dir build-debug --output-on-failure`). The Xonix tests and the placeholder test for each game (`Tests/tetris_test.cpp` and so on) should all pass.

**Show Ken or Ben your breakpoint being hit.** If you can't get this working within the first 15 minutes, ask straight away. Every later step depends on it.

## Step 1 – Choose a game

Choose **one** of these four games:

| Game | Your class (already provided, empty) | Test file |
|------|--------------------------------------|-----------|
| **Tetris** | `16_SFML_Games/TetrisBoard.h` | `Tests/tetris_test.cpp` |
| **Minesweeper** | `16_SFML_Games/MineField.h` | `Tests/minesweeper_test.cpp` |
| **Tron** | `16_SFML_Games/TronArena.h` | `Tests/tron_test.cpp` |
| **Netwalk** | `16_SFML_Games/PipeGrid.h` | `Tests/netwalk_test.cpp` |

All four keep their game state in whole numbers (grids, cells, directions), so their tests compare `int`s and `bool`s.

The other games aren't part of this project. Xonix is the worked example (see `Grid.h`, `Player.h` and `Tests/test.cpp`). Some games are too simple to need splitting, and others are too complex for three weeks or need floating-point tests.

Read the **Refactorings to look for** list at the top of your game's `.cpp` file. It tells you where to start.

Write your choice at the top of `NOTES.md`.

## Step 2 – Read and map the code

Understanding an unfamiliar codebase is hard, and you are not expected to understand most of the code the first time you read it. You will read it several times, and each time more of it will make sense.

### Read with a strategy

- **Top-down:** start at the game's entry function (e.g. `int tetris()`) and follow the main loop. What are the 3 or 4 "big" jobs the loop does every frame? The `// ----` signpost comments in the code will help.
- **Follow the data:** list every variable that survives between frames (globals, and locals declared before the loop). For each one, write down what you think it represents and which parts of the loop read or change it.
- **Use the debugger:** step through one full frame. Put a breakpoint on an `if` and check whether the condition is true when you expected it to be. Use watches to see the data change.

### Deliverable: `NOTES.md`

Fill in `NOTES.md`, in the root of your repository. A template is provided, with a heading and a short prompt for every section you need over the three weeks. This week, fill in:

1. The 3–4 big jobs of the game loop.
2. A table of the important state variables and what they mean.
3. A list of **questions you still have** about the code. It is fine (and normal) for this list to be long at first. Cross items off as you answer them.
4. An **AI use** section (it may stay empty).

You may add comments to the code as you go. Commit them with a message starting `docs:`. Comment commits are not refactorings.

## Step 3 – Manual test script and test list

There are no automated tests for your game yet, so before you change anything, write down what "it still works" means.

### Manual test script (in the lab)

Add a **Manual test script** section to `NOTES.md` with **4–5** short, repeatable scenarios. Each one needs the steps and the expected result. For example:

> **Line clear:** Fill the bottom row by dropping pieces. *Expected:* the row disappears and everything above it moves down one row.

Choose scenarios that exercise the important rules, not just "the window opens". You will run the whole script after **every** refactoring, so keep each one short: under a minute to play.

### Test list (at home)

Add a **Test list** section to `NOTES.md`: the game's rules written as plain English sentences, one rule per line. Write **at least 5**. For example:

```
- A new game starts with an empty board.
- A piece can't move through the left wall.
- A full row is cleared and the rows above move down.
```

They become your unit tests in week 2. Write down what the game **does**, found by playing it, not what you think it should do.

### Found something odd?

While reading and testing you may find behaviour that looks like a bug. **Do not fix it during a refactoring.** A refactoring must not change behaviour. Record it in a **Known issues** section of `NOTES.md`, including how you found it. If you want to fix it, do that later in a separate `fix:` commit, ideally with a test that fails first. Fix commits are not counted as refactorings.

## Step 4 – Refactoring rules

### What `refactor(<type>):` means

It's the start of a **commit message**. Every commit you make in this project begins with a short label saying what *kind* of change it is:

| Message starts with | Means |
|---------------------|-------|
| `refactor(<type>):` | a refactoring: the code is restructured, but the game behaves **exactly** the same. `<type>` names which refactoring, from the table below. |
| `docs:` | only comments or `NOTES.md` changed |
| `test:` | only tests were added or changed |
| `fix:` | a bug was fixed, so behaviour changed on purpose |
| `feat:` | a new feature (week 3) |

The rest of the message says, in plain English, what you did. For example:

```
refactor(rename): rename a and b to piece and previousPiece
refactor(constant): replace 18 with TILE_SIZE
refactor(inject): pass the next piece's shape into the spawn code instead of calling rand()
docs: add seam map
```

The word in brackets is the refactoring **type**. It's not a git feature, just a convention (it comes from a widely used style called *Conventional Commits*). It does three jobs:

- It makes you decide what a commit is *for* before you make it. If you can't pick one type, the commit is probably doing two things: split it.
- It lets a reader (and the marker) see your refactorings at a glance with `git log --oneline`.
- It's how your refactorings are counted: the "6 refactorings of 4 types" requirement is counted from these labels.

So `refactor(state-out-of-sfml):` means "a refactoring of type state-out-of-sfml": game state was moved out of an SFML object into a plain variable, and the game still plays the same.

### Rules

These rules apply to every refactoring in this project:

- **One refactoring per commit.** Start each message with `refactor(<type>):`, using a type from the table below, e.g. `refactor(rename): rename field to board`.
- **After every refactoring**, build, run your manual test script, commit, and **push**. [GIT_CHEATSHEET.md](https://github.com/kenpower/16_SFML_Games/blob/main/GIT_CHEATSHEET.md) shows how, in Visual Studio and on the command line.
- Keep each commit small enough to review in a minute or two. "Rename `a` to `currentPiece`" is one commit. "Rename everything and extract three functions" is too much.
- If a step breaks the game and you can't see why within a few minutes, undo it (`git restore .`) and take a smaller step.

**This week:** at least **6** refactorings, of at least **4** different types, including at least **2** seam types (in **bold** below).

Start with the easy general ones in the lab (`rename`, `constant`, `explain-var`), then move on to the seam ones at home.

| Type (for the commit message) | Refactoring | Smell it fixes | Example |
|-------------------------------|-------------|----------------|---------|
| `rename` | Rename variable or function | Unclear names | `a`, `b`, `k`, `t1`, `s` → names that say what they hold |
| `constant` | Replace magic number with named constant | Magic numbers | `18` → `TILE_SIZE`, `9` → `MINE` |
| `explain-var` | Introduce explanatory variable | Complex expression | `bool hitLeftWall = x < 0;` |
| `extract-function` | Extract function | Long function | `handleInput()`, `update()`, `draw()`, `setup()` |
| `remove-flag` | Remove control flag | Flag variable controlling a loop | Use `break`, `continue` or `return` instead |
| `struct` | Introduce struct for a data clump | Data clump | Variables that always appear together, e.g. `x, y, dx, dy` |
| `dedupe` | Remove duplicate code | Duplicated code | Two nearly identical loops → one function with a parameter |
| `std-array` | Replace C array with `std::array` | Primitive obsession | `int field[M][N]` → `std::array<std::array<int, N>, M>` |
| `enum` | Replace `int` codes with `enum class` | Primitive obsession | `0`, `9`, `10`, `11` → `Cell::Empty`, `Cell::Mine`, … |
| **`state-out-of-sfml`** | Keep game state in your own variables, not in SFML objects | Hidden state | A position read back with `sprite.getPosition()` → your own `x`, `y` |
| **`input-data`** | Turn input into plain data | Input mixed with rules | `Keyboard::isKeyPressed` inside the rules → `bool left` read once per frame |
| **`inject`** | Pass in time or randomness | Hidden dependency | `rand()` inside the rules → a value chosen outside and passed in |
| **`sfml-type`** | Replace an SFML type in the rules with your own | SFML in the rules | `sf::FloatRect` → `plain::Rect` from `PlainTypes.h`; `sf::Vector2i` → `plain::Point` |
| **`de-global`** | Make a function take what it needs, instead of reading a global | Hidden dependency | `bool check()` reads the global `field` → `bool check(const Field& field, ...)` |
| `other` | Other sensible refactorings | — | It still counts even if you don't know the refactoring's name |

## Step 5 – Refactor towards the seam (at home)

These refactorings get your game ready for week 2, when you move its rules into a class with no SFML. Every seam refactoring you do now makes week 2's in-lab task quicker.

### 5a. Add a seam map to the code

Tag the important lines of your game (setup, the loop, and any helper functions it calls) with an end-of-line comment saying what each one depends on:

| Tag | Means | Where it goes after the split |
|-------|-------|-------------------------------|
| `[RULE]` | game logic: moving, scoring, collisions | into your class |
| `[KEY]` | keyboard or mouse | stays in the game file, passed in as plain data |
| `[TIME]` | `Clock`, `dt`, timers | `dt` is passed in as a `float` |
| `[RAND]` | `rand()` | stays in the game file, the *result* is passed in |
| `[DRAW]` | sprites, `window.draw` | stays in the game file |
| `[GLOBAL]` | reads or writes a global variable | becomes a class member or a parameter |

For example, from [howto_split/before/catcher.cpp](howto_split/before/catcher.cpp), which has a complete seam map:

```cpp
if (Keyboard::isKeyPressed(Keyboard::Key::Left))  basket.move({-300 * dt, 0});   // [KEY+RULE+TIME+DRAW]
if (basket.getPosition().x < 0)   basket.setPosition({0, 470});                  // [RULE+DRAW]
fruit.setPosition({float(rand() % 380), 0});                                     // [RULE+RAND+DRAW]
int score = 0, lives = 3;                                                        // [RULE]
```

A tag with **more than one label** is a **tangle**. Each tangle is a seam refactoring waiting to happen.

- Commit the seam map on its own, as `docs: add seam map`.
- **Keep the tags up to date as you refactor.** When you untangle a line, its tag gets shorter (`[RULE+DRAW]` becomes `[RULE]` on one line and `[DRAW]` on another). Watching the tangles disappear is how you measure progress.
- In `NOTES.md`, add a short **Seam map** section: how many tangles you started with, and which ones look hardest.

When the rules move into your class in week 2, delete the tags from the class. Keep them in the game file if you find them useful.

### 5b. Untangle them

Work through `howto_split.md` **steps 1–5**, on your game, one commit each:

1. Take state out of SFML objects → `refactor(state-out-of-sfml):`
2. Name the magic numbers → `refactor(constant):`
3. Turn input into plain data → `refactor(input-data):`
4. Pass in time and randomness → `refactor(inject):`
5. Replace SFML types in the rules → `refactor(sfml-type):`
6. Stop helper functions reading globals → `refactor(de-global):`

Not every game needs all six. Do the ones your seam map shows you need. The step-by-step versions of Catcher in [howto_split/steps/](howto_split/steps/) show each of steps 1–5 as a small change you can compare.

### 5c. Gather the rules into `update()`

Once the rules are untangled, gather them in one place, **still in your game file** (`howto_split.md` step 6, and [steps/step6_update.cpp](howto_split/steps/step6_update.cpp) for Catcher):

- Put the game's state (the variables that live between frames) into one `struct`.
- Move the RULE lines into one function, `update(...)`, that takes the struct and the plain inputs (keys, `dt`, random values). If one function is too big, use a few: e.g. `movePiece`, `dropPiece`, `clearRows`.
- The loop should now read: **input → update → draw**.

Play the game and run your manual test script: it must still work. Commit as `refactor(extract-function): gather the rules into update()`. This counts as one of your 6 refactorings.

This is the hardest thinking in the whole split ("which lines are rules?"), so it's best done at home, without a clock running. In session 2 you move this struct and function into your class.

---

## End of week 1

- At the **end of the lab**, push and tag `session1`.
- **Before session 2**, push your home work and tag `week1`.

```sh
git push
git tag week1
git push origin week1
```

### Marking for week 1

| Part | Marks | What a top mark looks like |
|------|------:|----------------------------|
| Understanding | 10 | Breakpoint check-off. Clear state table. 4–5 scenarios that exercise real rules. A test list written from playing the game. A seam map. |
| Refactoring | 20 | 6+ small, well-named commits of 4+ types, including 2+ seam types. Each pushed separately. The game plays the same after each one. |

### Ready for week 2?

- [ ] Ken or Ben has seen your breakpoint hit
- [ ] `NOTES.md` has your game choice, game loop summary, state table, questions, manual test script, test list (5+), seam map summary, known issues and AI use
- [ ] Seam map tags in your game's code, kept up to date
- [ ] At least 6 `refactor(<type>):` commits, of at least 4 types, including at least 2 seam types
- [ ] The rules gathered into `update()` and a state `struct` in the game file, and the game still plays the same
- [ ] `session1` tag pushed at the end of the lab, and `week1` tag pushed before session 2

In session 2 you move your `update()` and state `struct` into your class, then do a test task. The faster the move, the more time you have for the task.

---

[^tag]: A **git tag** is a name, like `session1`, attached to one commit, like a bookmark. It records exactly what your repository looked like at that moment, even after you make more commits, so it is what the markers look at. `git push` on its own does **not** send tags to GitHub: push each one with `git push origin <tag>`. See [GIT_CHEATSHEET.md](https://github.com/kenpower/16_SFML_Games/blob/main/GIT_CHEATSHEET.md).

[^duck]: You sign in to cs50.ai with a **GitHub account**. Use the same account you use for this project, and check you can sign in before your first lab.
