#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// ===== GUIDE: Tetris =====
// Entry point: int tetris(). Almost everything happens inside its while (window.isOpen()) loop.
// State that survives between frames: globals field, a, b (below), plus dx, rotate, colorNum, timer, delay (declared before the loop).
// Suggested reading order: field and figures -> check() -> setup in tetris() -> the loop, top to bottom.
// =====
const int M = 20;
const int N = 10;

// ---- SHARED STATE: field = landed blocks, a[] = falling piece, b[] = its previous position ----
static int field[M][N] = {0};

struct Point
{int x,y;} a[4], b[4];

// ---- DATA: the seven piece shapes ----
// HINT: each number is a cell index in a 2-column grid - look at how the numbers are decoded when a new piece spawns.
int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

// ---- HELPER: check() asks whether the falling piece a[] is in a legal place ----
bool check()
{
   for (int i=0;i<4;i++)
      if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;
      else if (field[a[i].y][a[i].x]) return 0;

   return 1;
};


int tetris()
{
    // ---- SETUP: window, textures, sprites ----
    srand(time(0));     

    RenderWindow window(VideoMode({320, 480}), "The Game!");

    Texture t1("images/tetris/tiles.png");
    Texture t2("images/tetris/background.png");
    Texture t3("images/tetris/frame.png");

    Sprite s(t1), background(t2), frame(t3);

    // ---- GAME STATE: lives across frames, changed by input and the timer ----
    int dx=0; bool rotate=0; int colorNum=1;
    float timer=0,delay=0.3; 

    Clock clock;

    while (window.isOpen())
    {
        // ---- TIMING: how long since the last frame ----
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        // ---- INPUT: events set the dx / rotate flags ----
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();

            if (const auto* key = e->getIf<Event::KeyPressed>())
              if (key->code==Keyboard::Key::Up) rotate=true;
              else if (key->code==Keyboard::Key::Left) dx=-1;
              else if (key->code==Keyboard::Key::Right) dx=1;
        }

    if (Keyboard::isKeyPressed(Keyboard::Key::Down)) delay=0.05;

    // ---- UPDATE: move sideways ----
    //// <- Move -> ///
    for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
    if (!check()) for (int i=0;i<4;i++) a[i]=b[i];

    // ---- UPDATE: rotate ----
    //////Rotate//////
    if (rotate)
      {
        // HINT: this rotation uses a[1] as a pivot - try it on paper with a T piece.
        Point p = a[1]; //center of rotation
        for (int i=0;i<4;i++)
          {
            int x = a[i].y-p.y;
            int y = a[i].x-p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
           }
           if (!check()) for (int i=0;i<4;i++) a[i]=b[i];
      }

    // ---- UPDATE: gravity tick - drop, lock in place, spawn next piece ----
    ///////Tick//////
    if (timer>delay)
      {
        for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }

        if (!check())
        {
         for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;

         colorNum=1+rand()%7;
         int n=rand()%7;
         for (int i=0;i<4;i++)
           {
            a[i].x = figures[n][i] % 2;
            a[i].y = figures[n][i] / 2;
           }
        }

         timer=0;
      }

    // ---- UPDATE: clear completed lines ----
    ///////check lines//////////
    // HINT: put a breakpoint here and watch k and i - why do they sometimes differ?
    int k=M-1;
    for (int i=M-1;i>0;i--)
    {
        int count=0;
        for (int j=0;j<N;j++)
        {
            if (field[i][j]) count++;
            field[k][j]=field[i][j];
        }
        if (count<N) k--;
    }

    // ---- RESET: per-frame inputs go back to defaults ----
    dx=0; rotate=0; delay=0.3;

    // ---- DRAW ----
    /////////draw//////////
    window.clear(Color::White);    
    window.draw(background);
          
    for (int i=0;i<M;i++)
     for (int j=0;j<N;j++)
       {
         if (field[i][j]==0) continue;
         // HINT: 18 and the offset (28, 31) are pixel sizes - good candidates for named constants.
         s.setTextureRect(IntRect({field[i][j]*18,0},{18,18}));
         s.setPosition({float(j*18),float(i*18)});
         s.move({28,31}); //offset
         window.draw(s);
       }

    for (int i=0;i<4;i++)
      {
        s.setTextureRect(IntRect({colorNum*18,0},{18,18}));
        s.setPosition({float(a[i].x*18),float(a[i].y*18)});
        s.move({28,31}); //offset
        window.draw(s);
      }

    window.draw(frame);
    window.display();
    }

    return 0;
}
