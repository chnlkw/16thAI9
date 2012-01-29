#include "gameserver.h"

GameServer::GameServer() {
}

void GameServer::run() {
    this->listen(SERVER_ADDR, SERVER_PORT);

    shakeHands();
    cout << "shake hands over" << endl;

    for (int i = 0; i < 3000; i ++) {
        ServerTimer::msleep(1000);
        calc(i);
        bossSendThread->send(i);
        planeSendThread->send(i);
    }
}

void GameServer::shakeHands() {
    for (int i = 0; i < 4; i ++) {
        this->waitForNewConnection(-1);
        QTcpSocket* socket = this->nextPendingConnection();
        sendString(socket, QString("accepted"));

        QString sr;
        recvString(socket, sr);
        int clientType;
        recvInt(socket, clientType);
        players[clientType].socketDescriptor = socket->socketDescriptor();
        if (sr == "client sender") {            
            recvString(socket, players[clientType].name);
            if ((CLIENT_TYPE)clientType == BOSS) {
                cout << "boss sender" << endl;
                bossRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &newBullets, this);
                connect(bossRecvThread, SIGNAL(finished()), bossRecvThread, SLOT(deleteLater()));
                bossRecvThread->start();
                sendString(socket, QString("shake hand over"));
            } else {
                cout << "plane sender" << endl;
                planeRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &planeActions, this);
                connect(planeRecvThread, SIGNAL(finished()), planeRecvThread, SLOT(deleteLater()));
                planeRecvThread->start();
                sendString(socket, QString("shake hand over"));
            }
        } else if (sr == "client recver") {
            if ((CLIENT_TYPE)clientType == BOSS) {
                cout << "boss recver" << endl;
                bossSendThread = new ServerSenderThread(socket->socketDescriptor(), (CLIENT_TYPE)clientType,
                                               &newBullets, &planeActions, this);
                connect(bossSendThread, SIGNAL(finished()), bossSendThread, SLOT(deleteLater()));
                bossSendThread->start();
                sendString(socket, QString("shake hand over"));
            } else {
                cout << "plane recver" << endl;
                planeSendThread = new ServerSenderThread(socket->socketDescriptor(), (CLIENT_TYPE)clientType,
                                               &newBullets, &planeActions, this);
                connect(planeSendThread, SIGNAL(finished()), planeSendThread, SLOT(deleteLater()));
                planeSendThread->start();
                sendString(socket, QString("shake hand over"));
            }
        }
    }
}

/** Here is the server's game logic part.
  * The ais' actions will be received and placed into newBullets, planeActions.
  * This function will be called every 100ms.
  */
void GameServer::calc(int round) {
    cout << "server calculate round = " << round << endl;
}
