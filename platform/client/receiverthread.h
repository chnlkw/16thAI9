#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ReceiverThread : public QThread {
public:
    ReceiverThread(QString serverHost, quint16 serverPort);
    void run();
private:
    QString serverHost;
    quint16 serverPort;
    QTcpSocket* tcpSocket;

private slots:
    void receiveData();
};

#endif // RECEIVERTHREAD_H
