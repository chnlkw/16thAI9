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
                                  vector<NewBullet>* newBullets, vector<PlaneAction>* planeActions,
                                  QObject *parent = 0);
    void run();

signals:

public slots:

private:
    QHostAddress serverAddr;
    quint16 serverPort;
    CLIENT_TYPE clientType;

    vector<NewBullet>* newBullets;
    vector<PlaneAction>* planeActions;

    QTcpSocket* recvSocket;

    void shakeHands();
};

#endif // CLIENTRECEIVERTHREAD_H
