#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QString>

class GameClient {
public:
    GameClient(QString serverHost, quint16 serverPort);
    void run();
private:
    QString serverHost;
    quint16 serverPort;
};

#endif // GAMECLIENT_H
