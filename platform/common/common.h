#ifndef COMMON_H
#define COMMON_H

#include "utils.h"
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

struct Player {
    QString name;
    int socketDescriptor;
};

struct GameInfo {
    int round, score;
    GAME_STATUS gameStatus;
    int planeSkillsNum[2];
    double planeX, planeY;
    vector<Bullet> bullets;
};

int getBulletType(double vx, double vy);

#endif // COMMON_H
