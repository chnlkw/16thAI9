#include "gameclient.h"

GameClient::GameClient(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType) {
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
}

void GameClient::run() {
    shakeHands();

    while (true) {
        if (recvGameInfo.gameStatus == BOSS_WIN || recvGameInfo.gameStatus == PLANE_WIN) break;
        if (recvGameInfo.gameStatus != BATTLE) continue;
        update();
        vector<PlaneAction> planeActions;
        getActions(planeActions);
        sendString(sendSocket, QString("actions"));
        sendPlaneActions(sendSocket, planeActions);
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
    sendString(sendSocket, QString("Client"));
    recvString(sendSocket, s);
    assert(s == "shake hand over");

    cout << "plane sender shake hand over" << endl;

    recvThread = new ClientReceiverThread(serverAddr, serverPort, clientType, &recvGameInfo,
                                          &recvNewBullets, &recvPlaneActions);
    recvThread->start();
}

void GameClient::update() {
    // Calculate planeX, planeY
    double move[recvGameInfo.round][2];
    memset(move, 0, sizeof(move));
    for (int i = 0; i < recvPlaneActions.size(); i ++) {
        PlaneAction& act = recvPlaneActions[i];
        for (int t = act.startTime; t <= MIN(recvGameInfo.round - 1, act.endTime); t ++) {
            move[t][0] = act.dx;
            move[t][1] = act.dy;
        }
    }
    double planeX = PLANE_INIT_X, planeY = PLANE_INIT_Y;
    for (int t = 0; t < recvGameInfo.round; t ++) {
        planeX += move[t][0];
        planeY += move[t][1];
    }
    recvGameInfo.planeX = planeX;
    recvGameInfo.planeY = planeY;

    // Calculate bullets
    vector<Bullet> bullets;
    for (int i = 0; i < recvNewBullets.size(); i ++) {
        NewBullet& nb = recvNewBullets[i];
        if (nb.initTime > recvGameInfo.round ||
                (recvGameInfo.round - nb.initTime) * BULLET_V > sqrt(WIDTH*WIDTH + HEIGHT*HEIGHT)) continue;
        double x = nb.x, y = nb.y;
        x += nb.vx * (recvGameInfo.round - nb.initTime);
        y += nb.vy * (recvGameInfo.round - nb.initTime);
        if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
            Bullet bullet;
            bullet.x = x;
            bullet.y = y;
            bullet.vx = nb.vx;
            bullet.vy = nb.vy;
            bullets.push_back(bullet);
        }
    }
    recvGameInfo.bullets = bullets;
}

void GameClient::getActions(vector<PlaneAction> &planeActions) {
    // AI
    for (int i = 0; i < 2; i ++) {
        PlaneAction planeAction;
        planeAction.startTime = i * 10;
        planeAction.endTime = i * 10 + 5;
        planeAction.dx = i;
        planeAction.dy = i;
        planeActions.push_back(planeAction);
    }
}

