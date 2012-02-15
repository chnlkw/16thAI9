#include "aiBoss.h"
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
  name = "boss";
}

void getAction(const GameInfo& gameInfo, vector<NewBullet>& newBullets) {
	/*for (int i = 0; i < 10; i ++) {
		NewBullet newBullet;
		newBullet.initTime = gameInfo.round + 1;
		newBullet.x = 300;
		newBullet.y = 600;
		int t = randNum[cntRand++] % 2;
		int p = (t == 1 ? 1 : -1);
		newBullet.vx = p * (randNum[cntRand++] % 200);
		newBullet.vy = -1 * (randNum[cntRand++] % 200 + 1);
		newBullets.push_back(newBullet);
	}*/
	NewBullet newBullet;
	newBullet.initTime = gameInfo.round + 1;
	newBullet.x = 300;
	newBullet.y = 600;
	newBullet.vx = 0;
	newBullet.vy = -20;
	newBullets.push_back(newBullet);
}

