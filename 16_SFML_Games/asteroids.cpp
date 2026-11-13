// __        ___    ____  _   _ ___ _   _  ____
// \ \      / / \  |  _ \| \ | |_ _| \ | |/ ___|
//  \ \ /\ / / _ \ | |_) |  \| || ||  \| | |  _
//   \ V  V / ___ \|  _ <| |\  || || |\  | |_| |
//    \_/\_/_/   \_\_| \_\_| \_|___|_| \_|\____|
//
//  ADVANCED GAME - ATTEMPT WITH CAUTION
//
//  Asteroids is much harder to refactor than most games in this collection.
//  The game logic is spread across a class hierarchy (Entity and its subclasses),
//  entities are created with new and owned through raw pointers in a std::list,
//  and some game state lives inside Animation, which holds an SFML Sprite.
//  Extracting an SFML-free class here needs a redesign, not just a move.
//
//  Only choose this game if you are confident. Otherwise pick a simpler one.
// ===========================================================================

#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <optional>
using namespace sf;

// ===== GUIDE: Asteroids =====
// Entry point is int asteroids() at the bottom. State that survives between frames: the entities list (every rock, bullet,
// explosion and the ship), the player pointer p, and the Animation objects, all declared before the loop.
// Suggested reading order: Animation, Entity and its subclasses, isCollide, then asteroids(): SETUP, INPUT, UPDATE, DRAW.
// =====
const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

// Sprite has no default constructor in SFML 3, so default-constructed Animations
// (e.g. inside Entity) point at this empty texture until settings() assigns a real one.
static const Texture emptyTexture;

// ---- HELPER CLASS: steps through the frames (sub-rectangles) of a sprite sheet ----
class Animation
{
   public:
   float Frame, speed;
   Sprite sprite;
   std::vector<IntRect> frames;

   Animation() : Frame(0), speed(0), sprite(emptyTexture) {}

   Animation (Texture &t, int x, int y, int w, int h, int count, float Speed) : sprite(t)
   {
     Frame = 0;
     speed = Speed;

     for (int i=0;i<count;i++)
      frames.push_back( IntRect({x+i*w, y}, {w, h})  );

     sprite.setOrigin({float(w/2),float(h/2)});
     sprite.setTextureRect(frames[0]);
   }


   void update()
   {
     Frame += speed;
     int n = frames.size();
     if (Frame >= n) Frame -= n;
     if (n>0) sprite.setTextureRect( frames[int(Frame)] );
   }

   bool isEnd()
   {
     return Frame+speed>=frames.size();
   }

};


// ---- BASE CLASS: anything that moves and is drawn (position, velocity, radius, life flag) ----
class Entity
{
   public:
   float x,y,dx,dy,R,angle;
   bool life;
   std::string name;
   Animation anim;

   Entity()
   {
     life=1;
   }

   void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
   {
     anim = a;
     x=X; y=Y;
     angle = Angle;
     R = radius;
   }

   virtual void update(){};

   void draw(RenderWindow &app)
   {
     anim.sprite.setPosition({x,y});
     anim.sprite.setRotation(degrees(angle+90));
     app.draw(anim.sprite);

     CircleShape circle(R);
     circle.setFillColor(Color(255,0,0,170));
     circle.setPosition({x,y});
     circle.setOrigin({R,R});
     //app.draw(circle);
   }

   virtual ~Entity(){};
};


// ---- SUBCLASS: a drifting rock that wraps round the screen ----
class asteroid: public Entity
{
   public:
   asteroid()
   {
     dx=rand()%8-4;
     dy=rand()%8-4;
     name="asteroid";
   }

   void update()
   {
     x+=dx;
     y+=dy;

     if (x>W) x=0;  if (x<0) x=W;
     if (y>H) y=0;  if (y<0) y=H;
   }

};


// ---- SUBCLASS: a bullet, removed when it leaves the screen ----
class bullet: public Entity
{
   public:
   bullet()
   {
     name="bullet";
   }

   void  update()
   {
     // HINT: angle is in degrees but cos/sin want radians - find where the conversion happens and work out which way angle 0 points.
     dx=cos(angle*DEGTORAD)*6;
     dy=sin(angle*DEGTORAD)*6;
     // angle+=rand()%7-3;  /*try this*/
     x+=dx;
     y+=dy;

     if (x>W || x<0 || y>H || y<0) life=0;
   }

};


// ---- SUBCLASS: the player's ship (thrust, drag, speed limit) ----
class player: public Entity
{
   public:
   bool thrust;

   player()
   {
     name="player";
   }

