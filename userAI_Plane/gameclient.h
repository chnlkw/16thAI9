#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "common.h"
#include "network.h"
#include "clientreceiverthread.h"

class GameClient {
public:
    GameClient(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType);
    void run();
private:
    QHostAddress serverAddr;
    quint16 serverPort;
    CLIENT_TYPE clientType;

    QTcpSocket* sendSocket;
    ClientReceiverThread* recvThread;

    GameInfo recvGameInfo;
    vector<NewBullet> recvNewBullets;
    vector<PlaneAction> recvPlaneActions;

    void shakeHands();
    void update();
    void getActions(vector<PlaneAction>& planeActions);
};

#endif // GAMECLIENT_H
