#pragma once

class Player {

public:
    int x, y, dx, dy;
    Player() {
        reset();
    }

    void move() {
        x += dx;
        y += dy;
        constrain();
    }

    void constrain() {
        if (x < 0) x = 0;
        if (x > WIDTH - 1) x = WIDTH - 1;
        if (y < 0) y = 0;
        if (y > HEIGHT - 1) y = HEIGHT - 1;
    }

    void stop() {
        dx = 0;
        dy = 0;
    }

    void reset() {
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

};