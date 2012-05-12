#include "humanconsole.h"

HumanConsole::HumanConsole(GameCenter* gameCenter) {
    this->gameCenter = gameCenter;
    restart();
}

void HumanConsole::restart() {
    timeCount = 0;
    lastMove = 0;
    lastSpeedup = -1000;
    running = false;

    keyUp = keyDown = keyLeft = keyRight = false;
    keyShift = keyZ = keyX = false;

    gameInfo.gameStatus = INIT;
    gameInfo.score = gameInfo.round = 0;
    gameInfo.planeX = PLANE_INIT_X;
    gameInfo.planeY = PLANE_INIT_Y;
    gameInfo.planeSkillsNum = 0;

    string bossName;
    init(bossName);
    emit setBossName(QString(bossName.c_str()));
    emit setPlayerName(QString("Human"));
    emit setTime(0);
    emit setScore(0);
    emit setValue1(0);
    emit setValue2(0);

    gameCenter->init(QPointF(gameInfo.planeX, gameInfo.planeY), QPointF(BULLET_X, BULLET_Y));
}

void HumanConsole::run() {
    gameInfo.gameStatus = BATTLE;
}

void HumanConsole::onTimer() {
    if (gameInfo.gameStatus != BATTLE) return;

    /* Boss bullets */
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

        if (gameInfo.round % 100 == 0) gameInfo.planeSkillsNum ++;

        emit setTime(gameInfo.round);
        emit setValue1(gameInfo.planeSkillsNum);
        emit setValue2(gameInfo.planeSkillsNum);
    }

    /* Use skill */
    if (keyX) {
        if (gameInfo.planeSkillsNum < 5) goto fail;
        gameInfo.planeSkillsNum -= 5;
        emit setValue1(gameInfo.planeSkillsNum);
        emit setValue2(gameInfo.planeSkillsNum);
        gameInfo.bullets.clear();
        gameCenter->Bomb(QPointF(gameInfo.planeX, gameInfo.planeY));
    } else if (keyZ) {
        if (gameInfo.planeSkillsNum < 2) goto fail;
        gameInfo.planeSkillsNum -= 2;
        emit setValue1(gameInfo.planeSkillsNum);
        emit setValue2(gameInfo.planeSkillsNum);
        lastSpeedup = timeCount;
        gameCenter->SpeedUP(QPointF(gameInfo.planeX, gameInfo.planeY));
    }
fail:

    /* Judge hit */
    vector<QPointF> bp;
    gameCenter->getBulletsPos(bp);
    for (int i = 0; i < bp.size(); i ++) {
        double x = bp[i].x(), y = bp[i].y();
        if (SQR(gameInfo.planeX - x) + SQR(gameInfo.planeY - y) <= SQR(BULLET_R[0] + EPSILON)) {
            gameInfo.gameStatus = BOSS_WIN;
            emit gameOver();
            break;
        }
    }

    /* Game score */
    if (!keyUp && !keyDown && !keyRight && !keyLeft) {
        if (timeCount - lastMove == 5) {
            gameInfo.score ++;
            gameCenter->addPlaneBullet(QPointF(gameInfo.planeX, gameInfo.planeY), QPointF(BULLET_X, BULLET_Y));
            emit setScore(gameInfo.score);
            lastMove = timeCount;
        }
    } else lastMove = timeCount;

    /* Plane move */
    if (keyShift) cntSpeed = 4.0;
    else cntSpeed = 8.0;

    if (timeCount - lastSpeedup <= 150) cntSpeed *= 2;

    double dx = 0, dy = 0;
    if (keyUp) dy += cntSpeed;
    if (keyDown) dy -= cntSpeed;
    if (keyRight) dx += cntSpeed;
    if (keyLeft) dx -= cntSpeed;

    gameInfo.planeX += dx;
    gameInfo.planeY += dy;

    if (gameInfo.planeX < 0) gameInfo.planeX = 0;
    if (gameInfo.planeY < 0) gameInfo.planeY = 0;
    if (gameInfo.planeX > 1000) gameInfo.planeX = 1000;
    if (gameInfo.planeY > 1000) gameInfo.planeY = 1000;

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
