#include "clientreceiverthread.h"

ClientReceiverThread::ClientReceiverThread(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType,
                                           GameInfo* gameInfo,
                                           vector<NewBullet>* newBullets, vector<PlaneAction>* planeActions,
                                           QObject *parent) :
    QThread(parent)
{
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
    this->gameInfo = gameInfo;
    this->newBullets = newBullets;
    this->planeActions = planeActions;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void ClientReceiverThread::run() {
    shakeHands();
    while (true) {
        QString serverStatus;
        recvString(recvSocket, serverStatus);
        cout << "plane recv " << serverStatus.toStdString() << " ";
        if (serverStatus == "close") break;
        assert(serverStatus == "actions");
        recvGameInfo(recvSocket, *gameInfo);
        cout << gameInfo->round << "," << (int)gameInfo->gameStatus << endl;
        recvBossActions(recvSocket, *newBullets);
        recvPlaneActions(recvSocket, *planeActions);
    }
    recvSocket->disconnectFromHost();
}

void ClientReceiverThread::shakeHands() {
    recvSocket = new QTcpSocket();
    recvSocket->connectToHost(serverAddr, serverPort);

    QString s;
    recvString(recvSocket, s);
    assert(s == "accepted");
    sendString(recvSocket, "client recver");
    sendInt(recvSocket, (int)clientType);
    recvString(recvSocket, s);
    assert(s == "shake hand over");

    cout << "plane recver shake hand over" << endl;
}
