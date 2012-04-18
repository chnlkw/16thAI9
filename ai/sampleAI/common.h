#ifndef COMMON_H
#define COMMON_H

#include "const.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
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

struct Move {
    int startTime, endTime;
    double vx, vy;
};

struct Skill {
    int startTime;
    PLANE_SKILLS type;
};

struct GameInfo {
    int round, score;
    GAME_STATUS gameStatus;
    int planeSkillsNum;
    double planeX, planeY;
    vector<Bullet> bullets;
};


#endif // COMMON_H
