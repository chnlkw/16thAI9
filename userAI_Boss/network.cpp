#include "network.h"

void sendString(QTcpSocket* socket, const QString& v) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << v;
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvString(QTcpSocket *socket, QString &v) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    in >> v;
}

void sendInt(QTcpSocket *socket, const int &v) {
    QDataStream out(socket);
    out << (qint32)v;
    socket->waitForBytesWritten(-1);
}

void recvInt(QTcpSocket *socket, int &v) {
    QDataStream in(socket);
    while (socket->bytesAvailable() < (int)sizeof(qint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    in >> v;
}

void sendBossActions(QTcpSocket *socket, const vector<NewBullet> &newBullets) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)newBullets.size();
    for (int i = 0; i < newBullets.size(); i ++) {
        out << newBullets[i].initTime;
        out << newBullets[i].vx;
        out << newBullets[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void sendBossActions(QTcpSocket *socket, const vector<NewBullet> &newBullets, int start, int end) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)(end - start);
    for (int i = start; i < end; i ++) {
        out << newBullets[i].initTime;
        out << newBullets[i].vx;
        out << newBullets[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvBossActions(QTcpSocket *socket, vector<NewBullet> &newBullets) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        NewBullet newBullet;
        in >> newBullet.initTime;
        in >> newBullet.vx;
        in >> newBullet.vy;
        newBullets.push_back(newBullet);
    }
}

void sendPlaneActions(QTcpSocket *socket, const vector<PlaneAction> &planeActions) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)planeActions.size();
    for (int i = 0; i < planeActions.size(); i ++) {
        out << planeActions[i].startTime;
        out << planeActions[i].endTime;
        out << planeActions[i].dx;
        out << planeActions[i].dy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void sendPlaneActions(QTcpSocket *socket, const vector<PlaneAction> &planeActions, int start, int end) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)(end - start);
    for (int i = start; i < end; i ++) {
        out << planeActions[i].startTime;
        out << planeActions[i].endTime;
        out << planeActions[i].dx;
        out << planeActions[i].dy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvPlaneActions(QTcpSocket *socket, vector<PlaneAction> &planeActions) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        PlaneAction planeAction;
        in >> planeAction.startTime;
        in >> planeAction.endTime;
        in >> planeAction.dx;
        in >> planeAction.dy;
        planeActions.push_back(planeAction);
    }
}

void sendGameInfo(QTcpSocket *socket, const GameInfo &gameInfo) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << gameInfo.round;
    out << gameInfo.score;
    out << (int)gameInfo.gameStatus;
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvGameInfo(QTcpSocket *socket, GameInfo &gameInfo) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    in >> gameInfo.round;
    in >> gameInfo.score;
    int gameStatus;
    in >> gameStatus;
    gameInfo.gameStatus = (GAME_STATUS)gameStatus;
}
