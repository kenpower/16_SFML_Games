#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <string>
using namespace sf;
// ===== GUIDE: Netwalk =====
// Entry point: int netwalk(), at the bottom. The board is the global `grid` (N x N pipes) and the helpers above work on it.
// State that survives between frames: grid, plus servPos and the sprites declared before the loop in netwalk().
// Suggested reading order: struct pipe, then generatePuzzle, then drop, then netwalk() from SETUP downwards.
// =====

const int N = 6;
int tsz = 54; //tile size
Vector2f oset(65,55);

// ---- DATA: direction vectors used throughout ----
Vector2i Up(0,-1);
Vector2i Down(0,1);
Vector2i Right(1,0);
Vector2i Left(-1,0);

Vector2i DIR[4] = {Up,Right,Down,Left};

// ---- DATA: one pipe tile - which sides it connects to, plus rotation state ----
struct pipe
{
  std::vector<Vector2i> dirs;
  int orientation;
  float angle; bool on;

  pipe() {angle=0;}

  void rotate()
  {
    for(int i=0;i<dirs.size();i++)
      if (dirs[i]==Up)  dirs[i]=Right;
      else if (dirs[i]==Right) dirs[i]=Down;
      else if (dirs[i]==Down)  dirs[i]=Left;
      else if (dirs[i]==Left)  dirs[i]=Up;
  }

  bool isConnect(Vector2i dir)
  {
    for(auto d: dirs)
     if (d==dir) return true;
    return false;
  }
};


// ---- DATA: the board, with small helpers to access it ----
static pipe grid[N][N];
pipe& cell(Vector2i v) {return grid[v.x][v.y];}
bool isOut(Vector2i v) {return !IntRect({0,0},{N,N}).contains(v);}


// ---- SETUP helper: randomly grows a connected network of pipes ----
void generatePuzzle()
{
  std::vector<Vector2i> nodes;
  nodes.push_back(Vector2i(rand()%N,rand()%N));

  while(!nodes.empty())
  {
    int n = rand()%nodes.size();
    Vector2i v = nodes[n];
    Vector2i d = DIR[rand()%4];

    if (cell(v).dirs.size()==3) {nodes.erase(nodes.begin() + n); continue;}
    if (cell(v).dirs.size()==2) if (rand()%50) continue;

    bool complete=1;
    for(auto D:DIR)
     if (!isOut(v+D) && cell(v+D).dirs.empty()) complete=0;
    if (complete) {nodes.erase(nodes.begin() + n); continue; }

    if (isOut(v+d)) continue;
    if (!cell(v+d).dirs.empty()) continue;
    cell(v).dirs.push_back(d);
    cell(v+d).dirs.push_back(-d);
    nodes.push_back(v+d);
  }
}


// ---- UPDATE helper: marks every cell reachable from the server ----
void drop(Vector2i v)
{
   if (cell(v).on) return;
   cell(v).on=true;

   // HINT: drop() calls itself - trace it on paper for a tiny grid. When does the recursion stop?
   for(auto d:DIR)
    if (!isOut(v+d))
     if (cell(v).isConnect(d) && cell(v+d).isConnect(-d))
       drop(v+d);
}


int netwalk()
{
    // ---- SETUP: window, images and sprites ----
    srand(time(0));

    RenderWindow app(VideoMode({390, 390}), "Netwalk The Pipe Puzzle!");

    Texture t1("images/netwalk/background.png");
    Texture t2("images/netwalk/comp.png");
    Texture t3("images/netwalk/server.png");
    Texture t4("images/netwalk/pipes.png");
    t4.setSmooth(true);

    Sprite sBackground(t1), sComp(t2), sServer(t3), sPipe(t4);
    sPipe.setOrigin({27,27});
    sComp.setOrigin({18,18});
    sServer.setOrigin({20,20});


    // ---- SETUP: build the puzzle, work out each tile's orientation, then shuffle ----
    generatePuzzle();

    for(int i=0;i<N;i++)
     for(int j=0;j<N;j++)
       {
         pipe &p = grid[j][i];

         for(int n=4;n>0;n--) //find orientation//
         {
          std::string s="";
          for(auto d: DIR) s+=p.isConnect(d)? '1':'0';
          if (s=="0011" || s=="0111" || s=="0101" || s=="0010") p.orientation=n;
          p.rotate();
         }

         for(int n=0;n<rand()%4;n++) //shuffle//
          {grid[j][i].orientation++; grid[j][i].rotate();}
       }

    // ---- SETUP: place the server ----
    Vector2i servPos;
    while(cell(servPos).dirs.size()==1) {servPos = Vector2i(rand()%N, rand()%N);}
    sServer.setPosition(Vector2f(servPos*tsz));
    sServer.move(oset);

    while (app.isOpen())
    {
        // ---- INPUT: close the window, or click to rotate a pipe and re-check connections ----
        while (const std::optional e = app.pollEvent())
        {
            if (e->is<Event::Closed>())
                app.close();

            if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
                if (mb->button == Mouse::Button::Left)
                  {
                    // HINT: step through with the debugger - how do mouse pixels become a grid cell? Watch pos before and after the divide.
                    Vector2i pos = Mouse::getPosition(app) + Vector2i(tsz/2,tsz/2) - Vector2i(oset);
                    pos/=tsz;
                    if (isOut(pos)) continue;
                    cell(pos).orientation++;
                    cell(pos).rotate();

                    for(int i=0;i<N;i++)
                     for(int j=0;j<N;j++)
                      grid[j][i].on=0;

                    drop(servPos);
                  }
        }

        // ---- DRAW: background, every pipe (animating its rotation), then the server ----
        app.clear();
        app.draw(sBackground);

        for(int i=0;i<N;i++)
         for(int j=0;j<N;j++)
           {
            pipe &p = grid[j][i];

            // HINT: kind picks which image to use from pipes.png - what do 0, 1, 2 and 3 correspond to?
            int kind = p.dirs.size();
            if (kind==2 && p.dirs[0]==-p.dirs[1]) kind=0;

            p.angle+=5;
            if (p.angle>p.orientation*90) p.angle=p.orientation*90;

            sPipe.setTextureRect(IntRect({tsz*kind,0},{tsz,tsz}));
            sPipe.setRotation(degrees(p.angle));
            sPipe.setPosition({float(j*tsz),float(i*tsz)});sPipe.move(oset);
            app.draw(sPipe);

            if (kind==1)
               // HINT: these numbers are texture coordinates and sizes - good candidates for named constants.
               { if (p.on) sComp.setTextureRect(IntRect({53,0},{36,36}));
                 else sComp.setTextureRect(IntRect({0,0},{36,36}));
                 sComp.setPosition({float(j*tsz),float(i*tsz)});sComp.move(oset);
                 app.draw(sComp);
               }
           }

        app.draw(sServer);
        app.display();
    }

    return 0;
}

