#pragma once
// Catcher - the game rules, with no SFML.
// No window, no keyboard, no clock, no rand(): everything the rules need is passed in.
// That is what makes this class testable.

// ---- A plain rectangle, so the rules do not need sf::FloatRect ----
struct Rect
{
    float x, y, w, h;
};

// True when the rectangles share some area (touching edges do not count).
inline bool overlaps(Rect a, Rect b)
{
    return a.x < b.x + b.w && b.x < a.x + a.w
        && a.y < b.y + b.h && b.y < a.y + a.h;
}

class CatcherGame
{
public:
    // ---- Sizes and speeds, in pixels and pixels per second ----
    static constexpr float WIDTH = 400;
    static constexpr float HEIGHT = 500;
    static constexpr float BASKET_W = 80;
    static constexpr float BASKET_H = 15;
    static constexpr float BASKET_Y = 470;
    static constexpr float BASKET_START_X = 160;
    static constexpr float BASKET_SPEED = 300;
    static constexpr float FRUIT_SIZE = 20;
    static constexpr float FRUIT_START_SPEED = 100;
    static constexpr float FRUIT_SPEED_PER_POINT = 10;
    static constexpr int START_LIVES = 3;

    // ---- What the player is pressing this frame ----
    struct Input
    {
        bool left = false;
        bool right = false;
    };

    explicit CatcherGame(float firstFruitX) { restart(firstFruitX); }

    // Start a new game with the first fruit at the top of the screen, at fruitX.
    // The basket stays where it is, as it did in the original game.
    void restart(float fruitX)
    {
        score_ = 0;
        lives_ = START_LIVES;
        dropFruit(fruitX);
    }

    // Advance the game by dt seconds.
    // nextFruitX is only used if a new fruit is needed this frame (after a catch or a miss).
    void update(float dt, Input input, float nextFruitX)
    {
        if (isGameOver()) return;

        moveBasket(dt, input);

        fruitY_ += fruitSpeed() * dt;

        if (overlaps(fruitRect(), basketRect()))
        {
            score_++;
            dropFruit(nextFruitX);
        }
        else if (fruitY_ > HEIGHT)
        {
            lives_--;
            dropFruit(nextFruitX);
        }
    }

    // ---- Read-only state, for drawing and for tests ----
    float basketX() const { return basketX_; }
    float fruitX() const { return fruitX_; }
    float fruitY() const { return fruitY_; }
    int score() const { return score_; }
    int lives() const { return lives_; }
    bool isGameOver() const { return lives_ == 0; }
    float fruitSpeed() const { return FRUIT_START_SPEED + score_ * FRUIT_SPEED_PER_POINT; }

private:
    float basketX_ = BASKET_START_X;
    float fruitX_ = 0, fruitY_ = 0;
    int score_ = 0;
    int lives_ = START_LIVES;

    void moveBasket(float dt, Input input)
    {
        if (input.left)  basketX_ -= BASKET_SPEED * dt;
        if (input.right) basketX_ += BASKET_SPEED * dt;
        if (basketX_ < 0) basketX_ = 0;
        if (basketX_ > WIDTH - BASKET_W) basketX_ = WIDTH - BASKET_W;
    }

    void dropFruit(float x)
    {
        fruitX_ = x;
        fruitY_ = 0;
    }

    Rect basketRect() const { return {basketX_, BASKET_Y, BASKET_W, BASKET_H}; }
    Rect fruitRect() const { return {fruitX_, fruitY_, FRUIT_SIZE, FRUIT_SIZE}; }
};
