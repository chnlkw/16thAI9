#ifndef AIPLANE_H
#define AIPLANE_H

#include "common.h"

#define DLL_EXPORT extern "C" __declspec(dllexport)

DLL_EXPORT void init(string& name);
DLL_EXPORT void getAction(const GameInfo& gameInfo, vector<PlaneAction>& planeActions, string& msg);

#endif
