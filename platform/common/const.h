#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QHostAddress>
#include <cmath>

#define BULLET_INCREASE(t) (exp(0.00053648 * (t)))

using namespace std;

const QHostAddress SERVER_ADDR("127.0.0.1");
const quint16 SERVER_PORT = 20129;
const int SOCKET_RECV_TIMEOUT = 5;

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

const double WIDTH = 600;
const double HEIGHT = 600;

const double BULLET_X = 300;
const double BULLET_Y = 600;
const double BULLET_V[5] = {12, 18, 24, 30, 36};
const double BULLET_R[5] = {10, 10, 10, 10, 10};
const int BULLET_INIT_LIMIT[5] = {20, 18, 16, 14, 12};

const double PLANE_INIT_X = 300;
const double PLANE_INIT_Y = 100;
const double PLANE_V = 30;
const int SPEEDUP_TIME = 30;

const double EPSILON = 1e-4;

#endif // CONST_H
