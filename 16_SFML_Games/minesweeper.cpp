#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// ===== GUIDE: Minesweeper =====
// Entry point: int minesweeper(). There are no globals; the game is the while (app.isOpen()) loop.
// State that survives between frames: w, grid and sgrid, declared before the loop.
// Suggested reading order: grid / sgrid -> the two setup loops -> mouse input -> the draw loop.
// =====
int minesweeper()
{
    // ---- SETUP: window and textures ----
    srand(time(0));

    RenderWindow app(VideoMode({400, 400}), "Minesweeper!");

    // ---- GAME STATE: grid = the hidden answer, sgrid = what the player sees ----
    // HINT: the rules (mines, neighbour counts, reveal) need no SFML - could they live in a separate Board class?
    int w=32;
    int grid[12][12];
    // HINT: what do the values 0-11 in sgrid mean? Look at the tile image in images/minesweeper.
    int sgrid[12][12]; //for showing

    Texture t("images/minesweeper/tiles.jpg");
    Sprite s(t);

    // ---- SETUP: place the mines ----
    for (int i=1;i<=10;i++)
     for (int j=1;j<=10;j++)
      {
        sgrid[i][j]=10;
        if (rand()%5==0)  grid[i][j]=9;
        else grid[i][j]=0;
      }

    // ---- SETUP: count neighbouring mines for each cell ----
    for (int i=1;i<=10;i++)
     for (int j=1;j<=10;j++)
      {
        int n=0;
        if (grid[i][j]==9) continue;
        // HINT: eight near-identical lines - what refactoring would reduce the duplication?
        if (grid[i+1][j]==9) n++;
        if (grid[i][j+1]==9) n++;
        if (grid[i-1][j]==9) n++;
        if (grid[i][j-1]==9) n++;
        if (grid[i+1][j+1]==9) n++;
        if (grid[i-1][j-1]==9) n++;
        if (grid[i-1][j+1]==9) n++;
        if (grid[i+1][j-1]==9) n++;
        grid[i][j]=n;
      }

    while (app.isOpen())
    {
        // ---- INPUT: turn the mouse position into a cell ----
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x/w;
        int y = pos.y/w;

        // ---- INPUT: clicks change sgrid ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

            if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
              if (mb->button == Mouse::Button::Left) sgrid[x][y]=grid[x][y];
              else if (mb->button == Mouse::Button::Right) sgrid[x][y]=11;
        }

        // ---- DRAW: one sprite per cell ----
        app.clear(Color::White);

        for (int i=1;i<=10;i++)
         for (int j=1;j<=10;j++)
          {
           // HINT: step through this with the debugger after a click - which cell does this test use, and where did x and y come from?
           if (sgrid[x][y]==9) sgrid[i][j]=grid[i][j];
           s.setTextureRect(IntRect({sgrid[i][j]*w,0},{w,w}));
           s.setPosition({float(i*w), float(j*w)});
           app.draw(s);
          }

        app.display();
    }

    return 0;
}
