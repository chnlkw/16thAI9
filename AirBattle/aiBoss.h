#ifndef AIBOSS_H
#define AIBOSS_H

#include "common.h"
#include <ctime>
#include <cstdlib>

void init(string& name);
void getAction(const GameInfo& gameInfo, vector<NewBullet>& newBullets, string& msg);

#endif // AIBOSS_H
