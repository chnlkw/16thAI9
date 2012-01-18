#include "gameclient.h"
#include "receiverthread.h"

GameClient::GameClient(QString serverHost, quint16 serverPort) {
    this->serverHost = serverHost;
    this->serverPort = serverPort;
}

void GameClient::run() {
    ReceiverThread receiverThread(serverHost, serverPort);
    receiverThread.start();
}
