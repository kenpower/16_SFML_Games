// Unit tests for Racing.
//
// This file is already part of the Tests program: every .cpp in Tests/ is built automatically
// (re-run CMake after you add a new one). Only work in the file for YOUR game.
//
// How to use it (week 2):
//   1. Put your extracted class in its own header in 16_SFML_Games/, inside namespace racing_game,
//      e.g. 16_SFML_Games/RaceTrack.h  ->  namespace racing_game { class RaceTrack { ... }; }
//   2. Include it below. The Tests program does not link SFML, so if your header pulls in SFML,
//      this file will not compile. That is deliberate: it proves your class is free of SFML.
//   3. Replace the placeholder test with real ones, named after behaviours:
//        TEST(RaceTrack, CarStopsWhenNoKeyIsHeld) { ... }
//      Use your class name (not "Racing") as the first part of TEST(), so names don't clash with other files.

#include <gtest/gtest.h>
// #include "RaceTrack.h"
// using namespace racing_game;

// Placeholder: shows this file builds and runs. Delete it once you have a real test.
TEST(Racing, TestFileBuildsAndRuns)
{
    EXPECT_EQ(4, 2 + 2);
}
