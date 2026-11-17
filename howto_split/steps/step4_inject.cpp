// Catcher - STEP 4: pass in time and randomness.
// Changed since step 3: rand() is called once per frame, at the top of the loop, into nextFruitX.
// The rules use nextFruitX and no longer call rand(). dt was already a plain float used by the rules.
// commit: refactor: pass the random fruit position into the rules
//
// SEAM MAP: [RULE] game logic   [KEY] keyboard/mouse   [TIME] clock   [RAND] rand()   [DRAW] SFML shapes/window
// A tag with more than one label (e.g. [RULE+DRAW]) is a tangle that has to be split.

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace sf;

const float WIDTH = 400, HEIGHT = 500;
const float BASKET_W = 80, BASKET_H = 15, BASKET_Y = 470, BASKET_START_X = 160, BASKET_SPEED = 300;
const float FRUIT_SIZE = 20;
const float FRUIT_START_SPEED = 100, FRUIT_SPEED_PER_POINT = 10;
const int START_LIVES = 3;

int main()
{
    srand(time(0));                                                         // [RAND]

    RenderWindow window(VideoMode({unsigned(WIDTH), unsigned(HEIGHT)}), "Catcher");   // [DRAW]
    window.setFramerateLimit(60);                                           // [DRAW]

    float basketX = BASKET_START_X;                                         // [RULE]
    float fruitX = rand() % int(WIDTH - FRUIT_SIZE), fruitY = 0;            // [RULE+RAND]

    RectangleShape basket({BASKET_W, BASKET_H});                            // [DRAW]
    basket.setFillColor(Color::Yellow);                                     // [DRAW]

    CircleShape fruit(FRUIT_SIZE / 2);                                      // [DRAW]
    fruit.setFillColor(Color::Red);                                         // [DRAW]

    int score = 0, lives = START_LIVES;                                     // [RULE]
    Clock clock;                                                            // [TIME]

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();                             // [TIME]
        float nextFruitX = rand() % int(WIDTH - FRUIT_SIZE);                // [RAND]

        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())                                     // [KEY]
                window.close();                                             // [DRAW]

            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Space && lives == 0)        // [KEY+RULE]
                {
                    score = 0;                                              // [RULE]
                    lives = START_LIVES;                                    // [RULE]
                    fruitX = nextFruitX;                                    // [RULE]
                    fruitY = 0;                                             // [RULE]
                }
        }

        bool left  = Keyboard::isKeyPressed(Keyboard::Key::Left);           // [KEY]
        bool right = Keyboard::isKeyPressed(Keyboard::Key::Right);          // [KEY]

        if (lives > 0)                                                      // [RULE]
        {
            if (left)  basketX -= BASKET_SPEED * dt;                        // [RULE+TIME]
            if (right) basketX += BASKET_SPEED * dt;                        // [RULE+TIME]
            if (basketX < 0) basketX = 0;                                   // [RULE]
            if (basketX > WIDTH - BASKET_W) basketX = WIDTH - BASKET_W;     // [RULE]

            fruitY += (FRUIT_START_SPEED + score * FRUIT_SPEED_PER_POINT) * dt;   // [RULE+TIME]

            if (FloatRect({fruitX, fruitY}, {FRUIT_SIZE, FRUIT_SIZE}).findIntersection(FloatRect({basketX, BASKET_Y}, {BASKET_W, BASKET_H})))   // [RULE+DRAW] collision uses SFML rectangles
            {
                score++;                                                    // [RULE]
                fruitX = nextFruitX;                                        // [RULE]
                fruitY = 0;                                                 // [RULE]
            }
            else if (fruitY > HEIGHT)                                       // [RULE]
            {
                lives--;                                                    // [RULE]
                fruitX = nextFruitX;                                        // [RULE]
                fruitY = 0;                                                 // [RULE]
            }

            window.setTitle("Catcher  score: " + std::to_string(score) + "  lives: " + std::to_string(lives));   // [DRAW]
        }

        basket.setPosition({basketX, BASKET_Y});                            // [DRAW]
        fruit.setPosition({fruitX, fruitY});                                // [DRAW]

        window.clear();                                                     // [DRAW]
        window.draw(basket);                                                // [DRAW]
        window.draw(fruit);                                                 // [DRAW]
        window.display();                                                   // [DRAW]
    }

    return 0;
}
