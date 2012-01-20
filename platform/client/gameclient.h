#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QString>
#include "receiverthread.h"
#include "common/common.h"

class GameClient {
public:
    GameClient(QString serverHost, quint16 serverPort);
    void run();
private:
    GameStatus gameStatus;
    QString serverHost;
    quint16 serverPort;

    void sendBossAction(QTcpSocket& sendSocket, const vector<NewBullet>& newBullets);
    void sendPlaneAction(QTcpSocket& sendSocket, const vector<PlaneAction>& planeActions);
};

#endif // GAMECLIENT_H
