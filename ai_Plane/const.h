#ifndef CONST_H
#define CONST_H

#define SQR(a) ((a) * (a))

using namespace std;

enum CLIENT_TYPE {
    BOSS = 0,
    PLANE
};

enum GAME_STATUS {
    INIT = 0,
    BATTLE,
    BOSS_WIN,
    PLANE_WIN
};

const double BULLET_V = 20;
const double BULLET_R = 10;
const double WIDTH = 600;
const double HEIGHT = 800;

const double PLANE_INIT_X = 300;
const double PLANE_INIT_Y = 100;
const double PLANE_V = 40;

#endif // CONST_H
