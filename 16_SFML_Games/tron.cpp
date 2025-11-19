#include <SFML/Graphics.hpp>
#include <time.h>
#include <optional>
using namespace sf;

const int W=600;
const int H=480;
int speed = 4;
bool field[W][H]={0};

struct player
{
    int x,y,dir;
    Color color;

    player(Color c)
    {
        x=rand() % W;
        y=rand() % H;
        color=c;
        dir=rand() % 4;
    }

    void tick()
    {
        // move based on direction
        if (dir==0)
        {
            y+=1;
        }
        if (dir==1)
        {
            x-=1;
        }
        if (dir==2)
        {
            x+=1;
        }
        if (dir==3)
        {
            y-=1;
        }

        // wrap around screen
        if (x>=W)
        {
            x=0;
        }
        if (x<0)
        {
            x=W-1;
        }
        if (y>=H)
        {
            y=0;
        }
        if (y<0)
        {
            y=H-1;
        }
    }

    Vector3f getColor()
    {
        return Vector3f(color.r,color.g,color.b);
    }
};

int tron_messy()
{
    srand(time(0));

    RenderWindow window(VideoMode({W, H}), "The Tron Game!");
    window.setFramerateLimit(60);

    Texture texture;
    texture.loadFromFile("images/tron/background.jpg");
    Sprite sBackground(texture);

    player p1(Color::Red);
    player p2(Color::Green);

    RenderTexture t;
    t.resize({W, H});
    t.setSmooth(true);
    Sprite sprite(t.getTexture());
    t.clear();
    t.draw(sBackground);

    bool Game=1;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        // player 1 controls
        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            if (p1.dir!=2)
            {
                p1.dir=1;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            if (p1.dir!=1)
            {
                p1.dir=2;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
        {
            if (p1.dir!=0)
            {
                p1.dir=3;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Down))
        {
            if (p1.dir!=3)
            {
                p1.dir=0;
            }
        }

        // player 2 controls
        if (Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            if (p2.dir!=2)
            {
                p2.dir=1;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            if (p2.dir!=1)
            {
                p2.dir=2;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
        {
            if (p2.dir!=0)
            {
                p2.dir=3;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::S))
        {
            if (p2.dir!=3)
            {
                p2.dir=0;
            }
        }

        if (!Game)
        {
            continue;
        }

        // game loop
        for(int i=0;i<speed;i++)
        {
            p1.tick();
            p2.tick();

            // check collision
            if (field[p1.x][p1.y]==1)
            {
                Game=0;
            }
            if (field[p2.x][p2.y]==1)
            {
                Game=0;
            }

            field[p1.x][p1.y]=1;
            field[p2.x][p2.y]=1;

            // draw trail
            CircleShape c(3);
            c.setPosition({static_cast<float>(p1.x), static_cast<float>(p1.y)});
            c.setFillColor(p1.color);
            t.draw(c);

            c.setPosition({static_cast<float>(p2.x), static_cast<float>(p2.y)});
            c.setFillColor(p2.color);
            t.draw(c);

            t.display();
        }

       ////// draw  ///////
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
