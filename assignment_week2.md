# Week 2 – Split the game at the seam and test it (DRAFT)

> **DRAFT – this brief may change before session 2.** It shows the plan so you can see where week 1 is heading. Numbers, timings and marks are not final. This notice is removed when the brief is final.

Part of the three-week project **Refactor, test and extend an SFML game**. See [assignment.md](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md) for the overview, marking and rules.

> **Goal:** by the end of this week, *my game's rules live in a tested class with no SFML.*
>
> **Done when:**
> - your class meets the [definition of done](#definition-of-done)
> - the **in-lab test task** is pushed by the end of session 2
> - `NOTES.md` has a behaviour table with at least **8** rows
> - you have at least **6** tests in total, including at least **2** edge cases
> - the `week2` tag is pushed **before session 3**

## Before the lab

Your week 1 home work ends with the rules gathered into an `update()` function (or a few functions) and a state `struct`, **still in your game file** (week 1, step 5b). If that isn't done, do it before session 2: it makes the lab much easier.

## In the lab (100 minutes)

| Time | What you do |
|------|-------------|
| 0–40 | Step 6: move your state struct and `update()` into your game's header, make them a class with getters, and make the game use it. Check the `Tests` target builds. |
| 40–45 | Get the **test task** (below). Copy it into the **Test task** section of `NOTES.md`. |
| 45–95 | Step 7a: add the task's rows to your behaviour table, write the tests, and make them pass |
| 95–100 | Push, and push the `session2` tag. Your test task is marked from that tag. |

### The test task

At 40 minutes, Ken or Ben will give you the test task. It is the same kind of task for every game:

> Choose **one rule** in your class. Write **3 tests** for it:
> 1. a **normal case**: the rule does what it should in an ordinary situation
> 2. an **edge case**: the rule at a boundary (a wall, a corner, the first or last row, zero, full)
> 3. a case where the rule **does not apply**: the situation where nothing should happen
>
> Add the 3 rows to your behaviour table **before** you write the tests.

You also get a short sheet of example rows for your game. Use them if you can't think of a rule, or if the rule you wanted isn't in your class yet.

If you finish early, carry on with the home steps.

During the lab, the only AI tool allowed is [cs50.ai/chat](https://cs50.ai/chat)[^duck], for technical questions. See the AI rule in [assignment.md](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md#rules).

## At home

- Finish the class, so it holds all the rules from your test list, and the game draws from its getters.
- Grow your behaviour table to **8+ rows**, and your tests to **6+ in total** (step 7b).
- Push the `week2` tag.

---

## Step 6 – Extract a testable class

Move the game's rules into their own class, in their own header file (e.g. `16_SFML_Games/TetrisBoard.h`).

**Keep it focused.** One class with **3–4 public methods that contain real rules** is enough. For Tetris that could be the board, movement and line clearing, not the whole game. It must contain at least one rule that changes state based on a condition (a collision, a line clear, a reveal, a crash).

You start the lab with the rules already gathered into `update()` and a state `struct` in your game file (week 1). Now follow [howto_split.md](https://github.com/kenpower/split-at-the-seam/blob/main/howto_split.md) steps 7–8:

1. Move them into your game's header, which is already provided with an empty class inside your game's namespace (e.g. `16_SFML_Games/TetrisBoard.h`). State becomes private members, functions become public methods, and the game draws from **getters**. Add `#include "TetrisBoard.h"` (or your game's header) to your game's `.cpp` file.
2. Build the `Tests` target. Your game's test file (e.g. `Tests/tetris_test.cpp`) already includes the header, so if your class pulls in SFML, it won't compile. Replace the placeholder `name()` test with real tests once you have some.

Commit each step separately, with `refactor(extract-class):`, and push.

### Definition of done

Your class must:

- [ ] Be in its own file (header-only is fine, like `Grid.h` in the Xonix example or [`CatcherGame.h`](https://github.com/kenpower/split-at-the-seam/blob/main/howto_split/after/CatcherGame.h)), inside your game's namespace (`tetris_game`, `minesweeper_game`, …).
- [ ] **Not include SFML**, directly or through another header.
- [ ] **Not use global variables.** Everything it needs is passed to its constructor or methods, or stored as a member.
- [ ] **Not call `rand()`, read the clock, or read the keyboard or mouse.** If it needs randomness, time or input, take it as a parameter (e.g. `spawnPiece(int shapeIndex)`, `update(float dt)`, `reveal(int row, int col)`).
- [ ] Have **getters** for the state the game needs to draw, so the game draws from the class.
- [ ] Be used by the game, which still plays exactly as before. Run your manual test script.
- [ ] Compile in the `Tests` project.

If you need positions or rectangles, use `plain::Point`, `plain::Vec2f` and `plain::Rect` (with `plain::overlaps()`) from `16_SFML_Games/PlainTypes.h`, not the SFML types.

Search your header for `SFML`, `rand`, `time`, `Clock` and `Keyboard`. None of them should appear. The compiler checks the rest: the test project doesn't link SFML, so if your class pulls in SFML, the tests won't compile.

### Where to look

| Game | Logic that doesn't need SFML |
|------|------------------------------|
| Tetris | The board, collision checking, rotation, line clearing |
| Minesweeper | Placing mines, counting neighbours, revealing cells |
| Tron | The bikes, the trail map, steering, crash detection |
| Netwalk | The pipe grid, rotating pipes, finding which cells connect to the server, puzzle generation |

Drawing and input stay in the game file, because they are the parts that need SFML.

## Step 7 – Write the tests

### Structure every test the same way: Arrange / Act / Assert

```cpp
TEST(TetrisBoard, FullRowIsClearedAndRowsAboveMoveDown) {
    // Arrange - set up the situation
    TetrisBoard board;
    board.fillRow(19);
    board.setCell(18, 3, 5);

    // Act - do ONE thing
    board.clearFullRows();

    // Assert - check the result
    EXPECT_EQ(5, board.cellAt(19, 3));
    EXPECT_EQ(0, board.cellAt(18, 3));
}
```

Good tests:

- **Have names that read like the behaviour:** `PieceCannotMovePastLeftWall`, not `Test1` or `MoveTest`. Use `TEST(ClassName, BehaviourItChecks)`.
- **Check one behaviour each.** Several `EXPECT`s are fine if they all check the same behaviour.
- **Are independent.** Each test builds its own objects. No test relies on another having run first.
- **Don't need the game window**, and run in milliseconds.
- **Use `EXPECT_EQ`, `EXPECT_TRUE` and `EXPECT_FALSE`.** Your game keeps its state in whole numbers and `bool`s, so you can compare exactly.

You may need small helper methods so a test can set up a situation or read a result, e.g. `setCell(r, c, value)` or `cellAt(r, c)`. That's normal. Getting the class into a known state is a big part of testing.

### 7a. The test task (in the lab)

Write the 3 tests from the task: normal case, edge case, and a case where the rule doesn't apply. Commit them with `test: <rule>` and push before you leave.

If the rule turns out not to do what you expected, write a test for what it **does** do, name it honestly, and explain in the **Test task** section of `NOTES.md`.

### 7b. Your own tests (at home)

Turn your **Test list** into a **Behaviour table** in `NOTES.md`, with **at least 8 rows**:

| Starting situation | Action | Expected result |
|--------------------|--------|-----------------|
| An empty board, with a piece touching the left wall | Move the piece left | The piece stays where it is |
| A cell with a mine on all 8 sides | Count its neighbouring mines | The count is 8 |
| A bike moving right, with a trail in the next cell | Step the game once | The game is over |

- **Be specific.** "The piece stops" is too vague to test. "The piece's cells are copied into the board, and a new piece starts at the top" can be tested.
- **One action per row.** If you need the word "and" in the action column, split it into two rows.
- **The expected result must be something you can check in code**: a return value, or a value you can read back from a getter.

To find more behaviours, look at your manual test script, at **both sides of every `if`** in your class, at loops with 0, 1 and the maximum number of items, and at **boundaries**: first and last row, corners, zero, empty, full.

Then write tests from the table until you have **at least 6 tests in total** (the test task's tests count), including at least **2** edge or boundary cases.

**When you're not sure what the right answer is**, write a test that asserts what the code **currently does**, and name it honestly. This is a *characterisation test*.

**When a test finds a bug**, well done. Do **not** delete the test or change it to match the wrong answer. Record the bug in **Known issues** in `NOTES.md`. Either fix it in a separate `fix:` commit, or leave the test failing and explain why.

### Check the tick on GitHub

Every time you push, GitHub builds your code and runs your tests. You can see a ✔ or ✘ next to your latest commit, or in the **Actions** tab. A ✘ because a test found a real bug is fine, as long as it's explained in `NOTES.md`. A ✘ because the tests **don't compile** must be fixed. The usual cause is SFML leaking into your header.

---

## End of week 2

You pushed the `session2` tag at the end of the lab. Push your home work and tag it **before session 3**:

```sh
git push
git tag week2
git push origin week2
```

### Marking for week 2

| Part | Marks | What a top mark looks like |
|------|------:|----------------------------|
| Extracted class | 15 | Meets every item of the definition of done. The game draws from its getters. The class owns real rules. |
| Test task (in lab) | 15 | Rows written first. 3 tests (normal, edge, doesn't apply) for one real rule, with clear names and Arrange/Act/Assert. They pass, or an honest failure is explained. |
| Your own tests | 15 | Behaviour table with 8+ rows. 6+ tests in total that follow it, with 2+ edge cases, clear names and Arrange/Act/Assert. |

### Ready for week 3?

- [ ] A class that meets the definition of done
- [ ] The test task pushed, and the `session2` tag pushed, at the end of session 2
- [ ] Behaviour table with at least 8 rows
- [ ] At least 6 tests in total, including at least 2 edge cases, all passing or explained
- [ ] `week2` tag pushed

In week 3 you'll be given a feature to add to your class, test first, in the lab. You'll do it fastest if your class is clean, its tests pass, and you know how to add a method and a test for it.

---

[^duck]: You sign in to cs50.ai with a **GitHub account**. Use the same account you use for this project, and check you can sign in before your first lab.
