#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

struct point
{ int x,y;};

int doodle_jump_messy()
{
    srand(time(0));

    RenderWindow app(VideoMode({400, 533}), "Doodle Game!");
    app.setFramerateLimit(60);

    Texture t1,t2,t3;
    t1.loadFromFile("images/doodle_jump/background.png");
    t2.loadFromFile("images/doodle_jump/platform.png");
    t3.loadFromFile("images/doodle_jump/doodle.png");

    Sprite sBackground(t1), sPlat(t2), sPers(t3);

    point plat[20];

    // initialize platforms
    for (int i=0;i<10;i++)
    {
        plat[i].x=rand()%400;
        plat[i].y=rand()%533;
    }

    int x=100;
    int y=100;
    int h=200;
    float dx=0;
    float dy=0;
    bool isJumping = false;  // not used yet

    while (app.isOpen())
    {
        while (const std::optional event = app.pollEvent())
        {
            if (event->is<Event::Closed>())
                app.close();
        }

    // player movement
    if (Keyboard::isKeyPressed(Keyboard::Key::Right))
    {
        x=x+3;
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::Left))
    {
        x=x-3;
    }

    // gravity
    dy=dy+0.2;
    y=y+dy;

    // check if hit bottom
    if (y>500)
    {
        dy=-10;
    }

    // scroll screen
    if (y<h)
    {
        for (int i=0;i<10;i++)
        {
            y=h;
            plat[i].y=plat[i].y-dy;
            if (plat[i].y>533)
            {
                plat[i].y=0;
                plat[i].x=rand()%400;
            }
        }
    }

    // platform collision
    for (int i=0;i<10;i++)
    {
        int playerLeft = x+20;
        int playerRight = x+50;
        int platformLeft = plat[i].x;
        int platformRight = plat[i].x+68;
        int playerBottom = y+70;
        int platformTop = plat[i].y;
        int platformBottom = plat[i].y+14;

        bool horizontalOverlap = (playerRight>platformLeft) && (playerLeft<platformRight);
        bool verticalOverlap = (playerBottom>platformTop) && (playerBottom<platformBottom);
        bool movingDown = (dy>0);

        if (horizontalOverlap && verticalOverlap && movingDown)
        {
            dy=-10;
        }
    }

    sPers.setPosition({static_cast<float>(x), static_cast<float>(y)});

    // render everything
    app.draw(sBackground);
    app.draw(sPers);

    // draw platforms
    for (int i=0;i<10;i++)
    {
        sPlat.setPosition({static_cast<float>(plat[i].x), static_cast<float>(plat[i].y)});
        app.draw(sPlat);
    }

    app.display();
    }

    return 0;
}
