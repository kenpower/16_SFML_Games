#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>
#include <optional>
#include "Connector.hpp"
using namespace sf;
// ===== GUIDE: Chess =====
// Entry point: int chess(), at the bottom. It uses Connector.hpp to ask the Stockfish engine for the computer's move.
// State that survives between frames: the globals f (32 pieces), position (the move history string) and board, plus isMove, dx, dy, oldPos, newPos, str and n.
// Suggested reading order: toChessNote and toCoord, then move, then loadPosition, then chess() from SETUP downwards.
// =====

int size = 56;
Vector2f os(28,28);

// ---- DATA: a position on the board in pixels (top-left corner of a square) ----
struct BoardPos
{ float x,y; };

bool operator==(BoardPos a, BoardPos b) {return a.x==b.x && a.y==b.y;}
bool operator!=(BoardPos a, BoardPos b) {return !(a==b);}

// ---- DATA: one piece - where it is, and which picture (column, row) in figures.png it uses ----
struct Figure
{ BoardPos pos; int imageX, imageY; };

static Figure f[32]; //figures
std::string position="";

// ---- DATA: starting layout - numbers encode piece type and colour, 0 is an empty square ----
int board[8][8] = 
    {-1,-2,-3,-4,-5,-3,-2,-1,
     -6,-6,-6,-6,-6,-6,-6,-6,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0,
      6, 6, 6, 6, 6, 6, 6, 6,
      1, 2, 3, 4, 5, 3, 2, 1};

// ---- HELPERS: convert between pixel positions and chess notation such as "e2" ----
std::string toChessNote(BoardPos p)
{
  std::string s = "";
  // HINT: the 97 and 49 are ASCII codes - which characters are they, and why is y flipped with 7-y?
  s += char(p.x/size+97);
  s += char(7-p.y/size+49);
  return s;
}

BoardPos toCoord(char a,char b)
{
   int x = int(a) - 97;
   int y = 7-int(b)+49;
   return BoardPos{float(x*size),float(y*size)};
}

// ---- HELPER: applies one move string such as "e2e4" to the pieces ----
void move(std::string str)
{
    BoardPos oldPos = toCoord(str[0],str[1]);
    BoardPos newPos = toCoord(str[2],str[3]);

    for(int i=0;i<32;i++)
     if (f[i].pos==newPos) f[i].pos={-100,-100};
        
    for(int i=0;i<32;i++)
     if (f[i].pos==oldPos) f[i].pos=newPos;

    // HINT: how does the code know the king has not moved before? Check what `position` holds.
    //castling       //if the king didn't move
    if (str=="e1g1") if (position.find("e1")==-1) move("h1f1"); 
    if (str=="e8g8") if (position.find("e8")==-1) move("h8f8");
    if (str=="e1c1") if (position.find("e1")==-1) move("a1d1");
    if (str=="e8c8") if (position.find("e8")==-1) move("a8d8");
}

// ---- HELPER: resets the pieces to the start layout, then replays the move history ----
void loadPosition()
{
    int k=0;
    for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
     {
       int n = board[i][j];
       if (!n) continue;
       int x = abs(n)-1;
       int y = n>0?1:0;
       f[k].imageX = x;
       f[k].imageY = y;
       f[k].pos = {float(size*j),float(size*i)};
       k++;
     }

    for(int i=0;i<position.length();i+=5)
      move(position.substr(i,4));
}

// ---- DRAW helper: draws one piece using the shared sprite ----
void drawFigure(RenderWindow &window, Sprite &s, const Figure &fig)
{
    s.setTextureRect( IntRect({size*fig.imageX,size*fig.imageY},{size,size}) );
    s.setPosition(Vector2f(fig.pos.x,fig.pos.y) + os);
    window.draw(s);
}


int chess()
{
    // ---- SETUP: window and engine connection ----
    RenderWindow window(VideoMode({504, 504}), "The Chess! (press SPACE)");
    
#if defined(_WIN32)
    ConnectToEngine("stockfish.exe");             // bundled next to the executable
#elif defined(SFML_GAMES_STOCKFISH_PATH)
    ConnectToEngine(SFML_GAMES_STOCKFISH_PATH);   // located by CMake at configure time
#else
    ConnectToEngine("stockfish");                 // fall back to searching PATH at runtime
#endif

    Texture t1("images/chess/figures.png");
    Texture t2("images/chess/board.png");

    Sprite sFigure(t1), sBoard(t2); 

    loadPosition();

    // ---- SETUP: variables for drag and drop, kept across frames ----
    bool isMove=false;
    float dx=0, dy=0;
    BoardPos oldPos{0,0},newPos{0,0};
    std::string str;
    int n=0; 

    while (window.isOpen())
    {
        // ---- INPUT: mouse position and window events ----
        Vector2i pos = Mouse::getPosition(window) - Vector2i(os);

        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();

            ////move back//////
            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Backspace)
                { if (position.length()>6) position.erase(position.length()-6,5); loadPosition();}

            /////drag and drop///////
            if (const auto* mb = e->getIf<Event::MouseButtonPressed>())
                if (mb->button == Mouse::Button::Left)
                  for(int i=0;i<32;i++)
                  if (FloatRect({f[i].pos.x,f[i].pos.y},{float(size),float(size)}).contains(Vector2f(pos)))
                      {
                       isMove=true; n=i;
                       dx=pos.x - f[i].pos.x;
                       dy=pos.y - f[i].pos.y;
                       oldPos  =  f[i].pos;
                      }

             if (const auto* mr = e->getIf<Event::MouseButtonReleased>())
                if (mr->button == Mouse::Button::Left)
                 {
                  isMove=false;
                  // HINT: put a breakpoint here, drop a piece and watch str and position - what text gets built up?
                  BoardPos p = {f[n].pos.x + size/2, f[n].pos.y + size/2};
                  newPos = BoardPos{ float(size*int(p.x/size)), float(size*int(p.y/size)) };
                  str = toChessNote(oldPos)+toChessNote(newPos);
                  move(str); 
                  if (oldPos!=newPos) position+=str+" ";
                  f[n].pos = newPos;                   
                 }                       
        }

       // ---- UPDATE: computer move while SPACE is held ----
       //comp move
       if (Keyboard::isKeyPressed(Keyboard::Key::Space))
       {
         str =  getNextMove(position);
                   
         oldPos = toCoord(str[0],str[1]);
         newPos = toCoord(str[2],str[3]);
         
         for(int i=0;i<32;i++) if (f[i].pos==oldPos) n=i;
         
         /////animation///////
          // HINT: this draw code is very like the draw code at the bottom of the loop - could it be one function?
         for(int k=0;k<50;k++)
          {
            BoardPos p = {newPos.x - oldPos.x, newPos.y - oldPos.y};
            f[n].pos.x += p.x/50; f[n].pos.y += p.y/50; 
            window.draw(sBoard);
            for(int i=0;i<32;i++) drawFigure(window, sFigure, f[i]); drawFigure(window, sFigure, f[n]);
            window.display();
          }

        move(str);  position+=str+" ";
        f[n].pos = newPos; 
        }

        // ---- UPDATE: a dragged piece follows the mouse ----
        if (isMove) f[n].pos = {pos.x-dx,pos.y-dy};

    // ---- DRAW ----
    ////// draw  ///////
    window.clear();
    window.draw(sBoard);
    for(int i=0;i<32;i++) drawFigure(window, sFigure, f[i]); drawFigure(window, sFigure, f[n]);
    window.display();
    }

    CloseConnection();
    return 0;
}
