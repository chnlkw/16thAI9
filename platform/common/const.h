#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QHostAddress>
#include <cmath>

#define BULLET_INCREASE(t) (exp(0.00053648 * (t)))

using namespace std;

const QHostAddress SERVER_ADDR("127.0.0.1");
const quint16 SERVER_PORT = 20129;
const int SOCKET_RECV_TIMEOUT = 20;

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

enum PLANE_SKILLS {
    SPEEDUP = 0,
    BOMB
};

const double WIDTH = 1000;
const double HEIGHT = 1000;

const double BULLET_X = 500;
const double BULLET_Y = 800;
const double BULLET_V[5] = {34, 38, 42, 46, 50};
const double BULLET_R[5] = {15, 15, 15, 15, 15};
const int BULLET_INIT_LIMIT[5] = {20, 18, 16, 14, 12};
const double OUTPUT_FACTOR = 0.4;
const int MIN_BULLETS = 6;

const double PLANE_INIT_X = 500;
const double PLANE_INIT_Y = 100;
const double PLANE_V = 40;
const int GAIN_SKILLPOINT = 100;
const int SPEEDUP_COST = 2;
const int BOMB_COST = 5;
const int SPEEDUP_TIME = 30;

const double EPSILON = 1e-4;

#endif // CONST_H
