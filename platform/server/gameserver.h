#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTimer>
#include "serverreceiverthread.h"
#include "serversenderthread.h"

class GameServer : public QTcpServer {
    Q_OBJECT
public:
    GameServer();
    void run();

private:
    void shakeHands();
    void calc();
    void genRep();
    bool isValidNewBullet(const NewBullet& bullet);
    bool isValidPlaneAction(const PlaneAction& action);

    FILE* repFile;

    Player players[2];
    GameInfo gameInfo;

    int cntNewBulletsNum, cntPlaneActionsNum;

    vector<NewBullet> recvNewBullets, newBullets;
    vector<PlaneAction> recvPlaneActions, planeActions;

    ServerReceiverThread *bossRecvThread, *planeRecvThread;
    ServerSenderThread *bossSendThread, *planeSendThread;

};

#endif // GAMESERVER_H
