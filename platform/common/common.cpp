#include "common.h"

int getBulletType(double vx, double vy) {
    for (int i = 0; i < 5; i ++)
        if (fabs(SQR(vx)+SQR(vy) - SQR(BULLET_V[i]) < EPSILON))
            return i;
    return -1;
}
