#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include "network.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server();

signals:
    void change(int,int);

private slots:
    void reset();

private:
     QTcpSocket* socket;
     void shakeHands();
};

#endif // SERVER_H
