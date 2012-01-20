#include "gameclient.h"

GameClient::GameClient(QString serverHost, quint16 serverPort) {
    this->serverHost = serverHost;
    this->serverPort = serverPort;
}

void GameClient::run() {
    ReceiverThread receiverThread(serverHost, serverPort, &gameStatus);
    receiverThread.start();

    QTcpSocket sendSocket;
    sendSocket.connectToHost(serverHost, serverPort);

    while (true) {
        if (gameStatus.status != BATTLE) break;

        /**
        ** PlayerAI starts from here.
        ** input: gameStats
        ** output:
        **   Boss: vector<NewBullet> newBullets
        **   Plane: vector<PlaneAction> planeActions
        **/

        // Here is just a naive strategy.

        // Boss
        NewBullet newBullet;
        newBullet.initTime = gameStatus.time + 1;
        newBullet.initCoor.x = 0;
        newBullet.initCoor.y = 0;
        newBullet.vx = 1;
        newBullet.vy = 1;
        vector<NewBullet> newBullets;
        newBullets.push_back(newBullet);

        // Plane
        PlaneAction planeAction;
        planeAction.startTime = gameStatus.time + 1;
        planeAction.endTime = gameStatus.time + 10;
        planeAction.dx = 1;
        planeAction.dy = 1;
        vector<PlaneAction> planeActions;
        planeActions.push_back(planeAction);

        // Send the strategy to server.
        sendBossAction(sendSocket, newBullets);
        sendPlaneAction(sendSocket, planeActions);
    }

    sendSocket.disconnectFromHost();
}

void GameClient::sendBossAction(QTcpSocket& sendSocket, const vector<NewBullet>& newBullets) {
    QDataStream out(&sendSocket);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)newBullets.size();
    for (int i = 0; i < newBullets.size(); i ++) {
        out << newBullets[i].initTime;
        out << newBullets[i].initCoor.x;
        out << newBullets[i].initCoor.y;
        out << newBullets[i].vx;
        out << newBullets[i].vy;
    }
}

void GameClient::sendPlaneAction(QTcpSocket& sendSocket, const vector<PlaneAction>& planeActions) {
    QDataStream out(&sendSocket);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)planeActions.size();
    for (int i = 0; i < planeActions.size(); i ++) {
        out << planeActions[i].startTime;
        out << planeActions[i].endTime;
        out << planeActions[i].dx;
        out << planeActions[i].dy;
    }
}

