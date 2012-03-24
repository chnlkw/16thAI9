#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>

void sendString(QTcpSocket* socket, const QString& v);
bool recvString(QTcpSocket* socket, QString& v, int mode = 0);

void sendInt(QTcpSocket* socket, const int& v);
void recvInt(QTcpSocket* socket, int& v);

#endif // NETWORK_H
