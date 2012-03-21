#include "aiPlane.h"
using namespace std;

#define SQR(a) ((a) * (a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
	
void init(string& name)
{
	srand(time(0));
	name = "plane";
}

// 获得子弹类型的函数。
int getBulletType(double vx, double vy) {
    for (int i = 0; i < 5; i ++)
        if (fabs(SQR(vx)+SQR(vy) - SQR(BULLET_V[i])) < EPSILON)
            return i;
    return -1;
}

// 判断在接下来的一个回合内，Plane是否会被击中。
// 这里是非常精确的判断，用到了一些初、高中几何学的知识。
bool judgeHit(const vector<Bullet>& bullets, double planeX, double planeY, double planeVx, double planeVy) {
	double xp = planeX, yp = planeY;
    double dx = planeVx, dy = planeVy;
    
    for (int i = 0; i < bullets.size(); i ++) {
        const Bullet& bullet = bullets[i];
        double xb = bullet.x, yb = bullet.y;
        double vx = bullet.vx, vy = bullet.vy;
        double a = SQR(vx - dx) + SQR(vy - dy);
        double b = 2*((xb - xp)*(vx - dx) + (yb - yp)*(vy - dy));
        int type = getBulletType(bullet.vx, bullet.vy);
        double c = SQR(xb - xp) + SQR(yb - yp) - SQR(BULLET_R[type]);
        double min = MIN(a + b + c, c);
        double t = -b / (2*a);
        if (0 <= t && t <= 1)
            min = MIN(min, a*t*t + b*t + c);
        if (min <= 0) return true;
    }
    
    return false;
}

void getAction(const GameInfo& gameInfo, vector<Move>& moves, vector<Skill>& skills, string& msg) {
		
	// 一些调试信息。
	char bufRound[1024];
	sprintf(bufRound, "%d %d %lf %lf ", gameInfo.round, gameInfo.bullets.size(), gameInfo.planeX, gameInfo.planeY);
	msg = bufRound;
	msg += " ";
		
	double xp = gameInfo.planeX, yp = gameInfo.planeY;
	
	// 如果停住不动就可以躲过这一回合的子弹，那么就停住。
	if (!judgeHit(gameInfo.bullets, xp, yp, 0, 0)) {
		msg += "stay";
		return;
	}
		
	// 随机试100个方向，看看能否躲过去。
	for (int i = 0; i < 100; i ++) {
		// 随机一个速度大小。
		int v = rand() % ((int)(PLANE_V)) + 1;
		
		// 如果有“加速”可用，那么就用。
		if (gameInfo.planeSkillsNum[0] > 0) v = (int)(PLANE_V * 2);
			
		// 随机一个移动的方向。
		int t = rand() % 2;
		int p = (t == 1 ? 1 : -1);
		double vx = p * (rand() % 100);
		t = rand() % 2;
		p = (t == 1 ? 1 : -1);
		double vy = p * (rand() % 100);
		double r = sqrt(vx*vx + vy*vy);
		vx = v * vx / r;
		vy = v * vy / r;
		
		if (0 <= xp + vx && xp + vx <= WIDTH &&
			0 <= yp + vy && yp + vy <= HEIGHT) {
			if (!judgeHit(gameInfo.bullets, xp, yp, vx, vy)) {
				if (v == (int)(PLANE_V * 2)) {
					// 如果使用了“加速”技能，那么需要在skills中添加一下。
					Skill skill;
					skill.startTime = gameInfo.round;
					skill.type = SPEEDUP;
					skills.push_back(skill);	
				}
				
				// 在moves中添加这个移动。
				Move move;
				move.vx = vx;
				move.vy = vy;
				move.startTime = gameInfo.round;
				move.endTime = gameInfo.round + 1;
				moves.push_back(move);
				
				// 一些调试信息。
				char bufMove[1024];
				sprintf(bufMove, "(%lf, %lf) ", vx, vy);
				msg += bufMove;
				msg += "move";
				return;
			}
		}
	}
	
	// 如果都躲不过去，那么看看有没有“清屏”大招可用。
	if (gameInfo.planeSkillsNum[1] > 0) {
		Skill skill;
		skill.startTime = gameInfo.round;
		skill.type = BOMB;
		skills.push_back(skill);
		msg += " bomb";
		return;
	}
	
	msg += "hit";
}

