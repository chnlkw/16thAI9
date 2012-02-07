#include "serverreceiverthread.h"

ServerReceiverThread::ServerReceiverThread(int socketDescriptor, vector<NewBullet>* newBullets, QObject *parent) :
    QThread(parent)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    this->newBullets = newBullets;
    clientType = BOSS;
}

ServerReceiverThread::ServerReceiverThread(int socketDescriptor, vector<PlaneAction>* planeActions, QObject *parent) :
    QThread(parent)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    this->planeActions = planeActions;
    clientType = PLANE;
}

void ServerReceiverThread::run() {
    while (true) {
        QString clientStatus;
        recvString(socket, clientStatus);
        if (clientStatus == "close") break;

        if (clientType == BOSS) {
            recvBossActions(socket, *newBullets);
            NewBullet& bullet = (*newBullets)[0];
//            cout << "recv bullet " << bullet.initTime << " " << bullet.x << "," << bullet.y <<
//                    " " << bullet.vx << "," << bullet.vy << endl;
        } else {
            recvPlaneActions(socket, *planeActions);
            PlaneAction& act = (*planeActions)[0];
//            cout << "recv act " << act.startTime << "," << act.endTime <<
//                    " " << act.dx << "," << act.dy << endl;
        }
    }
}



