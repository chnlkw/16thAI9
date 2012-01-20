#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "common/common.h"

class ReceiverThread : public QThread {
public:
    ReceiverThread(QString serverHost, quint16 serverPort, GameStatus* gameStatus);
    void run();
private:
    QString serverHost;
    quint16 serverPort;
    QTcpSocket* recvSocket;

    GameStatus* gameStatus;

private slots:
    void receiveData();
};

#endif // RECEIVERTHREAD_H
