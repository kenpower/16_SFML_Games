# Lab: Test and refactor SFML game part 2

**Deadline:** TBD

## What you will learn

By the end of this lab you should be able to:

1. Decide **what** to test by describing behaviour as a table of situations, actions and expected results before writing any code.
2. Write clear, focused unit tests with GoogleTest using Arrange / Act / Assert.
3. Choose test cases that matter: normal cases, edge cases and boundaries.
4. Recognise what makes code hard to test, and refactor it until it is testable.
5. Check that your tests can actually catch mistakes.

## Overview

| Step | What you do | What you produce |
|------|-------------|------------------|
| 0 | Check your part 1 class is testable, and fix it if not | A testable `.h` file |
| 1 | List the behaviours of your class | A behaviour table in `NOTES.md` |
| 2 | Turn the behaviours into tests | `<subject>_test.cpp` with at least 6 tests |
| 3 | Check your tests can fail | Section in `NOTES.md` |
| 4 | Submit | Links to your files |

---

## Step 0 – Is your class from part 1 testable?

You need a class or function you can test **without opening a window**. Answer these questions about the class you extracted in part 1:

| Question | If the answer is "yes" | Fix |
|----------|------------------------|-----|
| Does its file `#include` SFML, or include a file that does? | It is not testable yet | Replace SFML types with your own: `sf::Vector2f` → your own `struct Vec2 { float x, y; };`, `sf::FloatRect` → your own `struct Rect`. Keep SFML in the game file only. |
| Does it read or write global variables? | Tests will affect each other, and you can't set up a clean starting state | Move the globals into the class as members. |
| Does it call `rand()`? | You can't predict the result, so you can't assert on it | Pass the random value in as a parameter, e.g. `spawnPiece(int shapeIndex)`. The game calls `rand()` and passes the result in. |
| Does it read the clock (`sf::Clock`, `time()`)? | Results depend on how fast the test runs | Pass the elapsed time in, e.g. `update(float dt)`. |
| Does it read the keyboard or mouse? | You can't press keys in a test | Give the class methods like `moveLeft()` or `reveal(int row, int col)`. The game reads the input and calls the method. |
| Does it draw anything? | You can't check pixels in a unit test | Move drawing back into the game file. The game asks your class for its state (e.g. `cellAt(r, c)`) and draws that. |
| Is all the logic inside one big `update()` that returns nothing? | Hard to check what happened | Split out smaller methods that return a result, e.g. `bool isRowFull(int row)`, `int countNeighbourMines(int r, int c)`. |

### If you didn't finish part 1, or your class isn't testable

That's OK, but fix it **first**. You may refactor your game in this lab. Use `refactor:` commits as in part 1, and keep the game working.

The quickest way in is to extract a single **pure function**: one that takes values in and returns a value out, touching nothing else.

```cpp
// Before: buried in the game loop, reads globals
if (x < 0 || x >= WIDTH || y >= HEIGHT) ...

// After: in its own header, no SFML, no globals - easy to test
bool isOutsideBoard(int x, int y, int width, int height) {
    return x < 0 || x >= width || y >= height;
}
```

One pure function is a start, but it isn't enough on its own for good marks. Aim for a class that owns some game state (a board, a ball, a car) and has several methods that change it.

## Step 1 – List the behaviours before writing any tests

Most people get stuck here because they start by writing code. Don't. Start by filling in a table.

For your class, write a table of **8–10 behaviours**. Each row is one thing your class should do, split into three parts:

| Starting situation | Action | Expected result |
|--------------------|--------|-----------------|
| An empty board, with a piece touching the left wall | Move the piece left | The piece stays where it is |
| A cell with a mine on all 8 sides | Count its neighbouring mines | The count is 8 |
| A ball moving right, at the right edge | Update the ball | Its horizontal direction reverses |

Tips:

- **Be specific.** "The ball bounces" is too vague to test. "The ball's `dx` changes from `+6` to `-6`" can be tested.
- **One action per row.** If you need the word "and" in the action column, split it into two rows.
- **The expected result must be something you can check in code**: a return value, or a value you can read back from the object afterwards.

Each row becomes one test in step 2. The three columns map directly onto the three parts of a test: the starting situation becomes the **Arrange** section, the action becomes the **Act** section, and the expected result becomes the **Assert** section.

