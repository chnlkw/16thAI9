#include "receiverthread.h"

ReceiverThread::ReceiverThread(QString serverHost, quint16 serverPort) {
    this->serverHost = serverHost;
    this->serverPort = serverPort;
}

void ReceiverThread::run() {
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData));
    tcpSocket->connectToHost(serverHost, serverPort);
}

void ReceiverThread::receiveData() {
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
}
