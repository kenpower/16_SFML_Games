#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <string>
#include <vector>
using namespace sf;
// ===== GUIDE: Outrun Racing =====
// Entry point: int outrun(), near the bottom. The globals at the top (width, height, roadW, segL, camD) are shared settings.
// State that survives between frames: the `lines` vector (the whole track, built once), plus playerX, pos and H declared before the loop.
// Suggested reading order: Line::project, then the SETUP in outrun(), then the road loop, then drawQuad and Line::drawSprite.
// =====

int width = 1024;
int height = 768;
int roadW = 2000;
int segL = 200; //segment length
float camD = 0.84; //camera depth

// ---- HELPER: draws one four-sided strip (grass, rumble or road) on screen ----
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

// ---- DATA: one slice of the track - 3D position, screen position, curve and optional sprite ----
struct Line
{
  float x,y,z; //3d center of line
  float X,Y,W; //screen coord
  float curve,spriteX,clip,scale;
  std::optional<Sprite> sprite; // empty when the line has no roadside object

  Line()
  {spriteX=curve=x=y=z=0;}

  // ---- Line::project: turns the 3D world position into screen coordinates ----
  void project(int camX,int camY,int camZ)
  {
    scale = camD/(z-camZ);
    X = (1 + scale*(x - camX)) * width/2;
    Y = (1 - scale*(y - camY)) * height/2;
    W = scale * roadW  * width/2;
  }

  // ---- Line::drawSprite: draws the roadside object (if any), scaled and clipped ----
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

    // HINT: put a breakpoint here and watch clip and destY+destH - when does the sprite get cut off or skipped?
    float clipH = destY+destH-clip;
    if (clipH<0) clipH=0;

    if (clipH>=destH) return;
    s.setTextureRect(IntRect({0,0},{w,int(h-h*clipH/destH)}));
    s.setScale({destW/w,destH/h});
    s.setPosition({destX, destY});
    app.draw(s);
    }
};


int outrun()
{
    // ---- SETUP: window and images ----
    RenderWindow app(VideoMode({unsigned(width), unsigned(height)}), "Outrun Racing!");
    app.setFramerateLimit(60);

    Texture t[8];                 // t[0] is unused
    std::vector<Sprite> object;   // Sprite has no default constructor, so fill it in order
    object.emplace_back(t[0]);    // placeholder for index 0
    for(int i=1;i<=7;i++)
     {
       t[i] = Texture("images/outrun/"+std::to_string(i)+".png");
       t[i].setSmooth(true);
       object.emplace_back(t[i]);
     }

    Texture bg("images/outrun/bg.png");
    bg.setRepeated(true);
    Sprite sBackground(bg);
    sBackground.setTextureRect(IntRect({0,0},{5000,411}));
    sBackground.setPosition({-2000,0});

    // ---- SETUP: build the track, one Line per segment ----
    std::vector<Line> lines;

    for(int i=0;i<1600;i++)
     {
       Line line;
       line.z = i*segL;

       // HINT: these index ranges and curve values describe the track layout - could they be data instead of if statements?
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
   float playerX = 0;
   int pos = 0;
   int H = 1500;

    while (app.isOpen())
    {
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();
        }

  // ---- INPUT: keys change playerX, speed and camera height ----
  int speed=0;

  if (Keyboard::isKeyPressed(Keyboard::Key::Right)) playerX+=0.1;
  if (Keyboard::isKeyPressed(Keyboard::Key::Left)) playerX-=0.1;
  if (Keyboard::isKeyPressed(Keyboard::Key::Up)) speed=200;
  if (Keyboard::isKeyPressed(Keyboard::Key::Down)) speed=-200;
  if (Keyboard::isKeyPressed(Keyboard::Key::Tab)) speed*=3;
  if (Keyboard::isKeyPressed(Keyboard::Key::W)) H+=100;
  if (Keyboard::isKeyPressed(Keyboard::Key::S)) H-=100;

  // ---- UPDATE: move along the track, wrapping at the ends ----
  pos+=speed;
  while (pos >= N*segL) pos-=N*segL;
  while (pos < 0) pos += N*segL;

  // ---- DRAW: background first, then the road ----
  app.clear(Color(105,205,4));
  app.draw(sBackground);
  int startPos = pos/segL;
  int camH = lines[startPos].y + H;
  if (speed>0) sBackground.move({-lines[startPos].curve*2,0});
  if (speed<0) sBackground.move({lines[startPos].curve*2,0});

  int maxy = height;
  float x=0,dx=0;

  ///////draw road////////
  for(int n = startPos; n<startPos+300; n++)  
   {
    Line &l = lines[n%N];
    // HINT: put a breakpoint in this loop and watch x, dx and l.curve change per iteration - how do bends appear on screen?
    l.project(playerX*roadW-x, camH, startPos*segL - (n>=N?N*segL:0));
    x+=dx;
    dx+=l.curve;

    l.clip=maxy;
    if (l.Y>=maxy) continue;
    maxy = l.Y;

    // HINT: three colours chosen by the same test (n/3)%2 - could a small struct or named constants tidy this?
    Color grass  = (n/3)%2?Color(16,200,16):Color(0,154,0);
    Color rumble = (n/3)%2?Color(255,255,255):Color(0,0,0);
    Color road   = (n/3)%2?Color(107,107,107):Color(105,105,105);

    Line p = lines[(n-1)%N]; //previous line

    drawQuad(app, grass, 0, p.Y, width, 0, l.Y, width);
    drawQuad(app, rumble,p.X, p.Y, p.W*1.2, l.X, l.Y, l.W*1.2);
    drawQuad(app, road,  p.X, p.Y, p.W, l.X, l.Y, l.W);
   }

    // ---- DRAW: roadside sprites, far to near ----
    ////////draw objects////////
    for(int n=startPos+300; n>startPos; n--)
      lines[n%N].drawSprite(app);

    app.display();
    }

    return 0;
}
