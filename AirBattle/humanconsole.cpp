#include "humanconsole.h"

HumanConsole::HumanConsole(GameCenter* gameCenter) {
    this->gameCenter = gameCenter;
    active = false;
    timeCount = 0;
    keyUp = keyDown = keyLeft = keyRight = false;
    keyShift = false;
}

void HumanConsole::init() {
    gameInfo.gameStatus = INIT;
    gameInfo.score = gameInfo.round = 0;
    gameInfo.planeX = PLANE_INIT_X;
    gameInfo.planeY = PLANE_INIT_Y;
    gameInfo.planeSkillsNum = 0;
    running = false;
    gameCenter->init(QPointF(gameInfo.planeX, gameInfo.planeY), QPointF(BULLET_X, BULLET_Y));
    gameCenter->Continue();
}

void HumanConsole::run() {
    gameInfo.gameStatus = BATTLE;
}

void HumanConsole::onTimer() {
    if (!active) return;
    if (gameInfo.gameStatus != BATTLE) return;

    timeCount ++;
    if (timeCount % 5 == 0) {
        vector<NewBullet> nb;
        string msg;
        gameInfo.round ++;
        getAction(gameInfo, nb, msg);
        gameInfo.round --;
        for (int i = 0; i < nb.size(); i ++)
            if (isValidNewBullet(nb[i]))
                newBullets[nb[i].initTime][getBulletType(nb[i].vx, nb[i].vy)].push_back(nb[i]);
        nb.clear();
        update(nb);
        for (int i = 0; i < nb.size(); i ++) {
            double vx = nb[i].vx, vy = nb[i].vy;
            vx *= 10;
            vy *= 10;
            gameCenter->addBullet(QPointF(BULLET_X, BULLET_Y), QPointF(vx, vy));
        }
        gameInfo.round ++;
    }

//    qDebug() << gameInfo.round;
//    for (int i = 0; i < gameInfo.bullets.size(); i ++) {
//        double t = (timeCount % 5) / 5.0;
//        double x = gameInfo.bullets[i].x, y = gameInfo.bullets[i].y;
//        qDebug() << x << "," << y;
//        double vx = gameInfo.bullets[i].vx, vy = gameInfo.bullets[i].vy;
//        x += t*vx;
//        y += t*vy;
//        if (SQR(gameInfo.planeX - x) + SQR(gameInfo.planeY - y) <= SQR(BULLET_R[0] + EPSILON)) {
//            gameInfo.gameStatus = BOSS_WIN;
//            gameCenter->Pause();
//            break;
//        }
//    }
//    qDebug() << "";

    vector<QPointF> bp;
    gameCenter->getBulletsPos(bp);
    for (int i = 0; i < bp.size(); i ++) {
        double x = bp[i].x(), y = bp[i].y();
        if (SQR(gameInfo.planeX - x) + SQR(gameInfo.planeY - y) <= SQR(BULLET_R[0] + EPSILON)) {
            gameInfo.gameStatus = BOSS_WIN;
            gameCenter->Pause();
            break;
        }
    }

    if (keyShift) cntSpeed = 4.0;
    else cntSpeed = 8.0;

    double dx = 0, dy = 0;
    if (keyUp) dy += cntSpeed;
    if (keyDown) dy -= cntSpeed;
    if (keyRight) dx += cntSpeed;
    if (keyLeft) dx -= cntSpeed;
    gameInfo.planeX += dx;
    gameInfo.planeY += dy;
    gameCenter->ElementMoveTo(1, QPointF(gameInfo.planeX, gameInfo.planeY), 1.0/50.0);

}

void HumanConsole::update(vector<NewBullet> &validNewBullets) {
    vector<Bullet> bullets;
    int cntBulletsNum[5], limitBulletsNum[5], maxNewBulletsNum[5];
    memset(cntBulletsNum, 0, sizeof(cntBulletsNum));
    for (int i = 0; i < 5; i ++)
        limitBulletsNum[i] = (int)(BULLET_INIT_LIMIT[i] * BULLET_INCREASE(gameInfo.round));

    for (int i = 0; i < gameInfo.bullets.size(); i ++) {
        int type = getBulletType(gameInfo.bullets[i].vx, gameInfo.bullets[i].vy);
        double x = gameInfo.bullets[i].x;
        double y = gameInfo.bullets[i].y;
        x += gameInfo.bullets[i].vx;
        y += gameInfo.bullets[i].vy;
        if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
            cntBulletsNum[type] ++;
            Bullet bullet;
            bullet.x = x;
            bullet.y = y;
            bullet.vx = gameInfo.bullets[i].vx;
            bullet.vy = gameInfo.bullets[i].vy;
            bullets.push_back(bullet);
        }
    }

    for (int i = 0; i < 5; i ++) {
        maxNewBulletsNum[i] = max(MIN_BULLETS, (int)(OUTPUT_FACTOR * (limitBulletsNum[i] - cntBulletsNum[i])));
        limitBulletsNum[i] = min(limitBulletsNum[i], cntBulletsNum[i] + maxNewBulletsNum[i]);
    }

    for (int j = 0; j < 5; j ++) {
        for (int i = 0; i < newBullets[gameInfo.round][j].size(); i ++) {
            if (cntBulletsNum[j] >= limitBulletsNum[j]) break;
            NewBullet& newBullet = newBullets[gameInfo.round][j][i];
            int type = getBulletType(newBullet.vx, newBullet.vy);
            double x = BULLET_X, y = BULLET_Y;
            x += newBullet.vx;
            y += newBullet.vy;
            if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
                validNewBullets.push_back(newBullet);
                cntBulletsNum[type] ++;
                Bullet bullet;
                bullet.x = x;
                bullet.y = y;
                bullet.vx = newBullet.vx;
                bullet.vy = newBullet.vy;
                bullets.push_back(bullet);
            }
        }
    }

    gameInfo.bullets = bullets;
}

int HumanConsole::getBulletType(double vx, double vy) {
    for (int i = 0; i < 5; i ++)
        if (fabs(SQR(vx)+SQR(vy) - SQR(BULLET_V[i])) < EPSILON)
            return i;
    return -1;
}

bool HumanConsole::isValidNewBullet(const NewBullet &bullet) {
    if (bullet.initTime < gameInfo.round || bullet.initTime > 3000) return false;
    if (getBulletType(bullet.vx, bullet.vy) == -1) return false;
    return true;
}
