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

    void markConnectedCellsNotToBeFilled(int y, int x) {
		if (grid[y][x] == EMPTY)
			grid[y][x] = NOT_TO_BE_FILLED;
		if (grid[y - 1][x] == EMPTY)
			markConnectedCellsNotToBeFilled(y - 1, x);
		if (grid[y + 1][x] == EMPTY)
			markConnectedCellsNotToBeFilled(y + 1, x);
		if (grid[y][x - 1] == EMPTY)
			markConnectedCellsNotToBeFilled(y, x - 1);
		if (grid[y][x + 1] == EMPTY)
			markConnectedCellsNotToBeFilled(y, x + 1);
	}

    void fillEmptyCells() {
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

class Player {
public:
    int x, y, dx, dy;
    Player() {
        x = 0;
        y = 0;
        dx = 0;
        dy = 0;
    }

    void move() {
        x += dx;
        y += dy;
        constrain();
    }

    void constrain(){
        if (x < 0) x = 0;
        if (x > WIDTH - 1) x = WIDTH - 1;
        if (y < 0) y = 0;
        if (y > HEIGHT - 1) y = HEIGHT - 1;
    }

    void stop() {
		dx = 0;
		dy = 0;
	}

    void reset(){
        x = 10; y = 0;
    }

    void goLeft() {
        dx = -1;
        dy = 0;
    }
    void goRight() {
        dx = +1;
        dy = 0;
    }
    void goUp() {
        dx = 0;
        dy = -1;
    }
    void goDown() {
        dx = 0;
        dy = +1;
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

    bool gameIsActive=true;
    Player player;
    float frameTimer = 0;
    const float FRAME_TIME=0.07; 
    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        frameTimer+=time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
               
            if (e.type == Event::KeyPressed)
             if (e.key.code==Keyboard::Escape)
               {
				grid.clear();
                player.reset();
                gameIsActive=true;
               }
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) player.goLeft() ;
        if (Keyboard::isKeyPressed(Keyboard::Right))   player.goRight() ;
        if (Keyboard::isKeyPressed(Keyboard::Up))  player.goUp() ;
        if (Keyboard::isKeyPressed(Keyboard::Down))  player.goDown() ;
        
        if (!gameIsActive) continue;

        bool newFrame = frameTimer > FRAME_TIME;
        if (newFrame)
        {
             player.move();

             bool playerIsTouchesNewWall = grid.cellIsNewWall(player.y, player.x);
             if (playerIsTouchesNewWall) 
                 gameIsActive=false;

             grid.newWall(player.y, player.x);
         
             frameTimer=0;
        }

        for (int i=0;i<enemyCount;i++) enemies[i].move();

        bool playerTouchesFilledWall = grid.isWall(player.y, player.x);

        if (playerTouchesFilledWall) 
        {
            player.stop();

            for (int i = 0; i < enemyCount; i++)
                grid.markConnectedCellsNotToBeFilled(enemies[i].y / tileSize, enemies[i].x / tileSize);

            grid.fillEmptyCells();
        }
        //if enemy touches new wall, game over
        for (int i=0;i<enemyCount;i++)
           if  (grid.pointInNewWall(enemies[i].y, enemies[i].x)) 
               gameIsActive=false;

      /////////draw//////////
      window.clear();

      IntRect blueTile(0, 0, tileSize, tileSize);
      IntRect greenTile(54, 0, tileSize, tileSize);
      IntRect redTile(36,0,tileSize,tileSize);

      //draw wall times
      for (int i=0;i<HEIGHT;i++)
        for (int j=0;j<WIDTH;j++)
         {
            if (grid.isEmpty(i,j))   continue;
            if (grid.isWall(i,j))    sTile.setTextureRect(blueTile);
            if (grid.isNewWall(i,j)) sTile.setTextureRect(greenTile);
            sTile.setPosition(j*tileSize,i*tileSize);
            window.draw(sTile);
         }

      //draw player
      sTile.setTextureRect(redTile);
      sTile.setPosition(player.x*tileSize,player.y*tileSize);
      window.draw(sTile);


      sEnemy.rotate(10);
      
      //draw enemy
      for (int i=0;i<enemyCount;i++)
       {
        sEnemy.setPosition(enemies[i].x,enemies[i].y);
        window.draw(sEnemy);
       }

      if (!gameIsActive) window.draw(sGameover);

      window.display();
    }

    return 0;
}
