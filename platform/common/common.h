#ifndef COMMON_H
#define COMMON_H

#include "utils.h"

struct Coordinate {
    double x, y;
    double dist(Coordinate p) { return sqrt(SQR(x-p.x) + SQR(y-p.y)); }
};

struct Bullet {
    Coordinate initCoor;
    double vx, vy;
};

struct PlaneAction {
    int startTime, endTime;
    double dx, dy;
};

#endif // COMMON_H
