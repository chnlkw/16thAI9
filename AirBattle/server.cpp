#include "server.h"

Server::Server()
{
    socket = NULL;
    qDebug()<<"thread init: "<<QThread::currentThreadId();
}

void Server::shakeHands()
{
    QString s;
    recvString(socket, s);
    sendString(socket, "gui");
    recvString(socket, s);
}

void Server::reset()
{
    qDebug()<<"thread reset: "<<QThread::currentThreadId();

    delete socket;

    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress("127.0.0.1"), 20129);
    socket->waitForConnected();

    shakeHands();
    int round, score;
    QString serverStatus;
    while (true)
    {
        recvString(socket, serverStatus);
        if (serverStatus == "close") break;
        recvInt(socket, round);
        recvInt(socket, score);
        emit change(round, score);
    }
    socket->disconnectFromHost();
    emit change(0, 0);
}
