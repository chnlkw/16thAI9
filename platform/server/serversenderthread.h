#ifndef SERVERSENDERTHREAD_H
#define SERVERSENDERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "../common/common.h"
#include "../common/network.h"

class ServerSenderThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerSenderThread(int socketDescriptor, CLIENT_TYPE clientType,
                                vector<NewBullet>* newBullets, vector<PlaneAction>* planeActions, QObject *parent = 0);
    void run();
    void send(const GameInfo& gameInfo);

signals:

public slots:

private:
    CLIENT_TYPE clientType;
    QTcpSocket* socket;
    vector<NewBullet>* newBullets;
    vector<PlaneAction>* planeActions;

    int cntNewBulletsNum, cntPlaneActionsNum;
};

#endif // SERVERSENDERTHREAD_H
