// Catcher - BEFORE the split.
// Move the basket with Left/Right and catch the falling fruit.
// A caught fruit scores a point, a missed fruit costs a life. Space restarts after game over.
//
// Everything is mixed together: game rules, SFML shapes, keyboard, clock and rand().
// None of the rules can be unit tested, because they only exist inside this loop.

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace sf;

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode({400, 500}), "Catcher");
    window.setFramerateLimit(60);

    RectangleShape basket({80, 15});
    basket.setFillColor(Color::Yellow);
    basket.setPosition({160, 470});

    CircleShape fruit(10);
    fruit.setFillColor(Color::Red);
    fruit.setPosition({float(rand() % 380), 0});

    int score = 0, lives = 3;
    Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();

            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Space && lives == 0)
                {
                    score = 0;
                    lives = 3;
                    fruit.setPosition({float(rand() % 380), 0});
                }
        }

        if (lives > 0)
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))  basket.move({-300 * dt, 0});
            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) basket.move({ 300 * dt, 0});
            if (basket.getPosition().x < 0)   basket.setPosition({0, 470});
            if (basket.getPosition().x > 320) basket.setPosition({320, 470});

            fruit.move({0, (100 + score * 10) * dt});

            if (fruit.getGlobalBounds().findIntersection(basket.getGlobalBounds()))
            {
                score++;
                fruit.setPosition({float(rand() % 380), 0});
            }
            else if (fruit.getPosition().y > 500)
            {
                lives--;
                fruit.setPosition({float(rand() % 380), 0});
            }

            window.setTitle("Catcher  score: " + std::to_string(score) + "  lives: " + std::to_string(lives));
        }

        window.clear();
        window.draw(basket);
        window.draw(fruit);
        window.display();
    }

    return 0;
}
