#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>

#include "EntityController.h"

using namespace sf;

class Animation
{
   public:
   float Frame, speed;
   Sprite sprite;
   std::vector<IntRect> frames;

   Animation(){}

   Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
   {
     Frame = 0;
     speed = Speed;

     for (int i=0;i<count;i++)
      frames.push_back( IntRect(x+i*w, y, w, h)  );

     sprite.setTexture(t);
     sprite.setOrigin(w/2,h/2);
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

class Entity
{
public:

   Animation anim;
   EntityController* controller;
   

   void settings(Animation &a, EntityController* ecr, int X,int Y,float Angle=0,int radius=1)
   {
     anim = a;
     controller = ecr;
     controller->settings(X, Y, Angle, radius);
   }

   void draw(RenderWindow &app)
   {
     anim.sprite.setPosition(controller->x,controller->y);
     anim.sprite.setRotation(controller->angle+90);
     app.draw(anim.sprite);

     //CircleShape circle(R);
     //circle.setFillColor(Color(255,0,0,170));
     //circle.setPosition(x,y);
     //circle.setOrigin(R,R);
     //app.draw(circle);
   }

   virtual ~Entity(){};
};


int asteroids()
{
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1,t2,t3,t4,t5,t6,t7;
    t1.loadFromFile("images/asteroids/spaceship.png");
    t2.loadFromFile("images/asteroids/background.jpg");
    t3.loadFromFile("images/asteroids/explosions/type_C.png");
    t4.loadFromFile("images/asteroids/rock.png");
    t5.loadFromFile("images/asteroids/fire_blue.png");
    t6.loadFromFile("images/asteroids/rock_small.png");
    t7.loadFromFile("images/asteroids/explosions/type_B.png");

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
      Entity* ae = new Entity;
      ae->settings(sRock,a, rand()%W, rand()%H, rand()%360, 25);
      entities.push_back(ae);
    }

    player *p = new player();
	Entity* pe = new Entity;
    pe->settings(sPlayer, p, W / 2, H / 2, 0, 20);
    entities.push_back(pe);

    /////main loop/////
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
             if (event.key.code == Keyboard::Space)
              {
                bullet *b = new bullet();
				Entity* be = new Entity();
                be->settings(sBullet, b, p->x,p->y,p->angle,10);
                entities.push_back(be);
              }
        }

    if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle+=3;
    if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle-=3;
    if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust=true;
    else p->thrust=false;


    for(auto ae:entities)
     for(auto be:entities)
     {
         auto a = ae->controller;
         auto b = be->controller;

      if (a->name=="asteroid" && b->name=="bullet")
       if ( isCollide(a,b) )
           {
            a->life=false;
            b->life=false;

            Entity *e = new Entity();
            EntityController* c = new EntityController;
            e->settings(sExplosion, c, a->x,a->y);
            c->name="explosion";
            entities.push_back(e);


            for(int i=0;i<2;i++)
            {
             if (a->R==15) continue;
             Entity* e = new Entity();
             EntityController* ac = new asteroid();
             e->settings(sRock_small,ac, a->x,a->y,rand()%360,15);
             entities.push_back(e);
            }

           }

      if (a->name=="player" && b->name=="asteroid")
       if ( isCollide(a,b) )
           {
            b->life=false;

            Entity *e = new Entity();
            EntityController* c = new EntityController;
            e->settings(sExplosion_ship,c, a->x,a->y);
            c->name="explosion";
            entities.push_back(e);

            a->settings(W/2,H/2,0,20);
            a->dx=0; a->dy=0;
           }
     }


    if (p->thrust)  pe->anim = sPlayer_go;
    else   pe->anim = sPlayer;


    for(auto e:entities)
     if (e->controller->name=="explosion")
      if (e->anim.isEnd()) e->controller->life=0;

    if (rand()%150==0)
     {
       asteroid *a = new asteroid();
       Entity* e = new Entity();
       e->settings(sRock, a, 0,rand()%H, rand()%360, 25);
       entities.push_back(e);
     }

    for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->controller->update();
      e->anim.update();

      if (e->controller->life==false) {i=entities.erase(i); delete e;}
      else i++;
    }

   //////draw//////
   app.draw(background);
   for(auto i:entities) i->draw(app);
   app.display();
    }

    return 0;
}
