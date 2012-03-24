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

bool recvString(QTcpSocket *socket, QString &v, int mode) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    int count = 0;
    while (socket->bytesAvailable() < (int)sizeof(quint32)) {
        socket->waitForReadyRead(20);
        count ++;
        if (mode == 1 && count == 300) return false;
    }
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(20);
    in >> v;
    return true;
}

void sendInt(QTcpSocket *socket, const int &v) {
    QDataStream out(socket);
    out << (qint32)v;
    socket->waitForBytesWritten(-1);
}

void recvInt(QTcpSocket *socket, int &v) {
    QDataStream in(socket);
    while (socket->bytesAvailable() < (int)sizeof(qint32))
        socket->waitForReadyRead(20);
    in >> v;
}
