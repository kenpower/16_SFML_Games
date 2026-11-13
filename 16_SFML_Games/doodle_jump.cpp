#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// ===== GUIDE: Doodle Jump =====
// Entry point is int doodle_jump(). Everything that survives between frames is a local declared before the loop:
// plat[] (platform positions), x and y (player), h (scroll threshold), dx and dy (dy is the vertical speed).
// Suggested reading order: SETUP, then the UPDATE sections in the loop from top to bottom, then DRAW.
// =====
// ---- DATA: a simple x,y position, used for the platforms ----
struct point
{ int x,y;};

int doodle_jump()
{
    // ---- SETUP: window, textures, sprites ----
    srand(time(0));

    RenderWindow app(VideoMode({400, 533}), "Doodle Game!");
    app.setFramerateLimit(60);

    Texture t1("images/doodle_jump/background.png");
    Texture t2("images/doodle_jump/platform.png");
    Texture t3("images/doodle_jump/doodle.png");

    Sprite sBackground(t1), sPlat(t2), sPers(t3);

    // ---- SETUP: random starting platforms and player state ----
    point plat[20];

    // HINT: 400 and 533 appear in several places - what would you have to change if the window size changed?
    for (int i=0;i<10;i++)
      {
       plat[i].x=rand()%400;
       plat[i].y=rand()%533;
      }

    int x=100,y=100,h=200;
    float dx=0,dy=0;

    while (app.isOpen())
    {
        // ---- INPUT: window events ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();
        }

    // ---- INPUT: left/right movement ----
    if (Keyboard::isKeyPressed(Keyboard::Key::Right)) x+=3;
    if (Keyboard::isKeyPressed(Keyboard::Key::Left)) x-=3;

    // ---- UPDATE: gravity and bouncing off the floor ----
    // HINT: put a breakpoint here and watch y and dy for a few frames - what is dy doing as the doodle rises and falls?
    dy+=0.2;
    y+=dy;
    if (y>500)  dy=-10;

    // ---- UPDATE: scroll the platforms when the player is high up ----
    // HINT: the player is held at y=h while the platforms move instead - why does that look like a camera following?
    if (y<h)
    for (int i=0;i<10;i++)
    {
      y=h;
      plat[i].y=plat[i].y-dy;
      if (plat[i].y>533) {plat[i].y=0; plat[i].x=rand()%400;}
    }

    // ---- UPDATE: landing on a platform ----
    // HINT: 50, 20, 68, 70 and 14 look like sprite pixel sizes - good candidates for named constants.
    for (int i=0;i<10;i++)
     if ((x+50>plat[i].x) && (x+20<plat[i].x+68)
      && (y+70>plat[i].y) && (y+70<plat[i].y+14) && (dy>0))  dy=-10;

    // ---- DRAW ----
    sPers.setPosition({float(x),float(y)});

    app.draw(sBackground);
    app.draw(sPers);
    for (int i=0;i<10;i++)
    {
    sPlat.setPosition({float(plat[i].x),float(plat[i].y)});
    app.draw(sPlat);
    }

    app.display();
}

    return 0;
}
