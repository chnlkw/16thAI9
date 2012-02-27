#include "gameclient.h"

GameClient::GameClient(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType, char* aiFile) {
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
    QLibrary aiDll(aiFile);
    if (aiDll.load()) {
        callInit = (funcInit)aiDll.resolve("init");
        callGetAction = (funcGetAction)aiDll.resolve("getAction");
        if (!callInit || !callGetAction) {
            cout << "failed resolve" << endl;
        }
    } else {
        cout << "failed load" << endl;
    }
    init();
}

void GameClient::run() {
    shakeHands();

    int prevRound = -1;

    while (true) {
        Timer::msleep(1);
        if (recvGameInfo.gameStatus == BOSS_WIN || recvGameInfo.gameStatus == PLANE_WIN) break;
        if (recvGameInfo.gameStatus != BATTLE) continue;
        if (prevRound == recvGameInfo.round) continue;
        prevRound = recvGameInfo.round;
        update();
        vector<PlaneAction> planeActions;
        string msg;
        getActions(planeActions, msg);
        sendString(sendSocket, QString("actions"));
        sendString(sendSocket, QString(msg.c_str()));
        sendPlaneActions(sendSocket, planeActions);
    }

    // The last action, send a string 'close'.
    sendString(sendSocket, QString("close"));
    sendSocket->disconnectFromHost();
}

void GameClient::init() {
    string buf;
    callInit(buf);
    aiName = QString(buf.c_str());
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
    sendString(sendSocket, aiName);
    recvString(sendSocket, s);
    assert(s == "shake hand over");

    cout << "plane sender shake hand over" << endl;

    recvThread = new ClientReceiverThread(serverAddr, serverPort, clientType, &recvGameInfo,
                                          &recvNewBullets, &recvPlaneActions);

    recvThread->start();
}

void GameClient::update() {
    updateGameInfo(recvGameInfo, recvNewBullets, recvPlaneActions);
}

void GameClient::getActions(vector<PlaneAction> &planeActions, string& msg) {
    callGetAction(recvGameInfo, planeActions, msg);
}

