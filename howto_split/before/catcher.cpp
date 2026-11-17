// Catcher - BEFORE the split.
// Move the basket with Left/Right and catch the falling fruit.
// A caught fruit scores a point, a missed fruit costs a life. Space restarts after game over.
//
// Everything is mixed together: game rules, SFML shapes, keyboard, clock and rand().
// None of the rules can be unit tested, because they only exist inside this loop.
//
// SEAM MAP: each line that matters is tagged with what it depends on.
//   [RULE] game logic   [KEY] keyboard/mouse   [TIME] clock   [RAND] rand()   [DRAW] SFML shapes/window
// A tag with more than one label (e.g. [RULE+DRAW]) is a tangle: a line that has to be split
// before the rules can move into their own class. As you refactor, the tangles disappear.

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

    RectangleShape basket({80, 15});                                        // [RULE+DRAW] basket size and position live in a shape
    basket.setFillColor(Color::Yellow);                                     // [DRAW]
    basket.setPosition({160, 470});                                         // [RULE+DRAW]

    CircleShape fruit(10);                                                  // [RULE+DRAW] fruit size lives in a shape
    fruit.setFillColor(Color::Red);                                         // [DRAW]
    fruit.setPosition({float(rand() % 380), 0});                            // [RULE+RAND+DRAW]

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
                    fruit.setPosition({float(rand() % 380), 0});            // [RULE+RAND+DRAW]
                }
        }

        if (lives > 0)                                                      // [RULE]
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))  basket.move({-300 * dt, 0});   // [KEY+RULE+TIME+DRAW]
            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) basket.move({ 300 * dt, 0});   // [KEY+RULE+TIME+DRAW]
            if (basket.getPosition().x < 0)   basket.setPosition({0, 470});                  // [RULE+DRAW]
            if (basket.getPosition().x > 320) basket.setPosition({320, 470});                // [RULE+DRAW]

            fruit.move({0, (100 + score * 10) * dt});                       // [RULE+TIME+DRAW]

            if (fruit.getGlobalBounds().findIntersection(basket.getGlobalBounds()))   // [RULE+DRAW] collision uses SFML rectangles
            {
                score++;                                                    // [RULE]
                fruit.setPosition({float(rand() % 380), 0});                // [RULE+RAND+DRAW]
            }
            else if (fruit.getPosition().y > 500)                           // [RULE+DRAW]
            {
                lives--;                                                    // [RULE]
                fruit.setPosition({float(rand() % 380), 0});                // [RULE+RAND+DRAW]
            }

            window.setTitle("Catcher  score: " + std::to_string(score) + "  lives: " + std::to_string(lives));   // [DRAW]
        }

        window.clear();                                                     // [DRAW]
        window.draw(basket);                                                // [DRAW]
        window.draw(fruit);                                                 // [DRAW]
        window.display();                                                   // [DRAW]
    }

    return 0;
}
