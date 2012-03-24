#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"
#include <QTcpSocket>

void sendString(QTcpSocket* socket, const QString& v);
bool recvString(QTcpSocket* socket, QString& v, int mode = 0);

void sendInt(QTcpSocket* socket, const int& v);
void recvInt(QTcpSocket* socket, int& v);

void sendBossActions(QTcpSocket* socket, const vector<NewBullet>& newBullets);
void sendBossActions(QTcpSocket* socket, const vector<NewBullet>& newBullets, int start, int end);
void recvBossActions(QTcpSocket* socket, vector<NewBullet>& newBullets);

void sendMoves(QTcpSocket* socket, const vector<Move>& moves);
void sendMoves(QTcpSocket* socket, const vector<Move>& moves, int start, int end);
void recvMoves(QTcpSocket* socket, vector<Move>& moves);

void sendSkills(QTcpSocket* socket, const vector<Skill>& skills);
void sendSkills(QTcpSocket* socket, const vector<Skill>& skills, int start, int end);
void recvSkills(QTcpSocket* socket, vector<Skill>& skills);

void sendGameInfo(QTcpSocket* socket, const GameInfo& gameInfo);
void recvGameInfo(QTcpSocket* socket, GameInfo& gameInfo);

#endif // NETWORK_H
