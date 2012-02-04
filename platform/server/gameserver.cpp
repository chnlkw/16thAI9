#include "gameserver.h"

GameServer::GameServer() {
    gameInfo.gameStatus = INIT;
    gameInfo.score = 0;
    gameInfo.planeX = PLANE_INIT_X;
    gameInfo.planeY = PLANE_INIT_Y;
    cntNewBulletsNum = cntPlaneActionsNum = 0;
    repFile = fopen("test.rep", "w");
    fprintf(repFile, "0\n");
    fprintf(repFile, "I'm Boss\n");
    fprintf(repFile, "I'm Player\n");
    fprintf(repFile, "300 600\n");
    fprintf(repFile, "300 200\n");
    fprintf(repFile, "\n");
}

void GameServer::run() {
    this->listen(SERVER_ADDR, SERVER_PORT);

    shakeHands();
    cout << "shake hands over" << endl;

    gameInfo.gameStatus = BATTLE;

    for (int i = 0; i < 3000; i ++) {
        ServerTimer::msleep(1000);
        gameInfo.round = i;
        calc();
        genRep();
        updateGameInfo(gameInfo, newBullets, planeActions);
        bossSendThread->send(gameInfo);
        planeSendThread->send(gameInfo);
        if (gameInfo.gameStatus != BATTLE) break;
    }

    fclose(repFile);
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
                bossRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvNewBullets, this);
                connect(bossRecvThread, SIGNAL(finished()), bossRecvThread, SLOT(deleteLater()));
                bossRecvThread->start();
                sendString(socket, QString("shake hand over"));
            } else {
                cout << "plane sender" << endl;
                planeRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvPlaneActions, this);
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

void GameServer::calc() {
    cout << "round = " << gameInfo.round << endl;
    cout << "plane = " << gameInfo.planeX << "," << gameInfo.planeY << endl;

    int size = recvNewBullets.size();
    for (int i = cntNewBulletsNum; i < size; i ++)
        if (isValidNewBullet(recvNewBullets[i]))
            newBullets.push_back(recvNewBullets[i]);
    cntNewBulletsNum = size;

    size = recvPlaneActions.size();
    cout << "recvPlaneActions.size = " << size << endl;
    for (int i = cntPlaneActionsNum; i < size; i ++) {
        cout << "recvPlaneActions " << i << ":" << endl;
        cout << recvPlaneActions[i].startTime << "," << recvPlaneActions[i].endTime << endl;
        cout << recvPlaneActions[i].dx << "," << recvPlaneActions[i].dy << endl;
        if (isValidPlaneAction(recvPlaneActions[i]))
            planeActions.push_back(recvPlaneActions[i]);
    }
    cntPlaneActionsNum = size;

    if (gameInfo.round > 0) {
        double xp = gameInfo.planeX, yp = gameInfo.planeY;
        double dx = 0, dy = 0;
        for (int i = 0; i < planeActions.size(); i ++) {
            PlaneAction& act = planeActions[i];
            if (act.startTime < gameInfo.round && act.endTime >= gameInfo.round) {
                dx = act.dx;
                dy = act.dy;
            }
        }

        bool hit = false;
        for (int i = 0; i < gameInfo.bullets.size(); i ++) {
            Bullet& bullet = gameInfo.bullets[i];
            double xb = bullet.x, yb = bullet.y;
            double vx = bullet.vx, vy = bullet.vy;
            double a = SQR(vx - dx) + SQR(vy - dy);
            double b = 2*((xb - xp)*(vx - dx) + (yb - yp)*(vy - dy));
            double c = SQR(xb - xp) + SQR(yb - yp) - SQR(BULLET_R);
            double min = MIN(a*0.01 + b*0.1 + c, c);
            double t = -b / (2*a);
            if (0 <= t && t <= 0.1)
                min = MIN(min, a*t*t + b*t + c);
            if (min <= 0) {
                hit = true;
                break;
            }
        }

        if (hit) gameInfo.gameStatus = BOSS_WIN;
        else if (gameInfo.round == 2999) gameInfo.gameStatus = PLANE_WIN;
    }
}

void GameServer::genRep() {
    vector<NewBullet> tmp;
    for (int i = 0; i < newBullets.size(); i ++) {
        NewBullet& bullet = newBullets[i];
        if (bullet.initTime == gameInfo.round)
            tmp.push_back(bullet);
    }
    fprintf(repFile, "%d\n", tmp.size());
    fprintf(repFile, "I'm Boss, HaHaHa\n");
    fprintf(repFile, "I'm Player, hAhAhA\n");
    fprintf(repFile, "300 600\n");
    fprintf(repFile, "%lf %lf\n", gameInfo.planeX, gameInfo.planeY);
    for (int i = 0; i < tmp.size(); i ++)
        fprintf(repFile, "%lf %lf %lf %lf\n", tmp[i].x, tmp[i].y, tmp[i].vx, tmp[i].vy);
    fprintf(repFile, "\n");
}

bool GameServer::isValidNewBullet(const NewBullet &bullet) {
    if (bullet.initTime < gameInfo.round) return false;
    return true;
}

bool GameServer::isValidPlaneAction(const PlaneAction &action) {
    if (action.startTime < gameInfo.round) return false;
    return true;
}
