#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
using namespace sf;

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth

void drawQuad(RenderWindow &w, Color c, int x1,int y1,int w1,int x2,int y2,int w2)
{
    ConvexShape shape(4);
    shape.setFillColor(c);
    shape.setPoint(0, Vector2f(x1-w1,y1));
    shape.setPoint(1, Vector2f(x2-w2,y2));
    shape.setPoint(2, Vector2f(x2+w2,y2));
    shape.setPoint(3, Vector2f(x1+w1,y1));
    w.draw(shape);
}

struct Line
{
  float x,y,z; //3d center of line
  float X,Y,W; //screen coord
  float curve,spriteX,clip,scale;
  std::optional<Sprite> sprite;

  Line()
  {spriteX=curve=x=y=z=0;}

  void project(int camX,int camY,int camZ)
  {
    scale = camD/(z-camZ);
    X = (1 + scale*(x - camX)) * width/2;
    Y = (1 - scale*(y - camY)) * height/2;
    W = scale * roadW  * width/2;
  }

  void drawSprite(RenderWindow &app)
  {
    if (!sprite) return;
    Sprite s = *sprite;
    int w = s.getTextureRect().size.x;
    int h = s.getTextureRect().size.y;

    float destX = X + scale * spriteX * width/2;
    float destY = Y + 4;
    float destW  = w * W / 266;
    float destH  = h * W / 266;

    destX += destW * spriteX; //offsetX
    destY += destH * (-1);    //offsetY

    float clipH = destY+destH-clip;
    if (clipH<0) clipH=0;

    if (clipH>=destH) return;
    s.setTextureRect(IntRect({0, 0}, {w, static_cast<int>(h-h*clipH/destH)}));
    s.setScale({destW/w, destH/h});
    s.setPosition({destX, destY});
    app.draw(s);
    }
};


