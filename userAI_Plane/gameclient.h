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
    GameStatus gameStatus;
    QHostAddress serverAddr;
    quint16 serverPort;

    CLIENT_TYPE clientType;

    void shakeHands();
    QTcpSocket* sendSocket;    

    ClientReceiverThread* recvThread;

    vector<NewBullet> recvNewBullets;
    vector<PlaneAction> recvPlaneActions;
};

#endif // GAMECLIENT_H
