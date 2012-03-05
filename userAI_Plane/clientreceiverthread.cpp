#include "clientreceiverthread.h"

ClientReceiverThread::ClientReceiverThread(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType,
                                           GameInfo* gameInfo,
                                           QObject *parent) :
    QThread(parent)
{
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
    this->gameInfo = gameInfo;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void ClientReceiverThread::run() {
    shakeHands();
    while (true) {
        QString serverStatus;
        recvString(recvSocket, serverStatus);
        if (serverStatus == "close") break;
        assert(serverStatus == "actions");
        recvGameInfo(recvSocket, *gameInfo);
    }
    recvSocket->disconnectFromHost();
}

void ClientReceiverThread::shakeHands() {
    recvSocket = new QTcpSocket();
    recvSocket->connectToHost(serverAddr, serverPort);
    recvSocket->waitForConnected();

    QString s;
    recvString(recvSocket, s);
    assert(s == "accepted");
    sendString(recvSocket, "client recver");
    sendInt(recvSocket, (int)clientType);
    recvString(recvSocket, s);
    assert(s == "shake hand over");
}
