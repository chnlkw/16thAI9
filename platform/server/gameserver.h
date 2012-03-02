#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTimer>
#include "serverreceiverthread.h"

class GameServer : public QTcpServer {
    Q_OBJECT
public:
    GameServer(int gui);
    void run();

private:
    void shakeHands();
    void send();
    void recv();
    void judge(const GameInfo& cntGameInfo, const vector<int>& newBulletsId);
    void genRep(const GameInfo& cntGameInfo, const vector<int>& newBulletsId);
    void updateGameInfo(vector<int>& newBulletsId);
    bool isValidNewBullet(const NewBullet& bullet);
    bool isValidMove(const Move& move);
    bool isValidSkill(const Skill& skill);

    FILE* repFile;

    Player players[2];
    GameInfo gameInfo;
    int lastSpeedup;
    bool useBomb;
    int hasGui;

    int cntRecvNewBulletsNum, cntRecvMovesNum, cntRecvSkillsNum;
    QTcpSocket* bossSendSocket, *planeSendSocket, *guiSendSocket;

    vector<NewBullet> recvNewBullets, newBullets;
    vector<Move> recvMoves, moves;
    vector<Skill> recvSkills, skills;
    QString recvBossMsg, recvPlaneMsg;

    ServerReceiverThread *bossRecvThread, *planeRecvThread;

};

#endif // GAMESERVER_H
