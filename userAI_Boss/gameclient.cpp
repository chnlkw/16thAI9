#include "gameclient.h"

GameClient::GameClient(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType) {
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
}

void GameClient::run() {
    shakeHands();

    int prevRound = -1;

    while (true) {
        if (recvGameInfo.gameStatus == BOSS_WIN || recvGameInfo.gameStatus == PLANE_WIN) break;
        if (recvGameInfo.gameStatus != BATTLE) continue;
        if (prevRound == recvGameInfo.round) continue;
        prevRound = recvGameInfo.round;
        update();
        vector<NewBullet> newBullets;
        getActions(newBullets);
        sendString(sendSocket, QString("actions"));
        sendBossActions(sendSocket, newBullets);
    }

    // The last action, send a string 'close'.
    sendString(sendSocket, QString("close"));
    sendSocket->disconnectFromHost();
}

void GameClient::shakeHands() {
    sendSocket = new QTcpSocket();
    sendSocket->connectToHost(serverAddr, serverPort);
    sendSocket->waitForConnected();

    QString s;
    recvString(sendSocket, s);
    assert(s == "accepted");
    sendString(sendSocket, "client sender");
    sendInt(sendSocket, (int)clientType);
    sendString(sendSocket, QString("Boss"));
    recvString(sendSocket, s);
    assert(s == "shake hand over");

    cout << "boss sender shake hand over" << endl;

    recvThread = new ClientReceiverThread(serverAddr, serverPort, clientType, &recvGameInfo,
                                          &recvNewBullets, &recvPlaneActions);
    recvThread->start();
}

void GameClient::update() {
    updateGameInfo(recvGameInfo, recvNewBullets, recvPlaneActions);
}

void GameClient::getActions(vector<NewBullet> &newBullets) {
    // AI
    for (int i = 0; i < 2; i ++) {
        NewBullet newBullet;
        newBullet.initTime = i * 10;
        newBullet.x = i;
        newBullet.y = i;
        newBullet.vx = i + 1;
        newBullet.vy = i + 1;
        newBullets.push_back(newBullet);
    }
}
