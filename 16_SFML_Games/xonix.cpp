#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int HEIGHT = 25;
const int WIDTH = 40;
const int tileSize = 18; 



class Grid {
    enum tiles { EMPTY = 0, WALL = 1, NEW_WALL = 2, NOT_TO_BE_FILLED = -1 };

    int grid[HEIGHT][WIDTH] = { EMPTY };

public:
    Grid() {
		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j < WIDTH; j++)
				if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1)
					grid[i][j] = WALL;
	}

    int cell(int y, int x) {
		return grid[y][x];
	}

    void drop(int y, int x) {
		if (grid[y][x] == EMPTY)
			grid[y][x] = NOT_TO_BE_FILLED;
		if (grid[y - 1][x] == EMPTY)
			drop(y - 1, x);
		if (grid[y + 1][x] == EMPTY)
			drop(y + 1, x);
		if (grid[y][x - 1] == EMPTY)
			drop(y, x - 1);
		if (grid[y][x + 1] == EMPTY)
			drop(y, x + 1);
	}

    void fill() {
		for (int i = 0; i < HEIGHT; i++)
			for (int j = 0; j < WIDTH; j++)
				if (grid[i][j] == NOT_TO_BE_FILLED)
					grid[i][j] = EMPTY;
				else
					grid[i][j] = WALL;
	}

    bool pointInWall(int y, int x) {
        return grid[y / tileSize][x / tileSize] == WALL;
    }

    bool pointInNewWall(int y, int x) {
        return grid[y / tileSize][x / tileSize] == NEW_WALL;
    }

    void clear() {
        for (int i = 1; i < HEIGHT - 1; i++)
            for (int j = 1; j < WIDTH - 1; j++)
                grid[i][j] = EMPTY;
    }

    bool cellIsNewWall(int y, int x) {
        		return grid[y][x] == NEW_WALL;
    }

    void newWall(int y, int x) {
        if (grid[y][x] == EMPTY) grid[y][x] = NEW_WALL;
    }

    bool isWall(int y, int x) {
		return grid[y][x] == WALL;
	}
    bool isNewWall(int y, int x) {
        return grid[y][x] == NEW_WALL;
    }
    bool isEmpty(int y, int x) {
        return grid[y][x] == EMPTY;
    }
};

Grid grid;

struct Enemy
{
    int x,y,dx,dy;

  Enemy()
   {
    x=y=300;
    dx=4-rand()%8;
    dy=4-rand()%8;
   }

  void move()
   { 
    x+=dx; if (grid.pointInWall(y,x)) {dx=-dx; x+=dx;}
    y+=dy; if (grid.pointInWall(y, x)) {dy=-dy; y+=dy;}
   }
};




int xonix()
{
    srand(time(0));

    RenderWindow window(VideoMode(WIDTH*tileSize, HEIGHT*tileSize), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1,t2,t3;
    t1.loadFromFile("images/xonix/tiles.png");
    t2.loadFromFile("images/xonix/gameover.png");
    t3.loadFromFile("images/xonix/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100,100);
    sEnemy.setOrigin(20,20);

    const int enemyCount = 4;
    Enemy enemies[enemyCount];

    bool Game=true;
    int x=0, y=0, dx=0, dy=0;
    float timer=0, delay=0.07; 
    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
               
            if (e.type == Event::KeyPressed)
             if (e.key.code==Keyboard::Escape)
               {
				grid.clear();
                x=10;y=0;
                Game=true;
               }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) {dx=-1;dy=0;};
        if (Keyboard::isKeyPressed(Keyboard::Right))  {dx=1;dy=0;};
        if (Keyboard::isKeyPressed(Keyboard::Up)) {dx=0;dy=-1;};
        if (Keyboard::isKeyPressed(Keyboard::Down)) {dx=0;dy=1;};
        
        if (!Game) continue;

        if (timer>delay)
        {
         x+=dx;
         y+=dy;

         if (x<0) x=0; if (x>WIDTH-1) x=WIDTH-1;
         if (y<0) y=0; if (y>HEIGHT-1) y=HEIGHT-1;

         if (grid.cellIsNewWall(y,x)) Game=false;

         grid.newWall(y, x);
         
         timer=0;
        }

        for (int i=0;i<enemyCount;i++) enemies[i].move();

        if (grid.isWall(y, x)) //player touches filled square
        {
            dx = dy = 0; //stop player

            //marks all tiles that are connected to enemy as -1
            for (int i = 0; i < enemyCount; i++)
                grid.drop(enemies[i].y / tileSize, enemies[i].x / tileSize);

            grid.fill();
        }
        //if player touches enemy, game over
        for (int i=0;i<enemyCount;i++)
           if  (grid.pointInNewWall(enemies[i].y, enemies[i].x)) Game=false;

      /////////draw//////////
      window.clear();

      //draw wall times
      for (int i=0;i<HEIGHT;i++)
        for (int j=0;j<WIDTH;j++)
         {
            if (grid.isEmpty(i,j)) continue;
            if (grid.isWall(i,j)) sTile.setTextureRect(IntRect(0, 0, tileSize, tileSize));
            if (grid.isNewWall(i,j)) sTile.setTextureRect(IntRect(54,0,tileSize,tileSize));
            sTile.setPosition(j*tileSize,i*tileSize);
            window.draw(sTile);
         }

      //draw player
      sTile.setTextureRect(IntRect(36,0,tileSize,tileSize));
      sTile.setPosition(x*tileSize,y*tileSize);
      window.draw(sTile);


      sEnemy.rotate(10);
      
      //draw enemy
      for (int i=0;i<enemyCount;i++)
       {
        sEnemy.setPosition(enemies[i].x,enemies[i].y);
        window.draw(sEnemy);
       }

      if (!Game) window.draw(sGameover);

      window.display();
    }

    return 0;
}
