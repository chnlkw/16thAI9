#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <cmath>
#include <QThread>

using namespace std;

#define BULLET_INCREASE(t) (exp(0.00053648 * (t)))
#define SQR(a) ((a) * (a))

const int SOCKET_RECV_TIMEOUT = 20;

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

class Timer : public QThread
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = 0) : QThread(parent) {}
    static void msleep(unsigned long secs) { QThread::msleep(secs); }
};

enum GAME_STATUS {
    INIT = 0,
    BATTLE,
    BOSS_WIN,
    PLANE_WIN
};

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

struct GameInfo {
    int round, score;
    GAME_STATUS gameStatus;
    int planeSkillsNum;
    double planeX, planeY;
    vector<Bullet> bullets;
};

#endif // COMMON_H
