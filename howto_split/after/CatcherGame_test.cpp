// Unit tests for CatcherGame. No window opens: the rules run on their own.
// Each test sets up a game, feeds it time and input, and checks the result.

#include <gtest/gtest.h>
#include "CatcherGame.h"

using Input = CatcherGame::Input;

const Input NO_KEYS;
const Input LEFT{true, false};
const Input RIGHT{false, true};

// ---- Starting state ----

TEST(CatcherGame, StartsWithThreeLivesAndNoScore)
{
    CatcherGame game(100);

    EXPECT_EQ(3, game.lives());
    EXPECT_EQ(0, game.score());
    EXPECT_FALSE(game.isGameOver());
}

TEST(CatcherGame, FirstFruitStartsAtTopAtGivenX)
{
    CatcherGame game(123);

    EXPECT_FLOAT_EQ(123, game.fruitX());
    EXPECT_FLOAT_EQ(0, game.fruitY());
}

// ---- Basket movement ----

TEST(CatcherGame, BasketMovesLeftWhileLeftIsHeld)
{
    CatcherGame game(0);

    game.update(0.1f, LEFT, 0);

    EXPECT_FLOAT_EQ(160 - 30, game.basketX());
}

TEST(CatcherGame, BasketMovesRightWhileRightIsHeld)
{
    CatcherGame game(0);

    game.update(0.1f, RIGHT, 0);

    EXPECT_FLOAT_EQ(160 + 30, game.basketX());
}

TEST(CatcherGame, BasketStopsAtLeftEdge)
{
    CatcherGame game(0);

    game.update(1.0f, LEFT, 0);

    EXPECT_FLOAT_EQ(0, game.basketX());
}

TEST(CatcherGame, BasketStopsAtRightEdge)
{
    CatcherGame game(0);

    game.update(1.0f, RIGHT, 0);

    EXPECT_FLOAT_EQ(400 - 80, game.basketX());
}

// ---- Falling fruit ----

TEST(CatcherGame, FruitFallsAt100PixelsPerSecondAtStart)
{
    CatcherGame game(0);

    game.update(1.0f, NO_KEYS, 0);

    EXPECT_FLOAT_EQ(100, game.fruitY());
}

// ---- Catching and missing ----

// Helper: run the game in small steps until the score or lives change (or give up).
void runUntilSomethingHappens(CatcherGame& game, float nextFruitX)
{
    int score = game.score(), lives = game.lives();
    for (int frame = 0; frame < 1000; frame++)
    {
        game.update(0.1f, NO_KEYS, nextFruitX);
        if (game.score() != score || game.lives() != lives) return;
    }
}

TEST(CatcherGame, FruitAboveBasketIsCaught)
{
    CatcherGame game(170);  // basket starts at x 160..240, so this fruit lands in it

    runUntilSomethingHappens(game, 50);

    EXPECT_EQ(1, game.score());
    EXPECT_EQ(3, game.lives());
}

TEST(CatcherGame, CaughtFruitIsReplacedAtTopAtNextX)
{
    CatcherGame game(170);

    runUntilSomethingHappens(game, 50);

    EXPECT_FLOAT_EQ(50, game.fruitX());
    EXPECT_FLOAT_EQ(0, game.fruitY());
}

TEST(CatcherGame, FruitAwayFromBasketIsMissedAndCostsALife)
{
    CatcherGame game(0);  // basket starts at x 160..240, so this fruit misses

    runUntilSomethingHappens(game, 0);

    EXPECT_EQ(0, game.score());
    EXPECT_EQ(2, game.lives());
}

TEST(CatcherGame, FruitFallsFasterAfterEachCatch)
{
    CatcherGame game(170);

    runUntilSomethingHappens(game, 170);

    EXPECT_FLOAT_EQ(110, game.fruitSpeed());
}

// ---- Game over and restart ----

TEST(CatcherGame, ThreeMissesIsGameOver)
{
    CatcherGame game(0);

    for (int i = 0; i < 3; i++)
        runUntilSomethingHappens(game, 0);

    EXPECT_EQ(0, game.lives());
    EXPECT_TRUE(game.isGameOver());
}

TEST(CatcherGame, NothingMovesAfterGameOver)
{
    CatcherGame game(0);
    for (int i = 0; i < 3; i++)
        runUntilSomethingHappens(game, 0);
    float fruitY = game.fruitY();

    game.update(1.0f, LEFT, 0);

    EXPECT_FLOAT_EQ(fruitY, game.fruitY());
    EXPECT_FLOAT_EQ(160, game.basketX());
}

TEST(CatcherGame, RestartResetsScoreAndLives)
{
    CatcherGame game(0);
    for (int i = 0; i < 3; i++)
        runUntilSomethingHappens(game, 0);

    game.restart(42);

    EXPECT_EQ(3, game.lives());
    EXPECT_EQ(0, game.score());
    EXPECT_FLOAT_EQ(42, game.fruitX());
    EXPECT_FLOAT_EQ(0, game.fruitY());
}

// A characterization test: it records what the original game did, not what we think it should do.
TEST(CatcherGame, RestartLeavesBasketWhereItWas)
{
    CatcherGame game(0);
    game.update(1.0f, LEFT, 0);  // basket now at the left edge

    game.restart(42);

    EXPECT_FLOAT_EQ(0, game.basketX());
}

// ---- The rectangle helper ----

TEST(Overlaps, RectanglesSharingAreaOverlap)
{
    EXPECT_TRUE(overlaps({0, 0, 10, 10}, {5, 5, 10, 10}));
}

TEST(Overlaps, RectanglesFarApartDoNotOverlap)
{
    EXPECT_FALSE(overlaps({0, 0, 10, 10}, {50, 50, 10, 10}));
}

TEST(Overlaps, RectanglesOnlyTouchingDoNotOverlap)
{
    EXPECT_FALSE(overlaps({0, 0, 10, 10}, {10, 0, 10, 10}));
}
