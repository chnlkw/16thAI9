#ifndef CLIENTRECEIVERTHREAD_H
#define CLIENTRECEIVERTHREAD_H

#include <QThread>
#include "common.h"
#include "network.h"

class ClientReceiverThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientReceiverThread(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType,
                                  GameInfo* gameInfo, volatile bool* recvOverFlag,
                                  QObject *parent = 0);
    void run();

signals:

public slots:

private:
    QHostAddress serverAddr;
    quint16 serverPort;
    CLIENT_TYPE clientType;

    volatile bool* recvOverFlag;
    GameInfo* gameInfo;

    QTcpSocket* recvSocket;

    void shakeHands();
};

#endif // CLIENTRECEIVERTHREAD_H
