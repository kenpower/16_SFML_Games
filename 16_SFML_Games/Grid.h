#pragma once
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
