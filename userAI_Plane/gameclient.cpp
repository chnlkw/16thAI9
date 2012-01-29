#include "gameclient.h"

GameClient::GameClient(QHostAddress serverAddr, quint16 serverPort, CLIENT_TYPE clientType) {
    this->serverAddr = serverAddr;
    this->serverPort = serverPort;
    this->clientType = clientType;
}

void GameClient::run() {
    shakeHands();

    /* AI starts from here.
     * It will get the game status from recvThread.
     * Do some calculations, and then send to server.
     * Here I just test for networks.
     */

    // The first actions, send a string 'actions', followed by newBullets.
    sendString(sendSocket, QString("actions"));
    vector<PlaneAction> planeActions;
    for (int i = 0; i < 2; i ++) {
        PlaneAction planeAction;
        planeAction.startTime = i * 10;
        planeAction.endTime = i * 10 + 5;
        planeAction.dx = i;
        planeAction.dy = i;
        planeActions.push_back(planeAction);
    }
    sendPlaneActions(sendSocket, planeActions);

    // The last action, send a string 'close'.
    sendString(sendSocket, QString("close"));

    sendSocket->disconnectFromHost();

    while (true);
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

    recvThread = new ClientReceiverThread(serverAddr, serverPort, clientType,
                                          &recvNewBullets, &recvPlaneActions);
    recvThread->start();
}

