#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"
#include <QTcpSocket>

void sendString(QTcpSocket* socket, const QString& v);
void recvString(QTcpSocket* socket, QString& v);

void sendInt(QTcpSocket* socket, const int& v);
void recvInt(QTcpSocket* socket, int& v);

void sendBossActions(QTcpSocket* socket, const vector<NewBullet>& newBullets);
void recvBossActions(QTcpSocket* socket, vector<NewBullet>& newBullets);

void sendPlaneActions(QTcpSocket* socket, const vector<PlaneAction>& planeActions);
void recvPlaneActions(QTcpSocket* socket, vector<PlaneAction>& planeActions);

#endif // NETWORK_H
