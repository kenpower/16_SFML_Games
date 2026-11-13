#include <SFML/Graphics.hpp>
#include <time.h>
#include <optional>


using namespace sf;

#include "XonixConstants.h"
#include "Grid.h"

static Grid grid;

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

#include"Player.h"


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
    Sprite sTile, sGameover, sEnemy;
public:

    Screen():window(VideoMode({WIDTH* tileSize, HEIGHT* tileSize}), "Xonix Game!"),
             t1("images/xonix/tiles.png"),
             t2("images/xonix/gameover.png"),
             t3("images/xonix/enemy.png"),
             sTile(t1), sGameover(t2), sEnemy(t3) {

        window.setFramerateLimit(60);

        sGameover.setPosition({100, 100});
        sEnemy.setOrigin({20, 20});
    }

    void drawFrame() {
        /////////draw//////////
        window.clear();

        IntRect blueTile({0, 0}, {tileSize, tileSize});
        IntRect greenTile({54, 0}, {tileSize, tileSize});
        IntRect redTile({36, 0}, {tileSize, tileSize});

        //draw wall times
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
            {
                if (grid.isEmpty(i, j))   continue;
                if (grid.isWall(i, j))    sTile.setTextureRect(blueTile);
                if (grid.isNewWall(i, j)) sTile.setTextureRect(greenTile);
                sTile.setPosition({float(j * tileSize), float(i * tileSize)});
                window.draw(sTile);
            }

        //draw player
        sTile.setTextureRect(redTile);
        sTile.setPosition({float(player.x * tileSize), float(player.y * tileSize)});
        window.draw(sTile);


        sEnemy.rotate(degrees(10));

        //draw enemy
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition({float(enemies[i].x), float(enemies[i].y)});
            window.draw(sEnemy);
        }

        if (gameOver) window.draw(sGameover);

        window.display();
    }

    bool isOpen() {
		return window.isOpen();
	}

    bool handleEvents() {
        bool shouldReset = false;
        while (const std::optional e = window.pollEvent())
        {
            if (e->is<Event::Closed>())
                window.close();

            if (const auto* key = e->getIf<Event::KeyPressed>())
                if (key->code == Keyboard::Key::Escape)
                {
					shouldReset = true;
                }
        }
        return shouldReset;
    }
};

void controlPlayer(){
    if (Keyboard::isKeyPressed(Keyboard::Key::Left)) player.goLeft();
    if (Keyboard::isKeyPressed(Keyboard::Key::Right))   player.goRight();
    if (Keyboard::isKeyPressed(Keyboard::Key::Up))  player.goUp();
    if (Keyboard::isKeyPressed(Keyboard::Key::Down))  player.goDown();
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

    const float TIME_BETWEEN_PLAYER_MOVES = 0.07;
    StopWatch playerMoveTimer(TIME_BETWEEN_PLAYER_MOVES);

    while (screen.isOpen())
    {

        if (screen.handleEvents() == true) 
            gameReset();

        if (gameOver) continue;

        controlPlayer();

        bool playerTouchedNewWall = false;
      
        if (playerMoveTimer.isTimeUp()) {
            movePlayer(playerTouchedNewWall);
            playerMoveTimer.reset();
        }

        moveEnemies();

        playerCompletesLoop();

        gameOver = enemyTouchesNewWall() || playerTouchedNewWall;

        screen.drawFrame();

      
    }

    return 0;
}
