#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QLibrary>
#include "common.h"
#include "network.h"
#include "clientreceiverthread.h"

typedef void (*funcInit) (string&);
typedef void (*funcGetAction) (const GameInfo& gameInfo, vector<Move>& moves, vector<Skill>& skills, string& msg);

class GameClient {
public:
    GameClient(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType, char* aiFile);
    void run();
private:
    QHostAddress serverAddr;
    quint16 serverPort;
    CLIENT_TYPE clientType;

    QTcpSocket* sendSocket;
    ClientReceiverThread* recvThread;

    GameInfo recvGameInfo;
    GameInfo gameInfo;

    funcInit callInit;
    funcGetAction callGetAction;

    QString aiName;

    void init();
    void shakeHands();
    void getActions(vector<Move>& moves, vector<Skill>& skills, string& msg);
};

#endif // GAMECLIENT_H