int outrun_messy()
{
    RenderWindow app(VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}), "Outrun Racing!");
    app.setFramerateLimit(60);

    Texture t[50];
    std::optional<Sprite> object[50];
    for(int i=1;i<=7;i++)
     {
       t[i].loadFromFile("images/outrun/"+std::to_string(i)+".png");
       t[i].setSmooth(true);
       object[i].emplace(t[i]);
     }

    Texture bg;
    bg.loadFromFile("images/outrun/bg.png");
    bg.setRepeated(true);
    Sprite sBackground(bg);
    sBackground.setTextureRect(IntRect({0, 0}, {5000, 411}));
    sBackground.setPosition({-2000, 0});

    // Player car
    Texture tCar;
    tCar.loadFromFile("images/outrun/car.png");
    Sprite sCar(tCar);
    sCar.setOrigin({22, 122}); // Center the car sprite
    sCar.setColor(Color::White);

    std::vector<Line> lines;

    for(int i=0;i<1600;i++)
     {
       Line line;
       line.z = i*segL;

       if (i>300 && i<700) line.curve=0.5;
       if (i>1100) line.curve=-0.7;

       if (i<300 && i%20==0) {line.spriteX=-2.5; line.sprite=object[5];}
       if (i%17==0)          {line.spriteX=2.0; line.sprite=object[6];}
       if (i>300 && i%20==0) {line.spriteX=-0.7; line.sprite=object[4];}
       if (i>800 && i%20==0) {line.spriteX=-1.2; line.sprite=object[1];}
       if (i==400)           {line.spriteX=-1.2; line.sprite=object[7];}

       if (i>750) line.y = sin(i/30.0)*1500;

       lines.push_back(line);
     }

   int N = lines.size();
   float px = 0;  // player x position
   int pos = 0;
   int H = 1500;

   // Off-road state
   bool offRoad = false;
   float friction = 0.98f;
   float carW = 44.0f;
   int maxSpd1 = 200;
   int maxSpd2 = 120;
   int score = 0;  // not used yet
   float lapTime = 0.0f;  // not used yet

    while (app.isOpen())
    {
        while (const std::optional event = app.pollEvent())
        {
            if (event->is<Event::Closed>())
                app.close();
        }

  static int spd=0;
  float turnRate = 0.0005f;

  // Different max speeds based on terrain
  int maxSpd;
  if (offRoad)
  {
      maxSpd = maxSpd2;
  }
  else
  {
      maxSpd = maxSpd1;
  }

  if (Keyboard::isKeyPressed(Keyboard::Key::Up))
  {
      spd = maxSpd;
  }
  if (Keyboard::isKeyPressed(Keyboard::Key::Down))
  {
      spd = -maxSpd;
  }
  if (Keyboard::isKeyPressed(Keyboard::Key::Tab))
  {
      spd = spd * 3;
  }
  if (Keyboard::isKeyPressed(Keyboard::Key::W))
  {
      H = H + 100;
  }
  if (Keyboard::isKeyPressed(Keyboard::Key::S))
  {
      H = H - 100;
  }

  // Turn speed proportional to forward speed
  float ts = turnRate * abs(spd);
  if (Keyboard::isKeyPressed(Keyboard::Key::Right))
  {
      px = px + ts;
  }
  if (Keyboard::isKeyPressed(Keyboard::Key::Left))
  {
      px = px - ts;
  }

  // Apply road curve to player position
  int sp = pos/segL;
  if (spd != 0)
  {
      float curveDrift = lines[sp % N].curve * 0.02f;
      if (spd > 0)
      {
          px = px - curveDrift;
      }
      else
      {
          px = px + curveDrift;
      }
  }

  // Apply off-road friction
  if (offRoad)
  {
      int absSpeed = abs(spd);
      if (absSpeed > 5)
      {
          spd = spd * friction;
      }
  }

  pos = pos + spd;
  while (pos >= N*segL)
  {
      pos = pos - N*segL;
  }
  while (pos < 0)
  {
      pos = pos + N*segL;
  }

  app.clear(Color(105,205,4));
  int startPos = pos/segL;
  int camH = lines[startPos].y + H;

  // Move background
  float bgX = -2000 - px * 200;
  sBackground.setPosition({bgX, 0});
  app.draw(sBackground);

  // Detect off-road
  int cl = (startPos + 2) % N;
  Line testLine = lines[cl];
  testLine.project(px*roadW, camH, startPos*segL);

  float rcx = testLine.X;
  float rw = testLine.W;
  float rl = rcx - rw;
  float rr = rcx + rw;

  float csx = width / 2.0f;
  float cl1 = csx - carW / 2;
  float cr1 = csx + carW / 2;

  // Check if outside road
  bool outsideLeft = cl1 < rl;
  bool outsideRight = cr1 > rr;
  if (outsideLeft || outsideRight)
  {
      offRoad = true;
  }
  else
  {
      offRoad = false;
  }

  std::cout << "Road: [" << rl << " - " << rr << "], Car: [" << cl1 << " - " << cr1 << "], Off: " << offRoad << "\n";

  int maxy = height;

  ///////draw road////////
  for(int n = startPos; n<startPos+300; n++)
   {
    Line &l = lines[n%N];
    int zOffset;
    if (n>=N)
    {
        zOffset = N*segL;
    }
    else
    {
        zOffset = 0;
    }
    l.project(px*roadW, camH, startPos*segL - zOffset);

    l.clip=maxy;
    if (l.Y>=maxy) continue;
    maxy = l.Y;

    Color grass;
    Color rumble;
    Color road;

    int stripeCheck = (n/3)%2;
    if (stripeCheck)
    {
        grass = Color(16,200,16);
        rumble = Color(255,255,255);
        road = Color(107,107,107);
    }
    else
    {
        grass = Color(0,154,0);
        rumble = Color(0,0,0);
        road = Color(105,105,105);
    }

    Line p = lines[(n-1)%N];

    drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
    drawQuad(app, rumble,p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
    drawQuad(app, road,  p.X, p.Y, p.W, l.X, l.Y, l.W);
   }

    ////////draw objects////////
    for(int n=startPos+300; n>startPos; n--)
    {
      lines[n%N].drawSprite(app);
    }

    ////////draw player car////////
    float carY = height - 100;
    sCar.setPosition({csx, carY});

    // Visual feedback
    if (offRoad)
    {
        int r = 255;
        int g = 150;
        int b = 150;
        sCar.setColor(Color(r, g, b));
    }
    else
    {
        sCar.setColor(Color::White);
    }

    std::cout << "Offroad " << offRoad << "\n";
    app.draw(sCar);

    app.display();
    }

    return 0;
}
