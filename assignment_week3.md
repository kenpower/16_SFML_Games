# Week 3 – Add a feature, test first (DRAFT)

> **DRAFT – this brief may change before session 3.** It shows the plan so you can see where the project is heading. Timings and marks are not final. This notice is removed when the brief is final.

Part of the three-week project **Refactor, test and extend an SFML game**. See [assignment.md](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md) for the overview, marking and rules.

> **Goal:** by the end of this week, *I have added a new feature to my class, test first.*
>
> **Done when:** the **feature task** is pushed by the end of session 3, with failing tests committed before the code that makes them pass.

This week is all in the lab. **The deadline is the end of session 3.**

## Before the session

- Make sure your week 2 work is pushed and the `week2` tag is there.
- Pull your repository on the lab machine, build both the game and `Tests`, and check that the tests pass. Do this **before** the session if you can, so you don't lose lab time to setup.

## In the lab (100 minutes)

At the start of the session, Ken or Ben will give you the **feature task**. It is the same kind of task for every game:

> Add **a counter** or **a win/lose condition** to your class, **test first**.

You also get a short list of options for your game (for example, for Tetris: count the rows cleared, or end the game when a new piece can't be placed). Choose one. You don't know the list in advance.

| Time | Step |
|------|------|
| 0–10 | Get the task and choose your feature. Pull, build, run the tests. Write the feature at the top of the **Feature** section in `NOTES.md`. |
| 10–20 | 1 Plan |
| 20–45 | 2 Red: write failing tests |
| 45–70 | 3 Green: make them pass |
| 70–85 | 4 Wire it into the game |
| 85–95 | 5 Tidy, and finish `NOTES.md` |
| 95–100 | Push, and push the `session3` tag. The feature is marked from that tag. |

The times are a guide. The **order** is what matters: the failing tests must be committed before the code that makes them pass.

During the lab, the only AI tool allowed is [cs50.ai/chat](https://cs50.ai/chat)[^duck], for technical questions. See the AI rule in [assignment.md](https://github.com/kenpower/16_SFML_Games/blob/main/assignment.md#rules).

---

## Step 1 – Plan

In the **Feature** section of `NOTES.md`:

- In two or three sentences, say what the feature does, in your own words.
- Add **2–4 rows** to your behaviour table for it (starting situation, action, expected result). Include at least one **edge case**: the last life, not the first; exactly the score where something changes; an empty board.
- Write down which method(s) you will add to your class, and what the game file will call or show.

The feature's logic must go in **your class**, not the game file. The game file only calls it when input happens, and shows the result (the window title is enough, e.g. `window.setTitle("Lives: " + std::to_string(game.lives()))`).

## Step 2 – Red: write failing tests

Write a test for each row you planned, in your game's test file (e.g. `Tests/tetris_test.cpp`). Use the same Arrange / Act / Assert structure and behaviour names as in week 2.

Build and run. The tests should **fail**, or not compile because the method doesn't exist yet. That's the point: it proves the tests check something the code doesn't do yet.

If a test passes straight away, it isn't testing the new feature. Fix the test.

Commit and push: `test: add failing tests for <feature>`.

## Step 3 – Green: make them pass

Write the **simplest** code in your class that makes the new tests pass. Then run **all** the tests: the old ones must still pass too.

Commit and push: `feat: <feature>`.

## Step 4 – Wire it into the game

Make the game file use the feature: call the new method when input happens, or show its result. Play the game and check it works. Run your manual test script to check nothing else broke.

Commit and push: `feat: show <feature> in the game`.

## Step 5 – Tidy, and finish `NOTES.md`

- If the new code is messy, refactor it with the tests still passing, and commit with `refactor(<type>):`.
- In the **Feature** section of `NOTES.md`, add a few sentences: did the feature work out as you planned? Did a test catch a mistake while you were writing it? Did your week 2 class make this easier or harder, and why?

---

## Marking for week 3

| Part | Marks | What a top mark looks like |
|------|------:|----------------------------|
| Feature task (in lab) | 25 | A plan and behaviour rows written first. 2+ tests for the feature, including an edge case, committed **failing** before the `feat:` commit. The logic is in the class, not the game file. All tests pass. The feature is visible in the game. A short, honest note in `NOTES.md`. |

A partly working feature with good tests, committed in the right order, gets more marks than a working feature with no tests.

### Checklist before you leave

- [ ] **Feature** section in `NOTES.md`: description, behaviour rows, methods planned, notes at the end
- [ ] At least 2 feature tests, including an edge case
- [ ] A `test:` commit **before** the `feat:` commit
- [ ] The logic is in your class, and the game shows it
- [ ] The `Tests` target builds and all tests pass (or failures are explained)
- [ ] Everything pushed, and the `session3` tag pushed

---

[^duck]: You sign in to cs50.ai with a **GitHub account**. Use the same account you use for this project, and check you can sign in before your first lab.
