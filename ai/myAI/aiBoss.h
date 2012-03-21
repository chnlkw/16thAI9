#ifndef AIBOSS_H
#define AIBOSS_H

#include "common.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

DLL_EXPORT void init(string& name);
DLL_EXPORT void getAction(const GameInfo& gameInfo, vector<NewBullet>& newBullets, string& msg);

#endif
