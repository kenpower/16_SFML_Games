#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
using namespace sf;

// ===== GUIDE: Arkanoid =====
// Entry point: int arkanoid(). No globals; the game is the while (app.isOpen()) loop.
// State that survives between frames: the block vector (and n), the ball's x, y, dx, dy, and the paddle sprite.
// Suggested reading order: block setup -> ball variables -> the loop (ball, walls, paddle, draw).
// =====
int arkanoid()
{
    // ---- SETUP: window, textures, sprites ----
    srand(time(0));

    RenderWindow app(VideoMode({520, 450}), "Arkanoid!");
    app.setFramerateLimit(60);

    Texture t1("images/arkanoid/block01.png");
    Texture t2("images/arkanoid/background.jpg");
    Texture t3("images/arkanoid/ball.png");
    Texture t4("images/arkanoid/paddle.png");

    Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition({300,440});

    // ---- SETUP: build the grid of blocks ----
    std::vector<Sprite> block;

    int n=0;
    for (int i=1;i<=10;i++)
    for (int j=1;j<=10;j++)
      {
         block.emplace_back(t1);
         // HINT: lots of raw pixel numbers in this file (43, 20, 6, 12 ...) - good candidates for named constants.
         block[n].setPosition({i*43.f,j*20.f});
         n++;
      }

    // ---- GAME STATE: ball position (x, y) and velocity (dx, dy) ----
    float dx=6, dy=5;
    float x=300, y=300;

    while (app.isOpen())
    {
       // ---- INPUT: window events ----
       while (const std::optional e = app.pollEvent())
       {
         if (e->is<Event::Closed>())
             app.close();
       }

    // ---- UPDATE: move ball horizontally, check blocks ----
    // HINT: x and y are moved and tested one after the other - what would change if they were moved together?
    x+=dx;
    for (int i=0;i<n;i++)
        if ( FloatRect({x+3,y+3},{6,6}).findIntersection(block[i].getGlobalBounds()) ) 
             // HINT: how is a 'destroyed' block represented here? What would a Block class look like?
             {block[i].setPosition({-100,0}); dx=-dx;}

    // ---- UPDATE: move ball vertically, check blocks ----
    y+=dy;
    for (int i=0;i<n;i++)
        if ( FloatRect({x+3,y+3},{6,6}).findIntersection(block[i].getGlobalBounds()) ) 
             {block[i].setPosition({-100,0}); dy=-dy;}

    // ---- UPDATE: bounce off the window edges ----
    if (x<0 || x>520)  dx=-dx;
    if (y<0 || y>450)  dy=-dy;

    // ---- INPUT: paddle keys ----
    if (Keyboard::isKeyPressed(Keyboard::Key::Right)) sPaddle.move({6,0});
    if (Keyboard::isKeyPressed(Keyboard::Key::Left)) sPaddle.move({-6,0});

    // ---- UPDATE: bounce off the paddle ----
    // HINT: set a breakpoint here and watch dy over several paddle hits - how does it differ from the -dy used elsewhere?
    if ( FloatRect({x,y},{12,12}).findIntersection(sPaddle.getGlobalBounds()) ) dy=-(rand()%5+2);

    sBall.setPosition({x,y});

    // ---- DRAW ----
    app.clear();
    app.draw(sBackground);
    app.draw(sBall);
    app.draw(sPaddle);

    for (int i=0;i<n;i++)
     app.draw(block[i]);

    app.display();
    }

  return 0;
}
