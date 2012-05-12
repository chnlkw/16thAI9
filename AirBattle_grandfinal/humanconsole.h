#ifndef HUMANCONSOLE_H
#define HUMANCONSOLE_H

#include "aiBoss.h"
#include "GameCenter.h"

class HumanConsole : public QObject
{
    Q_OBJECT
public:
    HumanConsole(GameCenter* gameCenter);
    void run();
    void restart();
    void update(vector<NewBullet>& validNewBullets);

    bool keyUp, keyDown, keyLeft, keyRight;
    bool keyShift, keyZ, keyX;
    int timeCount;
    int lastMove, lastSpeedup;
    double cntSpeed;

    bool running;

    GameInfo gameInfo;
    GameCenter* gameCenter;

signals:
    void gameOver();

    void setTime(int);
    void setScore(int);
    void setBossName(QString);
    void setPlayerName(QString);

    void setValue1(int);
    void setValue2(int);

public slots:
    void onTimer();

private:
    int getBulletType(double vx, double vy);
    bool isValidNewBullet(const NewBullet& bullet);
    vector<NewBullet> newBullets[3001][5];
};

#endif // HUMANCONSOLE_H
