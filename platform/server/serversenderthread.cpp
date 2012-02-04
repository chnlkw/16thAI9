#include "serversenderthread.h"

ServerSenderThread::ServerSenderThread(int socketDescriptor, CLIENT_TYPE clientType,
                                       vector<NewBullet>* newBullets, vector<PlaneAction>* planeActions, QObject *parent) :
    QThread(parent)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    this->newBullets = newBullets;
    this->planeActions = planeActions;
    this->clientType = clientType;
    cntNewBulletsNum = cntPlaneActionsNum = 0;
}

void ServerSenderThread::run() {
}

void ServerSenderThread::send(const GameInfo& gameInfo) {
    //cout << "round: " << gameInfo.round << " server send to " << (int)clientType << " ";
    //cout << gameInfo.round << "," << (int)gameInfo.gameStatus << endl;
    sendString(socket, QString("actions"));
    sendGameInfo(socket, gameInfo);
    int size = newBullets->size();
    sendBossActions(socket, *newBullets, cntNewBulletsNum, size);
    cntNewBulletsNum = size;
    size = planeActions->size();
    sendPlaneActions(socket, *planeActions, cntPlaneActionsNum, size);
    cntPlaneActionsNum = size;
}
