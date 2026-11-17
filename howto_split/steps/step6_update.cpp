// Catcher - STEP 6: gather the rules into one function.
// Changed since step 5: CatcherState holds the game data and update() holds all the rules (still in this file).
// The loop now reads: input -> update -> draw. randomFruitX() is the only place rand() is called.
// commit: refactor: extract update() and CatcherState from the game loop
//
// SEAM MAP: [RULE] game logic   [KEY] keyboard/mouse   [TIME] clock   [RAND] rand()   [DRAW] SFML shapes/window
// A tag with more than one label (e.g. [RULE+DRAW]) is a tangle that has to be split.

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>

struct Rect { float x, y, w, h; };                                          // [RULE]

bool overlaps(Rect a, Rect b)                                               // [RULE]
{
    return a.x < b.x + b.w && b.x < a.x + a.w
        && a.y < b.y + b.h && b.y < a.y + a.h;
}

using namespace sf;

const float WIDTH = 400, HEIGHT = 500;
const float BASKET_W = 80, BASKET_H = 15, BASKET_Y = 470, BASKET_START_X = 160, BASKET_SPEED = 300;
const float FRUIT_SIZE = 20;
const float FRUIT_START_SPEED = 100, FRUIT_SPEED_PER_POINT = 10;
const int START_LIVES = 3;

// ---- The rules: plain data in, plain data out ----
struct CatcherState                                                         // [RULE]
{
    float basketX = BASKET_START_X;
    float fruitX = 0, fruitY = 0;
    int score = 0, lives = START_LIVES;
};

bool isGameOver(const CatcherState& s)                                      // [RULE]
{
    return s.lives == 0;
}

void restart(CatcherState& s, float fruitX)                                 // [RULE]
{
    s.score = 0;
    s.lives = START_LIVES;
    s.fruitX = fruitX;
    s.fruitY = 0;
}

void update(CatcherState& s, float dt, bool left, bool right, float nextFruitX)   // [RULE]
{
    if (s.lives == 0) return;                                               // [RULE]

    if (left)  s.basketX -= BASKET_SPEED * dt;                              // [RULE]
    if (right) s.basketX += BASKET_SPEED * dt;                              // [RULE]
    if (s.basketX < 0) s.basketX = 0;                                       // [RULE]
    if (s.basketX > WIDTH - BASKET_W) s.basketX = WIDTH - BASKET_W;         // [RULE]

    s.fruitY += (FRUIT_START_SPEED + s.score * FRUIT_SPEED_PER_POINT) * dt; // [RULE]

    if (overlaps({s.fruitX, s.fruitY, FRUIT_SIZE, FRUIT_SIZE}, {s.basketX, BASKET_Y, BASKET_W, BASKET_H}))   // [RULE]
    {
        s.score++;                                                          // [RULE]
        s.fruitX = nextFruitX;                                              // [RULE]
        s.fruitY = 0;                                                       // [RULE]
    }
    else if (s.fruitY > HEIGHT)                                             // [RULE]
    {
        s.lives--;                                                          // [RULE]
        s.fruitX = nextFruitX;                                              // [RULE]
        s.fruitY = 0;                                                       // [RULE]
    }
}

// The only place rand() is used.
float randomFruitX()                                                        // [RAND]
{
    return float(rand() % int(WIDTH - FRUIT_SIZE));                         // [RAND]
}

int main()
{
    srand(time(0));                                                         // [RAND]

    RenderWindow window(VideoMode({unsigned(WIDTH), unsigned(HEIGHT)}), "Catcher");   // [DRAW]
    window.setFramerateLimit(60);                                           // [DRAW]

    RectangleShape basket({BASKET_W, BASKET_H});                            // [DRAW]
    basket.setFillColor(Color::Yellow);                                     // [DRAW]

    CircleShape fruit(FRUIT_SIZE / 2);                                      // [DRAW]
    fruit.setFillColor(Color::Red);                                         // [DRAW]

    CatcherState state;                                                     // [RULE]
    state.fruitX = randomFruitX();                                          // [RAND]
    Clock clock;                                                            // [TIME]

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();                             // [TIME]

        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())                                     // [KEY]
                window.close();                                             // [DRAW]

            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Space && isGameOver(state)) // [KEY]
                    restart(state, randomFruitX());                         // [RULE]
        }

        bool left  = Keyboard::isKeyPressed(Keyboard::Key::Left);           // [KEY]
        bool right = Keyboard::isKeyPressed(Keyboard::Key::Right);          // [KEY]

        update(state, dt, left, right, randomFruitX());                     // [RULE] all of the rules

        window.setTitle("Catcher  score: " + std::to_string(state.score) + "  lives: " + std::to_string(state.lives));   // [DRAW]

        basket.setPosition({state.basketX, BASKET_Y});                      // [DRAW]
        fruit.setPosition({state.fruitX, state.fruitY});                    // [DRAW]

        window.clear();                                                     // [DRAW]
        window.draw(basket);                                                // [DRAW]
        window.draw(fruit);                                                 // [DRAW]
        window.display();                                                   // [DRAW]
    }

    return 0;
}
