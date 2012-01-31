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
    socket->waitForReadyRead(-1);
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32));
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize);
    in >> v;
}

void sendInt(QTcpSocket *socket, const int &v) {
    QDataStream out(socket);
    out << (qint32)v;
    socket->waitForBytesWritten(-1);
}

void recvInt(QTcpSocket *socket, int &v) {
    socket->waitForReadyRead(-1);
    QDataStream in(socket);
    while (socket->bytesAvailable() < (int)sizeof(qint32));
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
        out << newBullets[i].x;
        out << newBullets[i].y;
        out << newBullets[i].vx;
        out << newBullets[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvBossActions(QTcpSocket *socket, vector<NewBullet> &newBullets) {
    socket->waitForReadyRead(-1);
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32));
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize);
    quint32 size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        NewBullet newBullet;
        in >> newBullet.initTime;
        in >> newBullet.x;
        in >> newBullet.y;
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

void recvPlaneActions(QTcpSocket *socket, vector<PlaneAction> &planeActions) {
    socket->waitForReadyRead(-1);
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32));
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize);
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
    socket->waitForReadyRead(-1);
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32));
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize);
    in >> gameInfo.round;
    in >> gameInfo.score;
    int gameStatus;
    in >> gameStatus;
    gameInfo.gameStatus = (GAME_STATUS)gameStatus;
}
