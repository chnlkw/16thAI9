#ifndef COMMON_H
#define COMMON_H

#include "const.h"
#include <iostream>
#include <vector>

using namespace std;

struct NewBullet {
    int initTime;
    double vx, vy;
};

struct Bullet {
    double x, y;
    double vx, vy;
};

struct PlaneAction {
    int startTime, endTime;
    double dx, dy;
};

struct GameInfo {
    int round, score;
    GAME_STATUS gameStatus;
    double planeX, planeY;
    vector<Bullet> bullets;
};

#endif // COMMON_H
