#include "serverreceiverthread.h"

ServerReceiverThread::ServerReceiverThread(int socketDescriptor, vector<NewBullet>* newBullets, QString* msg, QObject *parent) :
    QThread(parent)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    this->newBullets = newBullets;
    this->msg = msg;
    clientType = BOSS;
}

ServerReceiverThread::ServerReceiverThread(int socketDescriptor, vector<Move>* moves, vector<Skill>* skills, QString* msg, QObject *parent) :
    QThread(parent)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    this->moves = moves;
    this->skills = skills;
    this->msg = msg;
    clientType = PLANE;
}

void ServerReceiverThread::run() {
    while (true) {
        QString clientStatus;
        recvString(socket, clientStatus);
        if (clientStatus == "close") break;
        recvString(socket, *msg);
        if (clientType == BOSS) {
            recvBossActions(socket, *newBullets);
        } else {
            recvMoves(socket, *moves);
            recvSkills(socket, *skills);
        }
    }
}



