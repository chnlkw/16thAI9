#ifndef COMMON_H
#define COMMON_H

#include "const.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

// Boss新发出的子弹
struct NewBullet {
	// 起始时间
    int initTime;
    // 速度大小
    double vx, vy;
};

// Boss已发出的子弹
struct Bullet {
	// 当前位置坐标
    double x, y;
    // 速度大小
    double vx, vy;
};

// Plane的移动
struct Move {
	// 移动的起始、终止时间
    int startTime, endTime;
    // 移动的速度大小
    double vx, vy;
};

// Plane的技能
struct Skill {
	// 使用技能的时间
    int startTime;
    // 技能的类型
    PLANE_SKILLS type;
};

// 游戏局面信息
struct GameInfo {
	// 当前回合数，得分
    int round, score;
    // 当前游戏状态
    GAME_STATUS gameStatus;
    // 飞机当前可用的技能点数
    int planeSkillsNum[2];
    // 飞机当前的坐标
    double planeX, planeY;
    // 当前的子弹信息
    vector<Bullet> bullets;
};


#endif // COMMON_H
