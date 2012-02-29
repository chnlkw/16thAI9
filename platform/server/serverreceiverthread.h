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
    explicit ServerReceiverThread(int socketDescriptor, vector<NewBullet>* newBullets, QString* msg, QObject *parent = 0);
    explicit ServerReceiverThread(int socketDescriptor, vector<Move>* moves, vector<Skill>* skills, QString* msg, QObject *parent = 0);
    void run();

signals:

public slots:

private:
    CLIENT_TYPE clientType;
    QTcpSocket* socket;
    vector<NewBullet>* newBullets;
    vector<Move>* moves;
    vector<Skill>* skills;
    QString* msg;

};

#endif // RECEIVERTHREAD_H
