// Unit tests for Tetris.
//
// This file is already part of the Tests program: every .cpp in Tests/ is built automatically
// (re-run CMake after you add a new one). Only work in the file for YOUR game.
//
// How to use it (week 2):
//   1. Put your extracted class in its own header in 16_SFML_Games/, inside namespace tetris_game,
//      e.g. 16_SFML_Games/TetrisBoard.h  ->  namespace tetris_game { class TetrisBoard { ... }; }
//   2. Include it below. The Tests program does not link SFML, so if your header pulls in SFML,
//      this file will not compile. That is deliberate: it proves your class is free of SFML.
//   3. Replace the placeholder test with real ones, named after behaviours:
//        TEST(TetrisBoard, PieceCannotMovePastLeftWall) { ... }
//      Use your class name (not "Tetris") as the first part of TEST(), so names don't clash with other files.

#include <gtest/gtest.h>
// #include "TetrisBoard.h"
// using namespace tetris_game;

// Placeholder: shows this file builds and runs. Delete it once you have a real test.
TEST(Tetris, TestFileBuildsAndRuns)
{
    EXPECT_EQ(4, 2 + 2);
}
