#pragma once
#include<cmath>
#include<string>

float DEGTORAD = 0.017453f;
const int W = 1200;
const int H = 800;

class EntityController {
public:
    float x, y, dx, dy, R, angle;
    bool life;
    std::string name;

    EntityController()
    {
        life = 1;
    }
    void settings(int X, int Y, float Angle = 0, int radius = 1)
    {
        x = X; y = Y;
        angle = Angle;
        R = radius;
    }

    virtual void update() {};

};

class bullet : public EntityController
{
public:
    bullet()
    {
        name = "bullet";
    }

    void  update()
    {
        dx = cos(angle * DEGTORAD) * 6;
        dy = sin(angle * DEGTORAD) * 6;
        // angle+=rand()%7-3;  /*try this*/
        x += dx;
        y += dy;

        if (x > W || x<0 || y>H || y < 0) life = 0;
    }

};


class player : public EntityController
{
public:
    bool thrust;

    player()
    {
        name = "player";
        dx = dy = 0;
    }

    void update()
    {
        if (thrust)
        {
            dx += cos(angle * DEGTORAD) * 0.2;
            dy += sin(angle * DEGTORAD) * 0.2;
        }
        else
        {
            dx *= 0.99;
            dy *= 0.99;
        }

        int maxSpeed = 15;
        float speed = sqrt(dx * dx + dy * dy);
        if (speed > maxSpeed)
        {
            dx *= maxSpeed / speed;
            dy *= maxSpeed / speed;
        }

        x += dx;
        y += dy;

        if (x > W) x = 0; if (x < 0) x = W;
        if (y > H) y = 0; if (y < 0) y = H;
    }

};

class asteroid : public EntityController
{
public:
    asteroid()
    {
        dx = rand() % 8 - 4;
        dy = rand() % 8 - 4;
        name = "asteroid";
    }

    void update()
    {
        x += dx;
        y += dy;

        if (x > W) x = 0;  if (x < 0) x = W;
        if (y > H) y = 0;  if (y < 0) y = H;
    }

};


bool isCollide(EntityController* a, EntityController* b)
{
    return (b->x - a->x) * (b->x - a->x) +
        (b->y - a->y) * (b->y - a->y) <
        (a->R + b->R) * (a->R + b->R);
}

