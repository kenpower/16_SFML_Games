# Project: Refactor, test and extend an SFML game

Get your starter code here: https://classroom.github.com/a/qObzKUBL

This is a **solo** project that runs for **three weeks**, with **one 100-minute lab session each week**, plus work at home between sessions (plan on about **2–3 hours** a week).

**Deadline:** the end of your week 3 lab session. Work pushed after that is not marked.

| Week  | Brief                                      | Goal: by the end of the week...                                       | In the lab                                           |
| ----- | ------------------------------------------ | --------------------------------------------------------------------- | ---------------------------------------------------- |
| **1** | [assignment_week1.md](assignment_week1.md) | **...I understand my game, and I have refactored it towards a seam.** | Build and debug check-off. Start refactoring.        |
| **2** | [assignment_week2.md](assignment_week2.md) *(draft)* | **...my game's rules live in a tested class with no SFML.**           | **Test task**, given at the start of the session.    |
| **3** | [assignment_week3.md](assignment_week3.md) *(draft)* | **...I have added a new feature to my class, test first.**            | **Feature task**, given at the start of the session. |

> **The week 2 and week 3 briefs are drafts.** They show the plan, so you can see where week 1 is heading, but details (including the numbers of tests and the marking) may change. Check back before each session: the draft notice is removed when a brief is final.

The in-lab tasks in weeks 2 and 3 are **not published in advance**. You get them at the start of the session and push your work before you leave. How well you do on them depends almost entirely on the work you did in the weeks before: a game you understand and a class you wrote yourself are quick to test and extend.

Each week builds on the one before. If you fall behind, **catch up before the next session**, using the "ready for next week" list at the end of each week's brief.

## What you will learn

By the end of this project you should be able to:

1. Build, run and debug an unfamiliar C++ project with CMake.
2. Read unfamiliar code systematically and explain what it does.
3. Describe the current behaviour of code _before_ changing it.
4. Refactor in small, safe, behaviour-preserving steps, with one refactoring per commit.
5. Recognise and name common code smells.
6. Find the **seam** between game rules and their dependencies (graphics, input, time, randomness), and split the code there so the rules can be tested.
7. Decide **what** to test, and write clear, focused unit tests with GoogleTest.
8. Check that your tests can actually catch mistakes.
9. Add a new feature **test first**, to code you now control.

Supporting material: the lecture notes on seams (`seams_lecture_notes.md`) and the worked example (`howto_split.md`), which splits a small game called Catcher.

## Marking

| Part                                                                               | Week | Where      | Marks |
| ---------------------------------------------------------------------------------- | ---: | ---------- | ----: |
| Understanding: breakpoint check-off, `NOTES.md` map, manual test script, test list |    1 | lab + home |    10 |
| Refactoring: 6+ commits, 4+ types, 2+ towards the seam                             |    1 | lab + home |    20 |
| Extracted class: meets the definition of done                                      |    2 | lab + home |    15 |
| **Test task**                                                                      |    2 | **in lab** |    15 |
| Your own tests and break-the-code experiments                                      |    2 | home       |    15 |
| **Feature task**, test first                                                       |    3 | **in lab** |    25 |

What a top mark looks like for each part is in that week's brief.

### Tags

You push a tag at two points each week:

| Tag | When | What it shows |
|-----|------|---------------|
| `session1`, `session2`, `session3` | at the **end of each lab session**, before you leave | what you did in the lab. The in-lab tasks are marked from `session2` and `session3`. |
| `week1`, `week2` | **before the next session** starts | what you did at home |

To tag and push (replace `session1` with the tag you need):

```sh
git push
git tag session1
git push origin session1
```

The tags and your push history are used to see how your work progressed. Work that appears in one large push at the end is looked at closely.

## Explaining your work

Ken or Ben may ask any student, in any session, to explain one of their commits or tests, or to show it working in the debugger. Questions are things like:

- Why does this commit not change the game's behaviour?
- Where did `rand()` (or the clock, or the keyboard) go, and why isn't it in your class?
- Talk me through this test. Break the code and show me it fail.

Work you can't explain may not be marked. You can't prepare a script for this. Understanding every commit is the preparation.

## Rules

- **Push after every commit.** GitHub records when each push arrives. A history of small, regular pushes is your evidence that the work is yours.
- Only change your chosen game's `.cpp` file and its test file, `Tests/<game>_test.cpp` (one is provided for each of the six games). You may **add** new files for extracted classes. Don't delete any other files.
- Put your extracted class in a **namespace named after your game**: `tetris_game`, `minesweeper_game`, `arkanoid_game`, `racing_game`, `tron_game` or `netwalk_game`. All the games are built into one program, so without a namespace a class called `Grid` or `Car` can silently clash with another game's class of the same name.
- You don't need to edit `CMakeLists.txt`. A header-only class (like `Grid.h`) in the `16_SFML_Games/` folder is already found by both the game and the `Tests` project, and any `.cpp` in `Tests/` is added to the test program. If you add a `.cpp` file for your class, you will need to add it to `CMakeLists.txt`.
- This is individual work. You can discuss ideas with classmates, but every commit must be your own.
- **AI tools:** you may use them to _explain_ code or concepts, not to write your changes. Record each use in an **AI use** section of `NOTES.md` (one line each, e.g. "asked what `std::array` is"). Honest declarations are not penalised.
- **In the lab, including during the in-lab tasks,** the only AI tool you may use is the CS50 Duck, [cs50.ai/chat](https://cs50.ai/chat), and only for technical questions (e.g. "what does this compiler error mean?", "how do I use `EXPECT_FLOAT_EQ`?"). It is designed to help you learn rather than hand you answers. Don't paste in your task and ask it to do it. Record what you asked in **AI use**, as above.
- If you miss a session, tell Ken or Ben as soon as you can.

## Submission

Just push regularly. There is nothing else to upload. Your repository is marked as it is at the end of your week 3 session, together with your tags (`session1`–`session3`, `week1`, `week2`).

At the end of week 3, you will be asked to fill in a short **self-assessment** form. This is your chance to explain what you did and how you did it. So it is important to keep your `NOTES.md` up to date, and to push your work regularly.
