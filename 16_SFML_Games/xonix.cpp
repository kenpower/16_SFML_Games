#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int HEIGHT = 25;
const int WIDTH = 40;
const int tileSize = 18; 

class Grid {
    enum tile { EMPTY = 0, WALL = 1, NEW_WALL = 2, NOT_TO_BE_FILLED = -1 };

    tile grid[HEIGHT][WIDTH] = { EMPTY };

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

class StopWatch {
	Clock clock;
    float targetTime;

public:
    StopWatch(float targetTime):targetTime(targetTime) {}

    bool isTimeUp() { 
        return clock.getElapsedTime().asSeconds() > targetTime;
    }
    void reset() {
		clock.restart();
	}
};

class Player {
    const float TIME_BETWEEN_PLAYER_MOVES = 0.07;
    StopWatch moveTimer;
public:
    int x, y, dx, dy;
    Player(): moveTimer(TIME_BETWEEN_PLAYER_MOVES) {
        reset();
    }

    void move() {
        x += dx;
        y += dy;
        constrain();
        moveTimer.reset();
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
        dx = 0;
        dy = 0;
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

    bool shouldMove() {
        return moveTimer.isTimeUp();
	}
};

Player player;
const int enemyCount = 4;
Enemy enemies[enemyCount];
bool gameOver = false;

void gameReset() {
    grid.clear();
	player.reset();
    gameOver = false;
}

bool enemyTouchesNewWall() {
	for (int i=0;i<enemyCount;i++)
		if (grid.pointInNewWall(enemies[i].y,enemies[i].x)) return true;
	return false;
}

void moveEnemies() {
	for (int i=0;i<enemyCount;i++) enemies[i].move();
}

class Screen {
    RenderWindow window;
    Texture t1, t2, t3;
    //Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    Sprite sTile, sGameover, sEnemy;
public:

    Screen():window(VideoMode(WIDTH* tileSize, HEIGHT* tileSize), "Xonix Game!") {
     
        window.setFramerateLimit(60);

        t1.loadFromFile("images/xonix/tiles.png");
        t2.loadFromFile("images/xonix/gameover.png");
        t3.loadFromFile("images/xonix/enemy.png");

        sTile.setTexture(t1);
        sGameover.setTexture(t2);
        sEnemy.setTexture(t3);

        sGameover.setPosition(100, 100);
        sEnemy.setOrigin(20, 20);
    }

    void drawFrame() {
        /////////draw//////////
        window.clear();

        IntRect blueTile(0, 0, tileSize, tileSize);
        IntRect greenTile(54, 0, tileSize, tileSize);
        IntRect redTile(36, 0, tileSize, tileSize);

        //draw wall times
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
            {
                if (grid.isEmpty(i, j))   continue;
                if (grid.isWall(i, j))    sTile.setTextureRect(blueTile);
                if (grid.isNewWall(i, j)) sTile.setTextureRect(greenTile);
                sTile.setPosition(j * tileSize, i * tileSize);
                window.draw(sTile);
            }

        //draw player
        sTile.setTextureRect(redTile);
        sTile.setPosition(player.x * tileSize, player.y * tileSize);
        window.draw(sTile);


        sEnemy.rotate(10);

        //draw enemy
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(enemies[i].x, enemies[i].y);
            window.draw(sEnemy);
        }

        if (gameOver) window.draw(sGameover);

        window.display();
    }

    bool isOpen() {
		return window.isOpen();
	}

    bool handleEvents() {
        Event e;
        bool shouldReset = false;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
					shouldReset = true;
                }
        }
        return shouldReset;
    }
};

void controlPlayer(){
    if (Keyboard::isKeyPressed(Keyboard::Left)) player.goLeft();
    if (Keyboard::isKeyPressed(Keyboard::Right))   player.goRight();
    if (Keyboard::isKeyPressed(Keyboard::Up))  player.goUp();
    if (Keyboard::isKeyPressed(Keyboard::Down))  player.goDown();
}

void movePlayer(bool& playerTouchedNewWall){

        player.move();

        playerTouchedNewWall = grid.cellIsNewWall(player.y, player.x);

        grid.newWall(player.y, player.x);
}

void playerCompletesLoop() {
    bool playerTouchesFilledWall = grid.isWall(player.y, player.x);

    if (playerTouchesFilledWall)
    {
        player.stop();

        for (int i = 0; i < enemyCount; i++)
            grid.markConnectedCellsNotToBeFilled(enemies[i].y / tileSize, enemies[i].x / tileSize);

        grid.fillEmptyCells();
    }
}
int xonix()
{
    srand(time(0));

    Screen screen;
    gameReset();

    while (screen.isOpen())
    {

        if (screen.handleEvents() == true) 
            gameReset();

        if (gameOver) continue;

        controlPlayer();

        bool playerTouchedNewWall = false;
      
        if (player.shouldMove())
            movePlayer(playerTouchedNewWall);

        moveEnemies();

        playerCompletesLoop();

        gameOver = enemyTouchesNewWall() || playerTouchedNewWall;

        screen.drawFrame();

      
    }

    return 0;
}
