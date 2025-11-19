#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
using namespace sf;

int arkanoid()
{
    srand(time(0));

    RenderWindow app(VideoMode({520, 450}), "Arkanoid!");
    app.setFramerateLimit(60);

    Texture t1,t2,t3,t4;
    t1.loadFromFile("images/arkanoid/block01.png");
    t2.loadFromFile("images/arkanoid/background.jpg");
    t3.loadFromFile("images/arkanoid/ball.png");
    t4.loadFromFile("images/arkanoid/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);

    std::vector<Sprite> block;

    // TODO: make this configurable
    for (int i=1;i<=10;i++)
    for (int j=1;j<=10;j++)
      {
         block.emplace_back(t1);
         block.back().setPosition({i*43.f, j*20.f});
      }

    int n = block.size();

    float a=6, b=5;
    float c=300, d=300;
    float x1 = 300;
    float y1 = 440;
    sPaddle.setPosition({x1, y1});
    int score = 0;  // not used yet
    bool gameOver = false;  // not used yet

    while (app.isOpen())
    {
       while (const std::optional event = app.pollEvent())
       {
         if (event->is<Event::Closed>())
             app.close();
       }

    // move ball
    c+=a;
    // collision detection - horizontal
    for (int i=0;i<n;i++)
    {
        float ballLeft = c+3;
        float ballTop = d+3;
        float ballWidth = 6;
        float ballHeight = 6;
        FloatRect ball_rect = FloatRect({ballLeft, ballTop}, {ballWidth, ballHeight});
        FloatRect block_rect = block[i].getGlobalBounds();
        std::optional<FloatRect> collision = ball_rect.findIntersection(block_rect);
        if (collision.has_value())
        {
            block[i].setPosition({-100, 0});
            a=-a;
            score = score + 10;  // increment score
        }
    }

    // move ball
    d+=b;
    // collision detection - vertical
    for (int i=0;i<n;i++)
    {
        float ballLeft = c+3;
        float ballTop = d+3;
        float ballWidth = 6;
        float ballHeight = 6;
        FloatRect ball_rect = FloatRect({ballLeft, ballTop}, {ballWidth, ballHeight});
        FloatRect block_rect = block[i].getGlobalBounds();
        std::optional<FloatRect> collision = ball_rect.findIntersection(block_rect);
        if (collision.has_value())
        {
            block[i].setPosition({-100, 0});
            b=-b;
            score = score + 10;  // increment score
        }
    }

    // wall collision
    if (c<0)
    {
        a=-a;
    }
    if (c>520)
    {
        a=-a;
    }
    if (d<0)
    {
        b=-b;
    }
    if (d>450)
    {
        b=-b;
    }

    // keyboard input
    if (Keyboard::isKeyPressed(Keyboard::Key::Right))
    {
        if (x1 < 520)  // stay on screen
        {
            x1 = x1 + 6;
            sPaddle.setPosition({x1, y1});
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::Left))
    {
        if (x1 > 0)  // stay on screen
        {
            x1 = x1 - 6;
            sPaddle.setPosition({x1, y1});
        }
    }

    // paddle collision
    float ballLeft2 = c;
    float ballTop2 = d;
    float ballWidth2 = 12;
    float ballHeight2 = 12;
    FloatRect ball_rect2 = FloatRect({ballLeft2, ballTop2}, {ballWidth2, ballHeight2});
    FloatRect paddle_rect = sPaddle.getGlobalBounds();
    std::optional<FloatRect> paddle_collision = ball_rect2.findIntersection(paddle_rect);
    if (paddle_collision.has_value())
    {
        int randomValue = rand()%5;
        int bounceSpeed = randomValue + 2;
        b = -bounceSpeed;
    }

    // update ball position
    sBall.setPosition({c, d});

    // rendering code
    app.clear();
    app.draw(sBackground);
    app.draw(sBall);
    app.draw(sPaddle);

    // draw blocks
    for (int i=0;i<n;i++)
    {
        app.draw(block[i]);
    }

    app.display();
    }

  return 0;
}
