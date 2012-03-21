#include "aiPlane.h"
using namespace std;
	
/************************************************************************
 * 初始化函数：
 *     参数：
 *         name: Plane的名字
 *     说明：
 *         指定Plane的名字，做一些初始化工作。
 ***********************************************************************/        
void init(string& name) {
	name = "plane";
}

/************************************************************************
 * 指定Plane动作的函数：
 *     参数：
 *         gameInfo: 当前游戏局面信息
 *         moves:    Plane要完成的移动
 *         skills:   Plane要使用的技能
 *         msg:      Plane要说的话，可以包括一些调试信息，会输出到录像中
 *     说明：
 *         根据当前游戏局面信息，指定Plane要做出的移动和使用的技能。
 ***********************************************************************/        
void getAction(const GameInfo& gameInfo, vector<Move>& moves, vector<Skill>& skills, string& msg) {
	msg = "BlahBlah";
}

