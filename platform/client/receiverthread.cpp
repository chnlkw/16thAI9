#include "receiverthread.h"

ReceiverThread::ReceiverThread(QString serverHost, quint16 serverPort, GameStatus* gameStatus) {
    this->serverHost = serverHost;
    this->serverPort = serverPort;
    this->gameStatus = gameStatus;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void ReceiverThread::run() {
    recvSocket = new QTcpSocket(this);
    connect(recvSocket, SIGNAL(readyRead()), this, SLOT(receiveData));
    recvSocket->connectToHost(serverHost, serverPort);
}

void ReceiverThread::receiveData() {
    QDataStream in(recvSocket);
    in.setVersion(QDataStream::Qt_4_0);
    // receive gameStatus
    // 0. status
    int tmp;
    in >> tmp;
    gameStatus->status = (GAME_STATUS)tmp;
    if (gameStatus->status != BATTLE) {
        recvSocket->disconnectFromHost();
        return;
    }
    // 1. time
    in >> gameStatus->time;
    // 2. planePosition
    in >> gameStatus->planePosition.x;
    in >> gameStatus->planePosition.y;
    // 3. bullets
    gameStatus->bullets.clear();
    int size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        Bullet bullet;
        in >> bullet.cntCoor.x;
        in >> bullet.cntCoor.y;
        in >> bullet.vx;
        in >> bullet.vy;
        gameStatus->bullets.push_back(bullet);
    }
}
