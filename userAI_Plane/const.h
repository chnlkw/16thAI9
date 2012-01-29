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
    BATTLE = 0,
    BOSS_WIN,
    PLANE_WIN
};

#endif // CONST_H
