#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

// ===== GUIDE: Snake =====
// Entry point is int snake() at the bottom. State that survives between frames: the globals (N, M, sz, dir, num, s[], f)
// and the locals declared before the loop (clock, timer, delay). Suggested reading order: Tick() first, then SETUP in snake(),
// then the main loop: INPUT, UPDATE, DRAW.
// =====
// ---- GLOBAL STATE: grid size in cells, cell size in pixels, direction, snake length ----
int N=30,M=20;
int sz=16;
int w = sz*N;
int h = sz*M;

int dir,num=4;

// ---- DATA: one cell of the snake body (s[0] is the head) ----
struct Snake 
{ int x,y;}  s[100];

// ---- DATA: the fruit position ----
static struct Fruit
{ int x,y;} f;

// ---- UPDATE: one game step (move body, eat fruit, wrap edges, self-collision) ----
// HINT: Tick() mixes game rules with global data and has no SFML in it - could a Snake class own s[], num and dir?
void Tick()
 {
    // HINT: why does this loop run from the tail towards the head? Try it on paper with 3 segments.
    for (int i=num;i>0;--i)
     {s[i].x=s[i-1].x; s[i].y=s[i-1].y;}

    // HINT: what do the dir values 0-3 mean? Compare with the key handling in snake() - would names help?
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
    // ---- SETUP: window, textures, sprites, timing ----
    srand(time(0));

    RenderWindow window(VideoMode({unsigned(w), unsigned(h)}), "Snake Game!");

    Texture t1("images/snake/white.png");
    Texture t2("images/snake/red.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);

    Clock clock;
    float timer=0, delay=0.1;

    f.x=10;
    f.y=10; 
    
    while (window.isOpen())
    {
        // ---- TIMING: accumulate elapsed time since the last frame ----
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time; 

        // ---- INPUT: window events ----
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();
        }

        // ---- INPUT: arrow keys choose the direction ----
        if (Keyboard::isKeyPressed(Keyboard::Key::Left)) dir=1;   
        if (Keyboard::isKeyPressed(Keyboard::Key::Right)) dir=2;    
        if (Keyboard::isKeyPressed(Keyboard::Key::Up)) dir=3;
        if (Keyboard::isKeyPressed(Keyboard::Key::Down)) dir=0;

        // ---- UPDATE: only step the game once every 'delay' seconds ----
        // HINT: put a breakpoint in Tick() and watch timer here - how many frames pass between steps?
        if (timer>delay) {timer=0; Tick();}

   ////// draw  ///////
    // ---- DRAW: background grid, snake, fruit ----
    window.clear();

    for (int i=0; i<N; i++) 
      for (int j=0; j<M; j++) 
        { sprite1.setPosition({float(i*sz), float(j*sz)});  window.draw(sprite1); }

    for (int i=0;i<num;i++)
        { sprite2.setPosition({float(s[i].x*sz), float(s[i].y*sz)});  window.draw(sprite2); }
   
    sprite2.setPosition({float(f.x*sz), float(f.y*sz)});  window.draw(sprite2);    

    window.display();
    }

    return 0;
}
