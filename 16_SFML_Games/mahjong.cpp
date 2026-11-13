#include <SFML/Graphics.hpp>
#include <fstream>
#include <time.h>
#include <vector>
using namespace sf;
// ===== GUIDE: Mahjong Solitaire =====
// Entry point: int mahjong(), near the bottom. The board is the global 3D array `field`, accessed through f(x,y,z).
// State that survives between frames: field, plus v1, v2, moves and the layout numbers declared before the loop.
// Suggested reading order: f and isOpen, then SETUP in mahjong(), then the INPUT section, then DRAW.
// =====

// ---- DATA: the board - one int per (x, y, layer) position ----
static int field[50][50][50] = {0};

// ---- DATA: one board position (x, y, layer) ----
struct TilePos
{
  int x,y,z;
  TilePos() : x(0), y(0), z(0) {}
  TilePos(int x,int y,int z) : x(x), y(y), z(z) {}
};

bool operator==(TilePos a, TilePos b) {return a.x==b.x && a.y==b.y && a.z==b.z;}

// ---- HELPERS: f(...) reads or writes one board cell ----
int& f(int x,int y,int z){return field[y+2][x+2][z];}
int& f(TilePos v){return f(v.x,v.y,v.z);}


// ---- RULE: is this tile free to be picked? ----
bool isOpen(int x,int y,int z)
{
  // HINT: what is this loop checking? Draw tiles at x-2, x and x+2 on paper and see which ones block.
  for(int i=-1;i<=1;i++)
   for(int j=-1;j<=1;j++)
    if (f(x+2,y+i,z)>0 && f(x-2,y+j,z)>0) return 0;

  for(int i=-1;i<=1;i++)
   for(int j=-1;j<=1;j++)
    if ( f(x+i,y+j,z+1)>0 ) return 0;

  return 1;
}


int mahjong()
{
    srand(time(0));

    // ---- SETUP: window, images and layout numbers ----
    RenderWindow app(VideoMode({740, 570}), "Mahjong Solitaire!");

    Texture t1("images/mahjong/tiles.png");
    Texture t2("images/mahjong/background.png");
    Sprite s(t1), sBackground(t2);
    int w=48, h=66;
    int stepX=w/2-2, stepY=h/2-2;
    float offX=4.6, offY=7.1; // z offset
    TilePos v1,v2;
    std::vector<TilePos> moves;


    // ---- SETUP: read the layout from map.txt ----
    ////load from file////
    std::fstream myfile("images/mahjong/map.txt");
    for(int y=0;y<18;y++)
     for(int x=0;x<30;x++)
      {
        char a;  myfile >> a;
        int n = a - '0';
        for(int z=0;z<n;z++)
          if (f(x-1,y-1,z)) f(x-1,y,z)=f(x,y-1,z)=0;
          else f(x,y,z)=1;
      }

    // ---- SETUP: deal out tile faces in matching pairs ----
    ////create map//////
    for(int k=1;;k++)
    {
     std::vector<TilePos> opens;
     for(int z=0;z<10;z++)
      for(int y=0;y<18;y++)
       for(int x=0;x<30;x++)
        if (f(x,y,z)>0 && isOpen(x,y,z)) opens.push_back(TilePos(x,y,z));

     // HINT: breakpoint here and watch k, n and opens.size() - how many passes does it take, and why does k change as it does?
     int n=opens.size();
     if (n<2) break;
     int a=0,b=0;
     while(a==b){a=rand()%n;b=rand()%n;}
     f(opens[a])=-k;  if (k>34) k++;
     f(opens[b])=-k;
     k%=42;
    }

    for(int z=0;z<10;z++)
     for(int y=0;y<18;y++)
      for(int x=0;x<30;x++) f(x,y,z)*=-1;


    while (app.isOpen())
    {
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

            // ---- INPUT: right click undoes the last pair ----
            //move back
            if (const auto* mr = e->getIf<Event::MouseButtonReleased>())
                if (mr->button == Mouse::Button::Right)
                  {
                    int n = moves.size();
                    if (n==0) continue;
                    f(moves[n-1])*=-1; moves.pop_back();
                    f(moves[n-2])*=-1; moves.pop_back();
                  }

              // ---- INPUT: left click picks tiles and removes a matching pair ----
               if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
                if (mb->button == Mouse::Button::Left)
                  for(int z=0;z<10;z++)
                   {
                     // HINT: this is pixel maths turning the mouse into a tile position - the draw code does the reverse, could both be functions?
                     Vector2i pos = Mouse::getPosition(app) - Vector2i(30,0); // 30 - desk offset
                     int x = (pos.x-z*offX)/stepX;
                     int y = (pos.y+z*offY)/stepY;

                     for(int i=0;i<2;i++)
                      for(int j=0;j<2;j++)
                        if (f(x-i,y-j,z)>0 && isOpen(x-i,y-j,z))
                           v1=TilePos(x-i,y-j,z);

                     if (v1==v2) continue;

                     int a=f(v1),b=f(v2);
                    // HINT: which tile numbers can match each other? Compare these ranges with the faces in images/mahjong/tiles.png.
                     if ( a==b || (a>34 && a<39 && b>34 && b<39) || (a>=39 && b>=39) )
                        {
                         f(v1)*=-1; moves.push_back(v1);
                         f(v2)*=-1; moves.push_back(v2);
                        }
                     v2=v1;
                   }
        }

       // ---- DRAW: layer by layer, back to front ----
       app.clear();
       app.draw(sBackground);
       for(int z=0;z<10;z++)
        for(int x=30;x>=0;x--)
         for(int y=0;y<18;y++)
         {
            int k = f(x,y,z)-1;
            if (k<0) continue;
            s.setTextureRect(IntRect({k*w,0},{w,h}));
            if (isOpen(x,y,z)) s.setTextureRect(IntRect({k*w,h},{w,h}));
            s.setPosition({x*stepX + z*offX, y*stepY - z*offY});
            s.move({30,0}); //desk offset
            app.draw(s);
          }

        app.display();
    }

    return 0;
}
