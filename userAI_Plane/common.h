#ifndef COMMON_H
#define COMMON_H

#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

struct NewBullet {
    int initTime;
    double x, y;
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

struct GameStatus {
    GAME_STATUS status;
    int time;
    double planeX, planeY;
    vector<Bullet> bullets;
};

struct Player {
    QString name;
    int socketDescriptor;
};

#endif // COMMON_H
