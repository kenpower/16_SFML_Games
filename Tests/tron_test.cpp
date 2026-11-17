// Unit tests for Tron.
//
// Every .cpp in Tests/ is built into the Tests program automatically.
// Only work in the file for YOUR game.
//
// How to use it (week 2):
//   1. Your extracted class goes in 16_SFML_Games/TronArena.h, which is already included below.
//      The Tests program does not link SFML, so if that header pulls in SFML, this file
//      will not compile. That is deliberate: it proves your class is free of SFML.
//   2. Replace the placeholder test with real ones, named after behaviours, e.g.
//        TEST(TronArena, BikeCrashesIntoATrail) { ... }
//      Use Arrange / Act / Assert (see assignment_week2.md).

#include <gtest/gtest.h>
#include "TronArena.h"

using namespace tron_game;

// Placeholder: shows the class compiles in the Tests program. Delete it once you have a real test.
TEST(TronArena, NameIsTron)
{
    TronArena game;

    EXPECT_EQ("Tron", game.name());
}
