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
}

void ServerSenderThread::run() {
}

void ServerSenderThread::send(const GameInfo& gameInfo) {
    cout << "round: " << gameInfo.round << " server send to " << (int)clientType << endl;
    sendString(socket, QString("actions"));
    sendGameInfo(socket, gameInfo);
    sendBossActions(socket, *newBullets);
    sendPlaneActions(socket, *planeActions);
}
