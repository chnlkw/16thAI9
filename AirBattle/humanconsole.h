#ifndef HUMANCONSOLE_H
#define HUMANCONSOLE_H

#include "aiBoss.h"
#include "GameCenter.h"

class HumanConsole : public QObject
{
    Q_OBJECT
public:
    HumanConsole(GameCenter* gameCenter);
    void init();
    void run();
    void update(vector<NewBullet>& validNewBullets);

    bool keyUp, keyDown, keyLeft, keyRight;
    bool keyShift;
    bool active;
    int timeCount;
    double cntSpeed;

    bool running;

    GameInfo gameInfo;
    GameCenter* gameCenter;

signals:

public slots:
    void onTimer();

private:
    int getBulletType(double vx, double vy);
    bool isValidNewBullet(const NewBullet& bullet);
    vector<NewBullet> newBullets[3001][5];
};

#endif // HUMANCONSOLE_H
