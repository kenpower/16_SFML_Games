# Instructor notes: extracting testable logic (lab part 1, step 5)

Instructor only. Do not commit this to the repository students clone.

How hard it is to split each game into an SFML-free, testable class, for students relatively new to C++. Ratings are 1 (trivial) to 10 (very hard). Line numbers are as of 2026-11-13.

## Summary

| Game | Rating | Status in the brief |
|------|-------:|---------------------|
| Fifteen Puzzle | 2 | Not allowed (too simple) |
| Snake | 3 | Not allowed (too simple) |
| Doodle Jump | 3 | Not allowed (too simple) |
| Racing | 4 | Recommended |
| Arkanoid | 4 | Recommended |
| Tron | 4 | Recommended |
| Minesweeper | 4 | Recommended |
| Netwalk | 4–5 | Recommended |
| Tetris | 5 | Recommended |
| Mahjong | 5 | Allowed |
| Outrun | 6 | Allowed (stretch) |
| Bejeweled | 7 | Allowed (stretch) |
| Chess | 7 | Allowed (stretch) |
| Asteroids | 8 | Allowed (stretch) |
| Xonix | — | Not allowed (worked example) |
| Volleyball | — | Not built (needs Box2D, still SFML 2 API) |

## Too simple

**Fifteen Puzzle (2).** The grid swap is about 10 lines of pure int code with a zero border that prevents index errors. There is no shuffle and no win check. It would only be enough work if those were required.

**Snake (3).** `Tick()` (snake.cpp:28) has no SFML already. The work is turning the globals `s[100]`, `dir`, `f` and `num` into members and injecting the RNG for fruit placement.

**Doodle Jump (3).** About 15 lines of gravity, landing and scroll code, plus an RNG for platforms. `dx` is unused. `y` is an `int` and `dy` is a `float`, so `y += dy` truncates.

## Recommended

**Racing (4).** `Car` (racing.cpp:23) is already SFML-free, so extracting `Car` on its own is **not enough** for step 5. Expect a world or track class that owns:
- the checkpoints (global `points[]`, which `Car::findTarget` reads directly)
- the player's acceleration and steering (racing.cpp:98-117, currently locals in `racing()`)
- car-to-car collision (racing.cpp:127-140)

Tests need float tolerances.

**Arkanoid (4).** Replace `sf::FloatRect` with their own rect struct and the `x=-100` sentinel with an alive flag. Inject the paddle-bounce RNG (`rand()%5+2`, arkanoid.cpp:87). Block and paddle sizes come from texture sizes, so they must become constructor parameters. Blocks are resolved in separate X and Y passes (arkanoid.cpp:65-75). The ball never leaves through the bottom, so there is no lose state.

**Tron (4).** `Bike` is nearly pure apart from `rand()` in its constructor. Make the global `field[W][H]`, `speed`, `W` and `H` members. Separate the step loop from the per-step `RenderTexture` drawing (tron.cpp:85-98). Two-player input is duplicated, so a `setDirection(player, dir)` that rejects 180° reversals is a natural method.

**Minesweeper (4).** The rules are small, but revealing happens inside the draw loop (minesweeper.cpp:79), which tests `sgrid[x][y]` for the hover cell. Mine placement needs an RNG. The eight neighbour checks (minesweeper.cpp:43-50) are duplicated.

**Netwalk (4–5).** Mostly grid and graph code. `GridPos` has no SFML, and `drop()` is a recursive flood fill. The work is:
- turning the global `grid` into an instance
- injecting the RNG into generation and shuffling (netwalk.cpp:70-95, 147, 153)
- moving `pipe.angle`, which is animation state, out of the model

A good test: an unshuffled generated board is fully connected from the server. There is no win check.

**Tetris (5).** The board, collision, rotation and line-clear are all pure, but they are spread over the globals `field`, `a` and `b`, and `check()` reads those globals. The `timer > delay` accumulator becomes `step(dt)`, and the next shape and colour must be injected (tetris.cpp:114-115). Rotation restores from `b`, which is saved before the sideways move, so a failed rotation also undoes that frame's sideways move.

## Stretch

**Mahjong (5).** Integer logic with no animation, but:
- there is a global `field[50][50][50]` behind the `f()` accessor
- the layout is loaded from a file (use an `std::istream`), with no error check
- the deal uses `rand()`
- the pixel-to-tile mapping is duplicated in input and drawing
- selection state `v1`/`v2` is not reset between clicks

`isOpen()` (mahjong.cpp:31-43) and undo are good test targets.

**Outrun (6).** `Line::project` writes screen-space cache values into the track, and the draw loop writes `clip` back. Off-road detection lives in the draw section. There is a hidden `static int speed` inside the loop (outrun.cpp:153), and the track geometry is a hard-coded chain of `if`s.

**Bejeweled (7).** `piece` mixes grid `col/row` with pixel `x/y`. The rules only run when `!isMoving`, and `match` is incremented every frame while a match stands. The local `score` is really an "any matches?" flag used for the swap-back. Students have to redefine what the rules mean (find matches, remove, apply gravity, refill) rather than move code. The initial fill uses `rand()%3` and the refill uses `rand()%7`. Border sentinels have `kind` 0.

**Chess (7).** The model stores pixel floats, not squares. Castling is detected with `position.find("e1")`. The Stockfish connector is a process-wide singleton with no injection point. A proper split needs a `Square` type, a game class, and a move-source interface with a fake engine for tests. Interfaces and test doubles are likely beyond this cohort.

**Asteroids (8).** `Entity` owns an `Animation` that owns an `sf::Sprite`. It uses an inheritance hierarchy with raw `new`/`delete`, string-name type checks, and `rand()` in constructors. This needs a redesign, not an extraction.

## Bugs students' tests may expose

These were verified by reading the code, not by running it. None are flagged in the student-facing comments.

| Game | Location | Bug |
|------|----------|-----|
| Minesweeper | minesweeper.cpp:19-21 | **Fixed in the starter code (2026-11-13):** `grid` and `sgrid` were uninitialised, so neighbour counts on edge cells read garbage from border rows/columns 0 and 11. Both are now zero-initialised. |
| Minesweeper | minesweeper.cpp:57-68 | The cell comes from `pos.x/w` with no bounds check. A click at x ≥ 384 (or y ≥ 384) gives index 12 and writes out of bounds. |
| Racing | racing.cpp:133 | `car[i].x+=dy/10.0;` should update `y`. Collision push-apart is lopsided. `dx` and `dy` are `int`s, so small separations truncate to 0. |
| Snake | snake.cpp:43-44 | Wrapping tests `> N` / `< 0` and wraps to `N`, so the head can sit at column N (or row M), one cell off-screen. |
| Snake | snake.cpp:20, 41 | `s[100]` has no bounds check as `num` grows. Fruit can spawn on the body. |
| Tetris | tetris.cpp:129-139 | Line-clear never copies row 0 down and doesn't clear the rows freed at the top. This only shows when the stack reaches the top row. |
| Tron | tron.cpp:89-92 | Both bikes are checked against `field` before either is written, so a head-on move into the same cell in one step is not detected. |
