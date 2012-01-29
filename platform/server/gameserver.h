#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <QTcpServer>
#include <QTimer>
#include "servertimer.h"
#include "serverreceiverthread.h"
#include "serversenderthread.h"

class GameServer : public QTcpServer {
    Q_OBJECT
public:
    GameServer();
    void run();

private:
    void shakeHands();
    void calc(int round);

    Player players[2];

    vector<NewBullet> newBullets;
    vector<PlaneAction> planeActions;

    ServerReceiverThread *bossRecvThread, *planeRecvThread;
    ServerSenderThread *bossSendThread, *planeSendThread;

};

#endif // GAMESERVER_H
