#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;

int fifteen_puzzle()
{
    RenderWindow app(VideoMode({256,256}), "15-Puzzle!");
    app.setFramerateLimit(60);

    Texture t("images/fifteen_puzzle/15.png");
 
    int w = 64;
    int grid[6][6] = {0};
    std::vector<Sprite> sprite;
    for (int i=0;i<20;i++) sprite.emplace_back(t);

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
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

               if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
                if (mb->button == Mouse::Button::Left)
                  {
                     Vector2i pos = Mouse::getPosition(app);
                     int x = pos.x/w + 1;
                     int y = pos.y/w + 1;

                     int dx=0;
                     int dy=0;

                     if (grid[x+1][y]==16) {dx=1; dy=0;};
                     if (grid[x][y+1]==16) {dx=0; dy=1;};
                     if (grid[x][y-1]==16) {dx=0; dy=-1;};
                     if (grid[x-1][y]==16) {dx=-1;dy=0;};

                     int n = grid[x][y];
                     grid[x][y] = 16;
                     grid[x+dx][y+dy] = n;

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
