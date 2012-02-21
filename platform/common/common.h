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

struct PlaneAction {
    int startTime, endTime;
    double dx, dy;
};

struct Player {
    QString name;
    int socketDescriptor;
};

struct GameInfo {
    int round, score;
    GAME_STATUS gameStatus;
    double planeX, planeY;
    vector<Bullet> bullets;
};

void updateGameInfo(GameInfo& gameInfo, const vector<NewBullet>& newBullets, const vector<PlaneAction>& planeActions);

#endif // COMMON_H
