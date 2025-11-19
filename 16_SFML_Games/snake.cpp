#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int N=30,M=20;
int sz=16;
int w = sz*N;
int h = sz*M;

int dir,num=4;

struct Snake
{ int x,y;} s[100];

struct Fruit
{ int x,y;} f;

void Tick()
 {
    for (int i=num;i>0;--i)
     {s[i].x=s[i-1].x; s[i].y=s[i-1].y;}

    if (dir==0) s[0].y+=1;
    if (dir==1) s[0].x-=1;
    if (dir==2) s[0].x+=1;
    if (dir==3) s[0].y-=1;

    if ((s[0].x==f.x) && (s[0].y==f.y))
     {num++; f.x=rand()%N; f.y=rand()%M;}

    if (s[0].x>N) s[0].x=0;  if (s[0].x<0) s[0].x=N;
    if (s[0].y>M) s[0].y=0;  if (s[0].y<0) s[0].y=M;

    for (int i=1;i<num;i++)
     if (s[0].x==s[i].x && s[0].y==s[i].y)  num=i;
 }

int snake()
{
    srand(time(0));

    RenderWindow window(VideoMode({static_cast<unsigned int>(w), static_cast<unsigned int>(h)}), "Snake Game!");

    Texture t1,t2;
    t1.loadFromFile("images/snake/white.png");
    t2.loadFromFile("images/snake/red.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);

    Clock clock;
    float timer=0, delay=0.1;

    f.x=10;
    f.y=10;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        // handle keyboard input
        if (Keyboard::isKeyPressed(Keyboard::Key::Left))
        {
            dir=1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Right))
        {
            dir=2;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Up))
        {
            dir=3;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Down))
        {
            dir=0;
        }

        if (timer>delay)
        {
            timer=0;
            Tick();
        }

   // clear screen
    window.clear();

    // draw background tiles
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<M; j++)
        {
            sprite1.setPosition({static_cast<float>(i*sz), static_cast<float>(j*sz)});
            window.draw(sprite1);
        }
    }

    // draw snake segments
    for (int i=0;i<num;i++)
    {
        sprite2.setPosition({static_cast<float>(s[i].x*sz), static_cast<float>(s[i].y*sz)});
        window.draw(sprite2);
    }

    // draw fruit
    sprite2.setPosition({static_cast<float>(f.x*sz), static_cast<float>(f.y*sz)});
    window.draw(sprite2);

    window.display();
    }

    return 0;
}
