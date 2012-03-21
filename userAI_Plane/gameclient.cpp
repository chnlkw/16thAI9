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

    while (true) {
        Timer::msleep(1);
        if (recvGameInfo.gameStatus == BOSS_WIN || recvGameInfo.gameStatus == PLANE_WIN) break;
        if (recvGameInfo.gameStatus != BATTLE) continue;
        if (gameInfo.round == recvGameInfo.round) continue;
        while (!recvOverFlag) continue;
        //printf("main: %d %d\n", recvGameInfo.round, recvGameInfo.bullets.size());
        gameInfo = recvGameInfo;
        vector<Move> moves;
        vector<Skill> skills;
        string msg;
        getActions(moves, skills, msg);
        sendString(sendSocket, QString("actions"));
        sendString(sendSocket, QString(msg.c_str()));
        sendMoves(sendSocket, moves);
        sendSkills(sendSocket, skills);
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
    //cout << "recv " << s.toStdString() << endl;
    assert(s == "accepted");
    sendString(sendSocket, "client sender");
    sendInt(sendSocket, (int)clientType);
    sendString(sendSocket, aiName);
    recvString(sendSocket, s);
    //cout << "recv " << s.toStdString() << endl;
    assert(s == "shake hand over");

    Timer::msleep(500);

    recvThread = new ClientReceiverThread(serverAddr, serverPort, clientType, &recvGameInfo, &recvOverFlag);
    recvThread->start();
}

void GameClient::getActions(vector<Move>& moves, vector<Skill>& skills, string& msg) {
    callGetAction(gameInfo, moves, skills, msg);
}

