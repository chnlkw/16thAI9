#include "clientreceiverthread.h"

ClientReceiverThread::ClientReceiverThread(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType,
                                           vector<NewBullet>* newBullets, vector<PlaneAction>* planeActions,
                                           QObject *parent) :
    QThread(parent)
{
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
    this->newBullets = newBullets;
    this->planeActions = planeActions;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void ClientReceiverThread::run() {
    shakeHands();
    while (true) {
        QString serverStatus;
        recvString(recvSocket, serverStatus);
        cout << "boss recv " << serverStatus.toStdString() << endl;
        if (serverStatus == "close") break;
        assert(serverStatus == "actions");
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

    cout << "boss recver shake hand over" << endl;
}