(Professional developers often write these as *Given … When … Then …* sentences. It's the same idea in a different layout.)

### Where to find behaviours

- **Your part 1 manual test script.** Each scenario you checked by hand is a behaviour you can now check automatically.
- **Every `if` statement in your class.** Each one is a decision, so test both sides of it.
- **Every loop.** What happens with 0 items? 1 item? The maximum?
- **Boundaries.** First row, last row, corners, zero speed, maximum speed, empty board, full board.
- **The rules of the game.** Write down what *should* happen if you played it, then check whether the code agrees.

### Behaviours to consider for the recommended games

These are prompts, not a checklist. Your class may not have all of these behaviours.

| Game | Behaviours worth testing |
|------|--------------------------|
| Tetris | A piece can't move through a wall or another block. A piece stops when it lands. A full row is cleared and the rows above move down. A row that isn't full is not cleared. A rotation that would overlap something is undone. |
| Minesweeper | Neighbour counts in the middle, on an edge, in a corner, with 0 and with 8 mines. Revealing a cell shows its value. Flagging a cell. Revealing a mine. |
| Arkanoid | The ball bounces off each wall. The ball bounces off a brick, and the brick is destroyed. The ball bounces off the paddle. What happens when a brick is hit twice? |
| Racing | The car moves in the direction it faces. It accelerates up to a maximum speed. It slows to a stop with no input. An AI car moves on to the next checkpoint when it is close enough. The checkpoint number wraps back to the start. |
| Bejeweled | Three in a row is a match, horizontally and vertically. Two in a row is not. A swap that makes no match is undone. Gems fall into gaps. |

## Step 2 – Write the tests

Write **at least 6 tests** in a file called `<subject>_test.cpp` in the `Tests` folder, e.g. `Tests/tetris_board_test.cpp`. Each test should come from one row of your behaviour table.

### Add your test file to the build

Any `.cpp` file in the `Tests` folder is added to the test program automatically, so you don't need to edit `CMakeLists.txt`. After creating your file:

1. Re-run CMake (in Visual Studio: *Project → Configure Cache*; or run `cmake -S . -B build`).
2. Build the `Tests` target.
3. Run the tests: `ctest --test-dir build -C Debug --output-on-failure`, or use your IDE's Test Explorer.

Put the class you're testing in a header file (`.h`) in `16_SFML_Games/`, like `Grid.h` in the Xonix example. The test program doesn't include SFML, so **if your class includes SFML, the tests won't compile**. That is deliberate.

### Check the tick on GitHub

Every time you push, GitHub builds your code and runs your tests. You can see the result as a ✔ or ✘ next to your latest commit, or in the **Actions** tab of your repository. There are two checks:

- **Unit tests (no SFML):** your tests compile without SFML, and they pass.
- **Game builds (Windows):** the whole game still compiles.

A ✘ on the unit-test check doesn't always mean you did something wrong. A test that has found a real bug will fail, and that's fine as long as you explain it (see "When a test finds a bug" below). A ✘ because the tests **don't compile** must be fixed.

### Structure every test the same way: Arrange / Act / Assert

```cpp
TEST(TetrisBoard, FullRowIsClearedAndRowsAboveMoveDown) {
    // Arrange - set up the situation (the starting situation)
    TetrisBoard board;
    board.fillRow(19);
    board.setCell(18, 3, 5);

    // Act - do ONE thing (the action)
    board.clearFullRows();

    // Assert - check the result (the expected result)
    EXPECT_EQ(5, board.cellAt(19, 3));
    EXPECT_EQ(0, board.cellAt(18, 3));
}
```

### Good tests

- **Have names that read like the behaviour:** `PieceCannotMovePastLeftWall`, not `Test1` or `MoveTest`. Use `TEST(ClassName, BehaviourItChecks)`.
- **Check one behaviour each.** Several `EXPECT`s are fine if they all check the same behaviour.
- **Are independent.** Each test builds its own objects. No test relies on another having run first.
- **Don't need the game window**, and run in milliseconds.
- **Cover more than the easy case.** Include at least two edge or boundary cases (walls, corners, empty, full, zero).

### Adding methods so you can test

You may need small helper methods so a test can set up a situation or check a result, e.g. `setCell(r, c, value)` or `cellAt(r, c)`. That's normal and fine. Getting the class into a known state is a big part of testing.

### When you're not sure what the right answer is

Sometimes you won't know whether the current behaviour is correct. Write a test that asserts what the code **currently does**, and name it honestly, e.g. `BallSpeedAfterPaddleHitIsBetween2And6`. This is called a *characterisation test*, and it protects existing behaviour while you refactor.

### When a test finds a bug

Well done, that's what tests are for. Do **not** delete the test or change it to match the wrong answer. Record the bug in the "known issues" list in `NOTES.md`. Then either:
- fix the bug in a separate `fix:` commit, and the test will pass, or
- leave the test failing and explain why in `NOTES.md`.

## Step 3 – Check your tests can fail

A test that can never fail is worthless. For at least **three** of your tests:

1. Deliberately break the code it tests. For example, change `<` to `<=`, remove a line, or return the wrong value.
2. Run the tests and check that **that** test fails.
3. Undo the change. Don't commit the broken code.

Record each experiment in `NOTES.md`: what you broke, and which test caught it. If no test failed, you've found a missing test, so write it.

## Grading

| Criterion | Weight | What a top mark looks like |
|-----------|--------|----------------------------|
| Subject under test | 20% | A class with real game logic and state. No SFML, no globals, no `rand()`, clock or input inside. |
| Choice of tests | 30% | Tests come from listed behaviours, and cover both sides of the important decisions and at least two edge cases. |
| Test quality | 25% | Names describe behaviour, Arrange/Act/Assert is clear, one behaviour per test, tests are independent. |
| Tests can fail | 15% | At least three recorded break-the-code experiments, each caught by a test. |
| Everything builds and runs | 10% | The game builds and plays. The `Tests` target builds and the tests run. Failing tests are explained. |

## Submission

On Blackboard, submit GitHub links to:

1. `Tests/<subject>_test.cpp`, your test file, e.g. `https://github.com/KenPowerClassroom/<your-repo>/blob/main/Tests/tetris_board_test.cpp`
2. `16_SFML_Games/<Subject>.h`, the class or function being tested
3. `NOTES.md`, with your behaviour table, break-the-code experiments and any known issues

### Checklist

- [ ] The subject under test has no SFML, globals, `rand()`, clock or input
- [ ] A behaviour table with at least 8 rows, written before the tests
- [ ] At least 6 tests, each named after a behaviour, including at least 2 edge cases
- [ ] At least 3 break-the-code experiments recorded
- [ ] The game still builds and plays
- [ ] The `Tests` target builds and the tests run
