#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>

void sendString(QTcpSocket* socket, const QString& v);
void recvString(QTcpSocket* socket, QString& v);

void sendInt(QTcpSocket* socket, const int& v);
void recvInt(QTcpSocket* socket, int& v);

#endif // NETWORK_H
