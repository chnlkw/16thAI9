#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTimer>
#include "serverreceiverthread.h"

class GameServer : public QTcpServer {
    Q_OBJECT
public:
    GameServer();
    void run();

private:
    void shakeHands();
    void calc();
    void genRep(int move);
    void send(QTcpSocket* bossSocket, QTcpSocket* planeSocket);
    bool isValidNewBullet(const NewBullet& bullet);
    bool isValidPlaneAction(const PlaneAction& action);

    FILE* repFile;

    Player players[2];
    GameInfo gameInfo;

    int cntRecvNewBulletsNum, cntRecvPlaneActionsNum;
    int cntSendNewBulletsNum, cntSendPlaneActionsNum;
    QTcpSocket* bossSendSocket, *planeSendSocket;

    vector<NewBullet> recvNewBullets, newBullets;
    vector<PlaneAction> recvPlaneActions, planeActions;
    QString recvBossMsg, recvPlaneMsg;

    ServerReceiverThread *bossRecvThread, *planeRecvThread;

};

#endif // GAMESERVER_H
