#ifndef COMMON_H
#define COMMON_H

#include "utils.h"
#include <vector>

using namespace std;

struct Coordinate {
    double x, y;
    double dist(Coordinate p) { return sqrt(SQR(x-p.x) + SQR(y-p.y)); }
};

struct NewBullet {
    int initTime;
    Coordinate initCoor;
    double vx, vy;
};

struct Bullet {
    Coordinate cntCoor;
    double vx, vy;
};

struct PlaneAction {
    int startTime, endTime;
    double dx, dy;
};

struct GameStatus {
    GAME_STATUS status;
    int time;
    Coordinate planePosition;
    vector<Bullet> bullets;
};

#endif // COMMON_H
