#include "common.h"

void updateGameInfo(GameInfo &gameInfo, const vector<NewBullet> &newBullets, const vector<PlaneAction> &planeActions) {
    // Calculate planeX, planeY
    double move[gameInfo.round][2];
    memset(move, 0, sizeof(move));
    for (int i = 0; i < planeActions.size(); i ++) {
        const PlaneAction& act = planeActions[i];
        for (int t = act.startTime; t <= MIN(gameInfo.round - 1, act.endTime); t ++) {
            move[t][0] = act.dx;
            move[t][1] = act.dy;
        }
    }
    double planeX = PLANE_INIT_X, planeY = PLANE_INIT_Y;
    for (int t = 0; t < gameInfo.round; t ++) {
        planeX += move[t][0];
        planeY += move[t][1];
    }
    gameInfo.planeX = planeX;
    gameInfo.planeY = planeY;

    // Calculate bullets
    vector<Bullet> bullets;
    for (int i = 0; i < newBullets.size(); i ++) {
        const NewBullet& nb = newBullets[i];
        if (nb.initTime > gameInfo.round ||
                (gameInfo.round - nb.initTime) * BULLET_V > sqrt(WIDTH*WIDTH + HEIGHT*HEIGHT)) continue;
        double x = nb.x, y = nb.y;
        x += nb.vx * (gameInfo.round - nb.initTime);
        y += nb.vy * (gameInfo.round - nb.initTime);
        if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
            Bullet bullet;
            bullet.x = x;
            bullet.y = y;
            bullet.vx = nb.vx;
            bullet.vy = nb.vy;
            bullets.push_back(bullet);
        }
    }
    gameInfo.bullets = bullets;
}


