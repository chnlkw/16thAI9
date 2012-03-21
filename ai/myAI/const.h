#ifndef CONST_H
#define CONST_H

// 子弹数目限制的增长函数
#define BULLET_INCREASE(t) (exp(0.00053648 * t))

using namespace std;

// 游戏运行状态
enum GAME_STATUS {
    INIT = 0,
    BATTLE,
    BOSS_WIN,
    PLANE_WIN
};

// Plane的技能
enum PLANE_SKILLS {
    SPEEDUP = 0,
    BOMB
};

// 对战区域的长和宽
const double WIDTH = 1000;
const double HEIGHT = 1000;

// 子弹的初始位置
const double BULLET_X = 500;
const double BULLET_Y = 800;

// 子弹的速度
const double BULLET_V[5] = {32, 36, 40, 44, 48};

// 子弹的半径
const double BULLET_R[5] = {15, 15, 15, 15, 15};

// 子弹数目的初始限制
const int BULLET_INIT_LIMIT[5] = {20, 18, 16, 14, 12};

// Plane的初始位置
const double PLANE_INIT_X = 500;
const double PLANE_INIT_Y = 100;

// Plane的速度上限
const double PLANE_V = 40;

// Plane获得一个“加速”技能需要攻击Boss的次数
const int GAIN_SPEEDUP = 50;

// Plane获得一个“清屏”技能需要攻击Boss的次数
const int GAIN_BOMB = 100;

// Plane“加速”技能的持续时间
const int SPEEDUP_TIME = 30;

// 浮点数误差的容许范围
const double EPSILON = 1e-4;

#endif // CONST_H
