#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "../common/common.h"
#include "../common/network.h"

class ServerReceiverThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerReceiverThread(int socketDescriptor, vector<NewBullet>* newBullets, QObject *parent = 0);
    explicit ServerReceiverThread(int socketDescriptor, vector<PlaneAction>* planeActions, QObject *parent = 0);
    void run();

signals:

public slots:

private:
    CLIENT_TYPE clientType;
    QTcpSocket* socket;
    vector<NewBullet>* newBullets;
    vector<PlaneAction>* planeActions;

};

#endif // RECEIVERTHREAD_H
