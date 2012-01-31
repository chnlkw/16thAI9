#ifndef CONST_H
#define CONST_H

#include <QString>
#include <QHostAddress>

#define SQR(a) ((a) * (a))

using namespace std;

const QHostAddress SERVER_ADDR("127.0.0.1");
const quint16 SERVER_PORT = 20129;

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
const double WIDTH = 600;
const double HEIGHT = 800;

const double PLANE_INIT_X = 100;
const double PLANE_INIT_Y = 100;
const double PLANE_V = 40;

#endif // CONST_H
