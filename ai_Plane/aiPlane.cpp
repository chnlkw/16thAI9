#include "aiPlane.h"
#include <cstdlib>
#include <ctime>
using namespace std;

int randNum[100000];
int cntRand;

void init(string& name) {
	srand(time(0) % 12345);
	cntRand = 0;
	for (int i = 0; i < 100000; i ++)
		randNum[i] = rand();
	name = "plane";
}

void getAction(const GameInfo& gameInfo, vector<PlaneAction>& planeActions, string& msg) {
	/*PlaneAction planeAction;
    planeAction.startTime = gameInfo.round * 10 + 1;
    planeAction.endTime = gameInfo.round * 10 + 11;
	int t = randNum[cntRand++] % 2;
	int p = (t == 1 ? 1 : -1);
    planeAction.dx = p * (randNum[cntRand++] % 10 + 1);
    t = randNum[cntRand++] % 2;
    p = (t == 1 ? 1 : -1);
    planeAction.dy = p * (randNum[cntRand++] % 10 + 1);
    planeActions.push_back(planeAction);*/
    PlaneAction planeAction;
    planeAction.startTime = gameInfo.round + 1;
    planeAction.endTime = gameInfo.round + 2;
    planeAction.dx = 0;
    planeAction.dy = PLANE_V;
    planeActions.push_back(planeAction);
    msg = "I'm plane.";
}

