#include "clientreceiverthread.h"

ClientReceiverThread::ClientReceiverThread(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType,
                                           GameInfo* gameInfo, volatile bool* recvOverFlag,
                                           QObject *parent) :
    QThread(parent)
{
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
    this->gameInfo = gameInfo;
    this->recvOverFlag = recvOverFlag;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void ClientReceiverThread::run() {
    shakeHands();
    while (true) {
        QString serverStatus;
        recvString(recvSocket, serverStatus);
        *recvOverFlag = false;
        if (serverStatus == "close") {
            *recvOverFlag = true;
            break;
        }
        assert(serverStatus == "actions");
        recvGameInfo(recvSocket, *gameInfo);
        *recvOverFlag = true;
        //printf("recv: %d %u\n", gameInfo->round, gameInfo->bullets.size());
    }
    recvSocket->disconnectFromHost();
}

void ClientReceiverThread::shakeHands() {
    //cout << "thread create" << endl;
    recvSocket = new QTcpSocket();
    recvSocket->connectToHost(serverAddr, serverPort);
    while (!recvSocket->waitForConnected(1000)) cout << "connect failed" << endl;

    //cout << "thread connect: " << recvSocket->state() << endl;
    QString s;
    recvString(recvSocket, s);
    //cout << "thread recv " << s.toStdString() << endl;
    assert(s == "accepted");
    sendString(recvSocket, "client recver");
    sendInt(recvSocket, (int)clientType);
    recvString(recvSocket, s);
    //cout << "thread recv " << s.toStdString() << endl;
    assert(s == "shake hand over");
}
