// Catcher - STEP 1: keep the state in plain variables.
// Changed since "before": basketX, fruitX and fruitY are plain floats. The shapes are only used to draw.
// The collision check builds FloatRects from the plain values and the sizes written down by hand.
// commit: refactor: keep basket and fruit positions in plain variables
//
// SEAM MAP: [RULE] game logic   [KEY] keyboard/mouse   [TIME] clock   [RAND] rand()   [DRAW] SFML shapes/window
// A tag with more than one label (e.g. [RULE+DRAW]) is a tangle that has to be split.

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace sf;

int main()
{
    srand(time(0));                                                         // [RAND]

    RenderWindow window(VideoMode({400, 500}), "Catcher");                  // [DRAW]
    window.setFramerateLimit(60);                                           // [DRAW]

    float basketX = 160;                                                    // [RULE]
    float fruitX = rand() % 380, fruitY = 0;                                // [RULE+RAND]

    RectangleShape basket({80, 15});                                        // [DRAW]
    basket.setFillColor(Color::Yellow);                                     // [DRAW]

    CircleShape fruit(10);                                                  // [DRAW]
    fruit.setFillColor(Color::Red);                                         // [DRAW]

    int score = 0, lives = 3;                                               // [RULE]
    Clock clock;                                                            // [TIME]

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();                             // [TIME]

        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())                                     // [KEY]
                window.close();                                             // [DRAW]

            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Space && lives == 0)        // [KEY+RULE]
                {
                    score = 0;                                              // [RULE]
                    lives = 3;                                              // [RULE]
                    fruitX = rand() % 380;                                  // [RULE+RAND]
                    fruitY = 0;                                             // [RULE]
                }
        }

        if (lives > 0)                                                      // [RULE]
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))  basketX -= 300 * dt;   // [KEY+RULE+TIME]
            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) basketX += 300 * dt;   // [KEY+RULE+TIME]
            if (basketX < 0)   basketX = 0;                                          // [RULE]
            if (basketX > 320) basketX = 320;                                        // [RULE]

            fruitY += (100 + score * 10) * dt;                              // [RULE+TIME]

            if (FloatRect({fruitX, fruitY}, {20, 20}).findIntersection(FloatRect({basketX, 470}, {80, 15})))   // [RULE+DRAW] collision uses SFML rectangles
            {
                score++;                                                    // [RULE]
                fruitX = rand() % 380;                                      // [RULE+RAND]
                fruitY = 0;                                                 // [RULE]
            }
            else if (fruitY > 500)                                          // [RULE]
            {
                lives--;                                                    // [RULE]
                fruitX = rand() % 380;                                      // [RULE+RAND]
                fruitY = 0;                                                 // [RULE]
            }

            window.setTitle("Catcher  score: " + std::to_string(score) + "  lives: " + std::to_string(lives));   // [DRAW]
        }

        basket.setPosition({basketX, 470});                                 // [DRAW]
        fruit.setPosition({fruitX, fruitY});                                // [DRAW]

        window.clear();                                                     // [DRAW]
        window.draw(basket);                                                // [DRAW]
        window.draw(fruit);                                                 // [DRAW]
        window.display();                                                   // [DRAW]
    }

    return 0;
}
