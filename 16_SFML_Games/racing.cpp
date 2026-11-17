#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
using namespace sf;

// ===== GUIDE: Racing =====
// Entry point: int racing(). Globals: num and points[] (checkpoints), and the Car struct above the entry function.
// State that survives between frames: car[], the player's speed and angle, the tuning values, offsetX / offsetY.
// Suggested reading order: points -> Car -> racing() setup -> player controls -> AI cars -> collision -> camera and draw.
// =====
// ---- DATA: checkpoints the AI cars drive between ----
const int num=8; //checkpoints
int points[num][2] = {300, 610,
                      1270,430,
                      1380,2380,
                      1900,2460,
                      1970,1700,
                      2550,1680,
                      2560,3150,
                      500, 3300};

// ---- STRUCT: Car holds position, speed, angle; move() drives, findTarget() steers the AI ----
struct Car
{
  float x,y,speed,angle; int n;

  Car() {speed=2; angle=0; n=0;}

  void move()
   {
    x += sin(angle) * speed;
    y -= cos(angle) * speed;
   }

  void findTarget()
  {
    float tx=points[n][0];
    float ty=points[n][1];
    // HINT: this decides whether to turn left or right - what do atan2 and sin(beta) tell you? Try printing beta.
    float beta = angle-atan2(tx-x,-ty+y);
    if (sin(beta)<0) angle+=0.005*speed; else angle-=0.005*speed;
    if ((x-tx)*(x-tx)+(y-ty)*(y-ty)<25*25) n=(n+1)%num;
   }
};


int racing()
{
    // ---- SETUP: window, textures, sprites ----
    RenderWindow app(VideoMode({640, 480}), "Car Racing Game!");
    app.setFramerateLimit(60);

    Texture t1("images/racing/background.png");
    Texture t2("images/racing/car.png");
    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite sBackground(t1), sCar(t2);
    sBackground.scale({2,2});

    sCar.setOrigin({22, 22});
    float R=22;

    // ---- SETUP: create the cars ----
    const int N=5;
    Car car[N];
    for(int i=0;i<N;i++)
    {
      car[i].x=300+i*50;
      car[i].y=1700+i*80;
      car[i].speed=7+i;
    }

   // ---- GAME STATE: the player's speed, angle and tuning values ----
   float speed=0,angle=0;
   float maxSpeed=12.0;
   float acc=0.2, dec=0.3;
   float turnSpeed=0.08;

   int offsetX=0,offsetY=0;

    while (app.isOpen())
    {
        // ---- INPUT: window events ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();
        }

    // ---- INPUT: keyboard ----
    bool Up=0,Right=0,Down=0,Left=0;
    if (Keyboard::isKeyPressed(Keyboard::Key::Up)) Up=1;
    if (Keyboard::isKeyPressed(Keyboard::Key::Right)) Right=1;
    if (Keyboard::isKeyPressed(Keyboard::Key::Down)) Down=1;
    if (Keyboard::isKeyPressed(Keyboard::Key::Left)) Left=1;

    // ---- UPDATE: player speed and steering ----
    //car movement
    if (Up && speed<maxSpeed)
        if (speed < 0)  speed += dec;
        else  speed += acc;

    if (Down && speed>-maxSpeed)
        if (speed > 0) speed -= dec;
        else  speed -= acc;

    if (!Up && !Down)
        if (speed - dec > 0) speed -= dec;
        else if (speed + dec < 0) speed += dec;
        else speed = 0;

    // HINT: put a breakpoint here, hold Up, then release - watch speed and angle as they change.
    if (Right && speed!=0)  angle += turnSpeed * speed/maxSpeed;
    if (Left && speed!=0)   angle -= turnSpeed * speed/maxSpeed;

    // ---- UPDATE: move every car (cars 1 and up steer towards checkpoints) ----
    car[0].speed = speed;
    car[0].angle = angle;

    for(int i=0;i<N;i++) car[i].move();
    for(int i=1;i<N;i++) car[i].findTarget();

    // ---- UPDATE: collision between cars ----
    //collision
    for(int i=0;i<N;i++)
    for(int j=0;j<N;j++)
    {      
        int dx=0, dy=0;
        while (dx*dx+dy*dy<4*R*R)
         {
           car[i].x+=dx/10.0;
           car[i].x+=dy/10.0;
           car[j].x-=dx/10.0;
           car[j].y-=dy/10.0;
           dx = car[i].x-car[j].x;
           dy = car[i].y-car[j].y;
           if (!dx && !dy) break;
         }
    }


    // ---- DRAW ----
    app.clear(Color::White);

    // ---- CAMERA: scroll the background to follow car 0 ----
    // HINT: 320 and 240 are tied to the window size - named constants (or derived values) would help.
    if (car[0].x>320) offsetX = car[0].x-320;
    if (car[0].y>240) offsetY = car[0].y-240;

    sBackground.setPosition({float(-offsetX),float(-offsetY)});
    app.draw(sBackground);

    Color colors[10] = {Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::White};

    for(int i=0;i<N;i++)
    {
      sCar.setPosition({car[i].x-offsetX,car[i].y-offsetY});
      sCar.setRotation(degrees(car[i].angle*180/3.141593f));
      sCar.setColor(colors[i]);
      app.draw(sCar);
    }

    app.display();
    }

    return 0;
}
