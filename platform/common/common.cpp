#include "common.h"

int getBulletType(double vx, double vy) {
    for (int i = 0; i < 5; i ++)
        if (fabs(SQR(vx)+SQR(vy) - SQR(BULLET_V[i]) < EPSILON))
            return i;
    return -1;
}

void updateGameInfo(GameInfo& gameInfo, const vector<NewBullet>& newBullets,
                    vector<Move>& moves, const vector<Skill>& skills,
                    vector<int>& newBulletsId, int& lastSpeedup, bool& useBomb) {
    // Use skills
    useBomb = false;
    for (int i = 0; i < skills.size(); i ++) {
        const Skill& skill = skills[i];
        if (skill.startTime != gameInfo.round) continue;
        if (gameInfo.planeSkillsNum[(int)skill.type] == 0) continue;
        switch (skill.type) {
        case SPEEDUP:
            lastSpeedup = gameInfo.round;
            break;
        case BOMB:
            useBomb = true;
            break;
        }
        gameInfo.planeSkillsNum[(int)skill.type] --;
    }

    // Calculate planeX, planeY
    double vx = 0, vy = 0;
    vector<Move> validMoves;
    for (int i = 0; i < moves.size(); i ++) {
        const Move& move = moves[i];
        if (move.endTime <= gameInfo.round) continue;
        if (move.startTime == gameInfo.round &&
                SQR(move.vx) + SQR(move.vy) > SQR(PLANE_V) && lastSpeedup + SPEEDUP_TIME <= gameInfo.round)
            continue;
        validMoves.push_back(move);
        if (move.startTime <= gameInfo.round) {
            vx = move.vx;
            vy = move.vy;
        }
    }
    moves = validMoves;
    double planeX = gameInfo.planeX + vx, planeY = gameInfo.planeY + vy;

    if (planeX < 0) planeX = 0;
    if (planeX > WIDTH) planeX = WIDTH;
    if (planeY < 0) planeY = 0;
    if (planeY > HEIGHT) planeY = HEIGHT;

    if (gameInfo.round > 1 && vx == 0 && vy == 0)
        gameInfo.score ++;
    if (gameInfo.score > 0 && gameInfo.score % 50 == 0)
        gameInfo.planeSkillsNum[0] ++;
    if (gameInfo.score > 0 && gameInfo.score % 100 == 0)
        gameInfo.planeSkillsNum[1] ++;
    gameInfo.planeX = planeX;
    gameInfo.planeY = planeY;

    // Calculate bullets
    vector<Bullet> bullets;
    if (!useBomb) {
        int cntBulletsNum[5], limitBulletsNum[5];
        memset(cntBulletsNum, 0, sizeof(cntBulletsNum));
        for (int i = 0; i < 5; i ++)
            limitBulletsNum[i] = (int)(BULLET_INIT_LIMIT[i] * BULLET_INCREASE(gameInfo.round + 1));
        printf("%d\n", newBullets.size());
        for (int i = 0; i < newBullets.size(); i ++) {
            const NewBullet& nb = newBullets[i];
            if (nb.initTime > gameInfo.round + 1 ||
                    (gameInfo.round + 1 - nb.initTime) * BULLET_V[0] > sqrt(WIDTH*WIDTH + HEIGHT*HEIGHT)) continue;
            int type = getBulletType(nb.vx, nb.vy);
            if (cntBulletsNum[type] >= limitBulletsNum[type]) continue;

            if (nb.initTime == gameInfo.round)
                newBulletsId.push_back(i);

            double x = BULLET_X, y = BULLET_Y;
            x += nb.vx * (gameInfo.round + 1 - nb.initTime);
            y += nb.vy * (gameInfo.round + 1 - nb.initTime);
            if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
                cntBulletsNum[type] ++;
                Bullet bullet;
                bullet.x = x;
                bullet.y = y;
                bullet.vx = nb.vx;
                bullet.vy = nb.vy;
                bullets.push_back(bullet);
            }
        }
    }
    gameInfo.bullets = bullets;
}

