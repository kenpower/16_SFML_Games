#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// ===== GUIDE: Tron =====
// Entry point is int tron(). State that survives between frames: the globals W, H, speed and field[][] (the trail map),
// plus the locals p1, p2, t (a RenderTexture holding the picture of the trails) and Game, declared before the loop.
// Suggested reading order: struct player, then SETUP in tron(), then the loop: INPUT, UPDATE, DRAW.
// =====
// ---- GLOBAL STATE: arena size, speed, trail map ----
const int W=600;
const int H=480;
int speed = 4;
static bool field[W][H]={0};

// ---- DATA + BEHAVIOUR: a player has a position, direction, colour and moves itself ----
struct player
{ int x,y,dir;
  Color color;
  player(Color c)
  {
    x=rand() % W;
    y=rand() % H;
    color=c;
    dir=rand() % 4;
  }
  // ---- UPDATE: move one pixel and wrap round the edges ----
  void tick()
  {
    // HINT: what do the dir values 0-3 mean? Compare with the steering keys in tron().
    if (dir==0) y+=1;
    if (dir==1) x-=1;
    if (dir==2) x+=1;
    if (dir==3) y-=1;

    if (x>=W) x=0;  if (x<0) x=W-1;
    if (y>=H) y=0;  if (y<0) y=H-1;
  }

  // ---- HELPER: colour as a Vector3f ----
  Vector3f getColor()
  {return Vector3f(color.r,color.g,color.b);}
};

int tron()
{
    // ---- SETUP: window, background, players, off-screen texture ----
    srand(time(0));

    RenderWindow window(VideoMode({W, H}), "The Tron Game!");
    window.setFramerateLimit(60);

    Texture texture("images/tron/background.jpg");
    Sprite sBackground(texture);

    player p1(Color::Red), p2(Color::Green); 

    RenderTexture t({W, H});
    t.setSmooth(true);
    Sprite sprite(t.getTexture());
    t.clear();  t.draw(sBackground);

    bool Game=1;

    while (window.isOpen())
    {
        // ---- INPUT: window events ----
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();
        }

        // ---- INPUT: steering keys for both players ----
        // HINT: the two blocks of key checks are near-duplicates - what differs? A small struct of keys per player might help.
        if (Keyboard::isKeyPressed(Keyboard::Key::Left)) if (p1.dir!=2) p1.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Key::Right)) if (p1.dir!=1)  p1.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Key::Up)) if (p1.dir!=0) p1.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Key::Down)) if (p1.dir!=3) p1.dir=0;

        if (Keyboard::isKeyPressed(Keyboard::Key::A)) if (p2.dir!=2) p2.dir=1;
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) if (p2.dir!=1)  p2.dir=2;
        if (Keyboard::isKeyPressed(Keyboard::Key::W)) if (p2.dir!=0) p2.dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Key::S)) if (p2.dir!=3) p2.dir=0;

        // ---- UPDATE: move both players and check the trails ----
        if (!Game)    continue;

        // HINT: why loop 'speed' times per frame? Put a breakpoint inside and watch p1.x, p1.y and Game.
        for(int i=0;i<speed;i++)
        {
            p1.tick(); p2.tick();
            // HINT: field[][] is the real memory of the trails, the RenderTexture is only the picture. Could the rules live in an SFML-free class?
            if (field[p1.x][p1.y]==1) Game=0; 
            if (field[p2.x][p2.y]==1) Game=0;
            field[p1.x][p1.y]=1; 
            field[p2.x][p2.y]=1;
    
            CircleShape c(3);
            c.setPosition({float(p1.x),float(p1.y)}); c.setFillColor(p1.color);    t.draw(c);
            c.setPosition({float(p2.x),float(p2.y)}); c.setFillColor(p2.color);    t.draw(c);
            t.display();    
        }

       ////// draw  ///////
        // ---- DRAW ----
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
