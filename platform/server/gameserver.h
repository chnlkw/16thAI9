#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTimer>
#include "serverreceiverthread.h"

class GameServer : public QTcpServer {
    Q_OBJECT
public:
    GameServer(int gui, char* repFileName, int totRounds, int sleepTime);
    void run();

private:
    void shakeHands();
    void send();
    void recv();
    void judge(const GameInfo& cntGameInfo, const vector<NewBullet>& validNewBullets);
    void genRep(const GameInfo& cntGameInfo, const vector<NewBullet>& validNewBullets);
    void updateGameInfo(vector<NewBullet>& validNewBullets);
    int getBulletType(double vx, double vy);
    bool isValidNewBullet(const NewBullet& bullet);
    bool isValidMove(const Move& move);
    bool isValidSkill(const Skill& skill);

    int gui;
    char* repFileName;
    int totRounds;
    int sleepTime;

    FILE* repFile;

    Player players[2];
    GameInfo gameInfo;
    int lastSpeedup;
    bool useBomb;
    double cntMoveX, cntMoveY;

    int cntRecvNewBulletsNum, cntRecvMovesNum, cntRecvSkillsNum;
    QTcpSocket* bossSendSocket, *planeSendSocket, *guiSendSocket;

    vector<NewBullet> newBullets[3001][5];
    QString bossMsg, planeMsg;

    vector<NewBullet> recvNewBullets;
    vector<Move> recvMoves, moves;
    vector<Skill> recvSkills, skills;
    QString recvBossMsg, recvPlaneMsg;

    volatile int bossRecvFinish, planeRecvFinish;

    ServerReceiverThread *bossRecvThread, *planeRecvThread;

};

#endif // GAMESERVER_H
