# Week 2 – Split the game at the seam and test it (DRAFT)

> **DRAFT – this brief may change before session 2.** It shows the plan so you can see where week 1 is heading. Numbers, timings and marks are not final. This notice is removed when the brief is final.

Part of the three-week project **Refactor, test and extend an SFML game**. See [assignment.md](assignment.md) for the overview, marking and rules.

> **Goal:** by the end of this week, *my game's rules live in a tested class with no SFML.*
>
> **Done when:**
> - your class meets the [definition of done](#definition-of-done)
> - the **in-lab test task** is pushed by the end of session 2
> - `NOTES.md` has a behaviour table with at least **8** rows
> - you have at least **6** tests in total, including at least **2** edge cases
> - you have recorded **3** break-the-code experiments
> - the `week2` tag is pushed **before session 3**

## In the lab (100 minutes)

At the start of the session, Ken or Ben will give you a **test task** for your game: one or two behaviours to write unit tests for. You don't know them in advance.

| Time | What you do |
|------|-------------|
| 0–5 | Get your test task. Write it at the top of a **Test task** section in `NOTES.md`. |
| 5–65 | Step 6: extract your class, far enough that the behaviours in the task are inside it |
| 65–95 | Step 7: write the test task's tests, and make them pass |
| 95–100 | Push, and push the `session2` tag. Your test task is marked from that tag. |

You only need to extract the parts of your game the test task needs, so read the task first and plan. If your week 1 seam refactorings are done, the extraction is mostly moving code you've already untangled.

If you finish early, carry on with the home steps.

During the lab, the only AI tool allowed is [cs50.ai/chat](https://cs50.ai/chat), for technical questions. See the AI rule in [assignment.md](assignment.md#rules).

## At home

- Finish extracting the class, so it holds all the rules you listed in your test list.
- Write your behaviour table and your own tests (step 7).
- Do the break-the-code experiments (step 8).
- Push the `week2` tag.

---

## Step 6 – Extract a testable class

Move the game's rules into their own class, in their own header file (e.g. `16_SFML_Games/TetrisBoard.h`).

**Keep it focused.** One class with **3–4 public methods that contain real rules** is enough. For Tetris that could be the board, movement and line clearing, not the whole game. It must contain at least one rule that changes state based on a condition (a collision, a line clear, a reveal, a crash).

The quickest route is `howto_split.md` steps 6–8:

1. Gather the rules into one `update()` function (or a few functions) and one state `struct`, **still in the game file**. Play the game: it must still work.
2. Move them into your game's header, which is already provided with an empty class inside your game's namespace (e.g. `16_SFML_Games/TetrisBoard.h`). State becomes private members, functions become public methods, and the game draws from **getters**. Add `#include "TetrisBoard.h"` (or your game's header) to your game's `.cpp` file.
3. Build the `Tests` target. Your game's test file (e.g. `Tests/tetris_test.cpp`) already includes the header, so if your class pulls in SFML, it won't compile. Replace the placeholder `name()` test with real tests once you have some.

Commit each step separately, with `refactor(extract-class):`, and push.

### Definition of done

Your class must:

- [ ] Be in its own file (header-only is fine, like `Grid.h` in the Xonix example or `CatcherGame.h`), inside your game's namespace (`tetris_game`, `minesweeper_game`, …).
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
- **Use `EXPECT_FLOAT_EQ`** (or `EXPECT_NEAR`) for floats.

You may need small helper methods so a test can set up a situation or read a result, e.g. `setCell(r, c, value)` or `cellAt(r, c)`. That's normal. Getting the class into a known state is a big part of testing.

### 7a. The test task (in the lab)

Write the tests for the behaviours you were given. Commit them with `test: <task>` and push before you leave.

If a given behaviour turns out not to be what your game does, write a test for what it **does** do, name it honestly, and explain in the **Test task** section of `NOTES.md`.

### 7b. Your own tests (at home)

Turn your **Test list** into a **Behaviour table** in `NOTES.md`, with **at least 8 rows**:

| Starting situation | Action | Expected result |
|--------------------|--------|-----------------|
| An empty board, with a piece touching the left wall | Move the piece left | The piece stays where it is |
| A cell with a mine on all 8 sides | Count its neighbouring mines | The count is 8 |
| A ball moving right, at the right edge | Update the ball | Its horizontal direction reverses |

- **Be specific.** "The ball bounces" is too vague to test. "The ball's `dx` changes from `+6` to `-6`" can be tested.
- **One action per row.** If you need the word "and" in the action column, split it into two rows.
- **The expected result must be something you can check in code**: a return value, or a value you can read back from a getter.

To find more behaviours, look at your manual test script, at **both sides of every `if`** in your class, at loops with 0, 1 and the maximum number of items, and at **boundaries**: first and last row, corners, zero, empty, full.

Then write tests from the table until you have **at least 6 tests in total** (the test task's tests count), including at least **2** edge or boundary cases.

**When you're not sure what the right answer is**, write a test that asserts what the code **currently does**, and name it honestly. This is a *characterisation test*.

**When a test finds a bug**, well done. Do **not** delete the test or change it to match the wrong answer. Record the bug in **Known issues** in `NOTES.md`. Either fix it in a separate `fix:` commit, or leave the test failing and explain why.

### Check the tick on GitHub

Every time you push, GitHub builds your code and runs your tests. You can see a ✔ or ✘ next to your latest commit, or in the **Actions** tab. A ✘ because a test found a real bug is fine, as long as it's explained in `NOTES.md`. A ✘ because the tests **don't compile** must be fixed. The usual cause is SFML leaking into your header.

## Step 8 – Check your tests can fail (at home)

A test that can never fail is worthless. For **three** of your tests:

1. Deliberately break the code it tests. For example, change `<` to `<=`, remove a line, or return the wrong value.
2. Run the tests and check that **that** test fails.
3. Undo the change. Don't commit the broken code.

Record each experiment in a **Break-the-code** section of `NOTES.md`: what you broke, and which test caught it. If no test failed, you've found a missing test, so write it.

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
| Test task (in lab) | 15 | The given behaviours are tested, with clear names and Arrange/Act/Assert, and the tests pass (or an honest failure is explained) |
| Your own tests | 15 | Behaviour table with 8+ rows. 6+ tests in total that follow it, with 2+ edge cases. 3 break-the-code experiments, each caught by a test. |

### Ready for week 3?

- [ ] A class that meets the definition of done
- [ ] The test task pushed, and the `session2` tag pushed, at the end of session 2
- [ ] Behaviour table with at least 8 rows
- [ ] At least 6 tests in total, including at least 2 edge cases, all passing or explained
- [ ] 3 break-the-code experiments recorded
- [ ] `week2` tag pushed

In week 3 you'll be given a feature to add to your class, test first, in the lab. You'll do it fastest if your class is clean, its tests pass, and you know how to add a method and a test for it.
