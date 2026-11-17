# NOTES

**Game:** _(write your game here: Tetris, Minesweeper, Tron or Netwalk)_

_Edit this file in place. Commit it with a message that starts `docs:`. Keep it up to date and push often: it is part of your marks._

---

# Week 1

## Game loop

_The 3-4 big jobs the main loop does every frame. One line each._

1.
2.
3.

## State variables

_Every variable that lives between frames (globals, and locals declared before the loop). Say what it means, and which parts of the loop read or change it._

| Variable | What it means | Read / changed where |
|----------|---------------|----------------------|
| `lives` _(example from Catcher - replace it)_ | lives left | read in the window title; changed when a fruit is missed |
|          |               |                      |

## Questions

_Things you still don't understand about the code. A long list is normal at first. Cross items off (~~like this~~) when you answer them._

- [ ]
- [ ]

## Manual test script

_4-5 short scenarios you can play in under a minute each. You run all of them after every refactoring. Test real rules, not "the window opens"._

| Scenario | Steps | Expected result |
|----------|-------|-----------------|
| Line clear | Fill the bottom row by dropping pieces | The row disappears and everything above moves down one row |
|          |       |                 |

## Test list

_The game's rules as plain English, one per line. At least 5. Write what the game does (found by playing it), not what you think it should do._

- A new game starts with an empty board.
-
-
-
-

## Seam map

_How many tangles (tags with more than one label, like `[RULE+DRAW]`) you started with, and which ones look hardest. A few sentences._

- Tangles at the start:
- Hardest:

## Known issues

_Odd behaviour or bugs you find. Do not fix them inside a refactoring. Say how you found each one._

-

## AI use

_One line per use, e.g. "asked what `std::array` is". It may stay empty. Honest entries are not penalised._

-

---

# Week 2 (week 2 - fill in later)

## Test task

_Given at the start of session 2. Write the task here first. If a behaviour is not what your game does, say so here and explain the test you wrote instead._

## Behaviour table

_Turn your Test list into at least 8 rows. Be specific and check-able in code. One action per row (no "and")._

| Starting situation | Action | Expected result |
|--------------------|--------|-----------------|
| An empty board, a piece touching the left wall | Move the piece left | The piece stays where it is |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |

# Week 3 (week 3 - fill in later)

## Feature

_Given at the start of session 3. Fill this in before you write any code._

**Feature (2-3 sentences, in your own words):**

**Behaviour rows (2-4, with at least one edge case):**

| Starting situation | Action | Expected result |
|--------------------|--------|-----------------|
|  |  |  |
|  |  |  |

**Methods I plan to add to my class (and what the game file will call or show):**

-

**Notes at the end (a few sentences):** Did the feature work out as planned? Did a test catch a mistake? Did your week 2 class make this easier or harder, and why?
