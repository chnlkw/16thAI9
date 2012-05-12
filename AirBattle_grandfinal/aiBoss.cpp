#include "aiBoss.h"

using namespace std;

#define SQR(a) ((a) * (a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void init(string& name)
{
        srand(time(0));
        name = "boss";
}

// ����ӵ����͵ĺ�����
int getBulletType(double vx, double vy) {
    for (int i = 0; i < 5; i ++)
        if (fabs(SQR(vx) + SQR(vy) - SQR(BULLET_V[i])) < EPSILON)
            return i;
    return -1;
}

void getAction(const GameInfo& gameInfo, vector<NewBullet>& newBullets, string& msg)
{
        // ���㵱ǰ�ѷ��ӵ���Ŀ���Լ������ӵ�����Ŀ��
    int cntBulletsNum[5], limitBulletsNum[5], maxNewBulletsNum[5];
    memset(cntBulletsNum, 0, sizeof(cntBulletsNum));
    for (int i = 0; i < 5; i ++)
        limitBulletsNum[i] = (int)(BULLET_INIT_LIMIT[i] * BULLET_INCREASE(gameInfo.round));

    for (int i = 0; i < gameInfo.bullets.size(); i ++) {
        int type = getBulletType(gameInfo.bullets[i].vx, gameInfo.bullets[i].vy);
        cntBulletsNum[type] ++;
    }

    for (int i = 0; i < 5; i ++) {
        maxNewBulletsNum[i] = MAX(MIN_BULLETS, (int)(OUTPUT_FACTOR * (limitBulletsNum[i] - cntBulletsNum[i])));
        limitBulletsNum[i] = MIN(limitBulletsNum[i], cntBulletsNum[i] + maxNewBulletsNum[i]);
    }

    // ÿ���ӵ���������
    for (int i = 0; i < 5; i ++) {
        while (cntBulletsNum[i] < limitBulletsNum[i]) {
                cntBulletsNum[i] ++;
                NewBullet newBullet;
                newBullet.initTime = gameInfo.round;

                if (i < 4) {
                        // 4��Ƚ������ӵ�������䡣
                        int t = rand() % 2;
                        int p = (t == 1 ? 1 : -1);
                        newBullet.vx = p * (rand() % 100);
                        newBullet.vy = -1 * (rand() % 100 + 1);
                        } else {
                                // �����ӵ�ֱ����Plane���䡣
                                newBullet.vx = gameInfo.planeX - BULLET_X;
                                newBullet.vy = gameInfo.planeY - BULLET_Y;
                        }

                        // �ٶȴ�С��׼����
                double v = sqrt(newBullet.vx*newBullet.vx + newBullet.vy*newBullet.vy);
                        newBullet.vx = BULLET_V[i] * newBullet.vx / v;
                        newBullet.vy = BULLET_V[i] * newBullet.vy / v;
                        newBullets.push_back(newBullet);

                        // �����ӵ�ֻ����һö��
                        if (i == 4) break;
        }
    }

    msg = "I'm boss. Haha";
}


