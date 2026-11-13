#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;

// ===== GUIDE: Fifteen Puzzle =====
// Entry point is int fifteen_puzzle(). State that survives between frames: grid (tile numbers, 16 = the empty space),
// sprite (one Sprite per tile number) and w (tile size in pixels), all declared before the loop.
// Suggested reading order: SETUP, then the mouse-click handler inside the event loop, then DRAW at the bottom.
// =====
int fifteen_puzzle()
{
    // ---- SETUP: window and texture ----
    RenderWindow app(VideoMode({256,256}), "15-Puzzle!");
    app.setFramerateLimit(60);

    Texture t("images/fifteen_puzzle/15.png");
 
    // ---- SETUP: grid array and one sprite per tile ----
    int w = 64;
    int grid[6][6] = {0};
    std::vector<Sprite> sprite;
    for (int i=0;i<20;i++) sprite.emplace_back(t);

    // ---- SETUP: number the tiles and cut each tile's image from the texture ----
    int n=0;
    for (int i=0;i<4;i++)
     for (int j=0;j<4;j++)
      {
        n++;
        sprite[n].setTextureRect( IntRect({i*w,j*w},{w,w}) );
        grid[i+1][j+1]=n;
      }

    while (app.isOpen())
    {
        // ---- INPUT: window events ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

               // ---- UPDATE: a left click slides a tile into the empty space ----
               if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
                if (mb->button == Mouse::Button::Left)
                  {
                     Vector2i pos = Mouse::getPosition(app);
                     // HINT: the grid is 6x6 but the puzzle is 4x4 - why the extra border, and why the +1 here?
                     int x = pos.x/w + 1;
                     int y = pos.y/w + 1;

                     int dx=0;
                     int dy=0;

                     // HINT: 16 is the empty tile. Which neighbour is each of these four lines testing?
                     if (grid[x+1][y]==16) {dx=1; dy=0;};
                     if (grid[x][y+1]==16) {dx=0; dy=1;};
                     if (grid[x][y-1]==16) {dx=0; dy=-1;};
                     if (grid[x-1][y]==16) {dx=-1;dy=0;};

                     // HINT: put a breakpoint here and watch grid in the debugger - how do the numbers change after the swap?
                     int n = grid[x][y];
                     grid[x][y] = 16;
                     grid[x+dx][y+dy] = n;

                     // ---- ANIMATION: slide the tile across over several frames ----
                     // HINT: the grid swap above needs no SFML at all - could it live in its own class so it can be tested?
                     //animation
                     sprite[16].move({float(-dx*w),float(-dy*w)});
                     float speed=3;

                     for (int i=0;i<w;i+=speed)
                     {sprite[n].move({speed*dx,speed*dy});
                      app.draw(sprite[16]);
                      app.draw(sprite[n]);
                      app.display(); }
                  }

        }

       // ---- DRAW: every tile at its grid position ----
       app.clear(Color::White);
       for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
         {
            int n = grid[i+1][j+1];
            sprite[n].setPosition({float(i*w),float(j*w)});
            app.draw(sprite[n]);
          }

        app.display();
    }

    return 0;
}
