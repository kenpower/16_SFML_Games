#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int minesweeper_messy()
{
    srand(time(0));

    RenderWindow app(VideoMode({400, 400}), "Minesweeper!");

    int w=32;
    int grid[12][12];
    int sgrid[12][12]; //for showing

    Texture t;
    t.loadFromFile("images/minesweeper/tiles.jpg");
    Sprite s(t);

    // initialize grid
    for (int i=1;i<=10;i++)
    {
        for (int j=1;j<=10;j++)
        {
            sgrid[i][j]=10;
            int randomValue = rand()%5;
            if (randomValue==0)
            {
                grid[i][j]=9;  // mine
            }
            else
            {
                grid[i][j]=0;
            }
        }
    }

    // count adjacent mines
    for (int i=1;i<=10;i++)
    {
        for (int j=1;j<=10;j++)
        {
            int n=0;
            if (grid[i][j]==9) continue;

            if (grid[i+1][j]==9)
            {
                n=n+1;
            }
            if (grid[i][j+1]==9)
            {
                n=n+1;
            }
            if (grid[i-1][j]==9)
            {
                n=n+1;
            }
            if (grid[i][j-1]==9)
            {
                n=n+1;
            }
            if (grid[i+1][j+1]==9)
            {
                n=n+1;
            }
            if (grid[i-1][j-1]==9)
            {
                n=n+1;
            }
            if (grid[i-1][j+1]==9)
            {
                n=n+1;
            }
            if (grid[i+1][j-1]==9)
            {
                n=n+1;
            }

            grid[i][j]=n;
        }
    }

    while (app.isOpen())
    {
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x/w;
        int y = pos.y/w;

        while (const std::optional event = app.pollEvent())
        {
            if (event->is<Event::Closed>())
                app.close();

            if (const auto* mousePressed = event->getIf<Event::MouseButtonPressed>())
            {
                if (mousePressed->button == Mouse::Button::Left)
                {
                    sgrid[x][y]=grid[x][y];
                }
                else if (mousePressed->button == Mouse::Button::Right)
                {
                    sgrid[x][y]=11;  // flag
                }
            }
        }

        app.clear(Color::White);

        // draw grid
        for (int i=1;i<=10;i++)
        {
            for (int j=1;j<=10;j++)
            {
                // reveal all mines if player hit one
                if (sgrid[x][y]==9)
                {
                    sgrid[i][j]=grid[i][j];
                }

                s.setTextureRect(IntRect({sgrid[i][j]*w, 0}, {w, w}));
                s.setPosition({static_cast<float>(i*w), static_cast<float>(j*w)});
                app.draw(s);
            }
        }

        app.display();
    }

    return 0;
}