   void update()
   {
     // HINT: 0.2, 0.99, 15 and the other tuning numbers here are good candidates for named constants.
     if (thrust)
      { dx+=cos(angle*DEGTORAD)*0.2;
        dy+=sin(angle*DEGTORAD)*0.2; }
     else
      { dx*=0.99;
        dy*=0.99; }

    int maxSpeed=15;
    float speed = sqrt(dx*dx+dy*dy);
    if (speed>maxSpeed)
     { dx *= maxSpeed/speed;
       dy *= maxSpeed/speed; }

    x+=dx;
    y+=dy;

    if (x>W) x=0; if (x<0) x=W;
    if (y>H) y=0; if (y<0) y=H;
   }

};


// ---- HELPER: circle-versus-circle collision test ----
bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}


int asteroids()
{
    // ---- SETUP: window, textures, animations, starting entities ----
    srand(time(0));

    RenderWindow app(VideoMode({W, H}), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1("images/asteroids/spaceship.png");
    Texture t2("images/asteroids/background.jpg");
    Texture t3("images/asteroids/explosions/type_C.png");
    Texture t4("images/asteroids/rock.png");
    Texture t5("images/asteroids/fire_blue.png");
    Texture t6("images/asteroids/rock_small.png");
    Texture t7("images/asteroids/explosions/type_B.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);

    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);
    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 40,0,40,40, 1, 0);
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);


    std::list<Entity*> entities;

    for(int i=0;i<15;i++)
    {
      asteroid *a = new asteroid();
      a->settings(sRock, rand()%W, rand()%H, rand()%360, 25);
      entities.push_back(a);
    }

    player *p = new player();
    p->settings(sPlayer,200,200,0,20);
    entities.push_back(p);

    /////main loop/////
    while (app.isOpen())
    {
        // ---- INPUT: window events (close, fire a bullet) ----
        while (const std::optional event = app.pollEvent())
        {
            if (event->is<Event::Closed>())
                app.close();

            if (const auto* key = event->getIf<Event::KeyPressed>())
             if (key->code == Keyboard::Key::Space)
              {
                bullet *b = new bullet();
                b->settings(sBullet,p->x,p->y,p->angle,10);
                entities.push_back(b);
              }
        }

    // ---- INPUT: steering and thrust keys ----
    if (Keyboard::isKeyPressed(Keyboard::Key::Right)) p->angle+=3;
    if (Keyboard::isKeyPressed(Keyboard::Key::Left))  p->angle-=3;
    if (Keyboard::isKeyPressed(Keyboard::Key::Up)) p->thrust=true;
    else p->thrust=false;


    // ---- UPDATE: collisions between entities ----
    // HINT: put a breakpoint inside this nested loop - how many pairs get tested each frame, and how many reach isCollide?
    for(auto a:entities)
     for(auto b:entities)
     {
      if (a->name=="asteroid" && b->name=="bullet")
       if ( isCollide(a,b) )
           {
            a->life=false;
            b->life=false;

            Entity *e = new Entity();
            e->settings(sExplosion,a->x,a->y);
            e->name="explosion";
            entities.push_back(e);


            for(int i=0;i<2;i++)
            {
             if (a->R==15) continue;
             Entity *e = new asteroid();
             e->settings(sRock_small,a->x,a->y,rand()%360,15);
             entities.push_back(e);
            }

           }

      if (a->name=="player" && b->name=="asteroid")
       if ( isCollide(a,b) )
           {
            b->life=false;

            Entity *e = new Entity();
            e->settings(sExplosion_ship,a->x,a->y);
            e->name="explosion";
            entities.push_back(e);

            p->settings(sPlayer,W/2,H/2,0,20);
            p->dx=0; p->dy=0;
           }
     }


    // ---- UPDATE: ship image, expiring explosions, spawning new rocks ----
    if (p->thrust)  p->anim = sPlayer_go;
    else   p->anim = sPlayer;


    for(auto e:entities)
     if (e->name=="explosion")
      if (e->anim.isEnd()) e->life=0;

    if (rand()%150==0)
     {
       asteroid *a = new asteroid();
       a->settings(sRock, 0,rand()%H, rand()%360, 25);
       entities.push_back(a);
     }

    // ---- UPDATE: move everything and remove dead entities ----
    // HINT: why is erase() assigned back to i, and why is there no i++ in the for header? Step through it in the debugger.
    for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->update();
      e->anim.update();

      if (e->life==false) {i=entities.erase(i); delete e;}
      else i++;
    }

   //////draw//////
   // ---- DRAW ----
   app.draw(background);
   for(auto i:entities) i->draw(app);
   app.display();
    }

    return 0;
}
