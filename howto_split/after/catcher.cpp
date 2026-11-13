// Catcher - AFTER the split.
// This file is now only the "shell": it reads the keyboard, the clock and rand(),
// passes them to CatcherGame, and draws what CatcherGame says.
// All the rules live in CatcherGame.h, which has no SFML and is unit tested.

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
#include "CatcherGame.h"
using namespace sf;

// The only place rand() is used. The game never calls it itself.
float randomFruitX()
{
    return float(rand() % int(CatcherGame::WIDTH - CatcherGame::FRUIT_SIZE));
}

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode({unsigned(CatcherGame::WIDTH), unsigned(CatcherGame::HEIGHT)}), "Catcher");
    window.setFramerateLimit(60);

    // Shapes are only used for drawing. Their positions come from the game every frame.
    RectangleShape basketShape({CatcherGame::BASKET_W, CatcherGame::BASKET_H});
    basketShape.setFillColor(Color::Yellow);

    CircleShape fruitShape(CatcherGame::FRUIT_SIZE / 2);
    fruitShape.setFillColor(Color::Red);

    CatcherGame game(randomFruitX());
    Clock clock;

    while (window.isOpen())
    {
        // ---- TIME ----
        float dt = clock.restart().asSeconds();

        // ---- INPUT: events ----
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();

            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Space && game.isGameOver())
                    game.restart(randomFruitX());
        }

        // ---- INPUT: held keys, turned into plain data ----
        CatcherGame::Input input;
        input.left = Keyboard::isKeyPressed(Keyboard::Key::Left);
        input.right = Keyboard::isKeyPressed(Keyboard::Key::Right);

        // ---- UPDATE: all the rules, in one call ----
        game.update(dt, input, randomFruitX());

        // ---- DRAW ----
        window.setTitle("Catcher  score: " + std::to_string(game.score()) + "  lives: " + std::to_string(game.lives()));

        basketShape.setPosition({game.basketX(), CatcherGame::BASKET_Y});
        fruitShape.setPosition({game.fruitX(), game.fruitY()});

        window.clear();
        window.draw(basketShape);
        window.draw(fruitShape);
        window.display();
    }

    return 0;
}
