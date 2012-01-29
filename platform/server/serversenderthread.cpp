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

void ServerSenderThread::send(int round) {
    cout << "round: " << round << " server send to " << (int)clientType << endl;
    sendString(socket, QString("actions"));
    sendBossActions(socket, *newBullets);
    sendPlaneActions(socket, *planeActions);
}
