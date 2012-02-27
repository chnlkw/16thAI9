#include "gameserver.h"

GameServer::GameServer() {
    gameInfo.gameStatus = INIT;
    gameInfo.score = 0;
    gameInfo.planeX = PLANE_INIT_X;
    gameInfo.planeY = PLANE_INIT_Y;
    cntRecvNewBulletsNum = cntRecvPlaneActionsNum = 0;
    cntSendNewBulletsNum = cntSendPlaneActionsNum = 0;
}

void GameServer::run() {
    this->listen(SERVER_ADDR, SERVER_PORT);

    shakeHands();
    cout << "shake hands over" << endl;

    gameInfo.gameStatus = BATTLE;

    repFile = fopen("record.txt", "w");
    fprintf(repFile, "%s\n", players[(int)BOSS].name.toStdString().c_str());
    fprintf(repFile, "%s\n", players[(int)PLANE].name.toStdString().c_str());
    fprintf(repFile, "\n");

    for (int i = 0; i <= 3000; i ++) {
        ServerTimer::msleep(100);
        gameInfo.round = i;
        cout << gameInfo.round << " ";
        calc();
        int prevScore = gameInfo.score;
        updateGameInfo(gameInfo, newBullets, planeActions);
        genRep(gameInfo.score - prevScore);
        cout << gameInfo.score << endl;
        send(bossSendSocket, planeSendSocket);
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
                bossRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvNewBullets, &recvBossMsg, this);
                connect(bossRecvThread, SIGNAL(finished()), bossRecvThread, SLOT(deleteLater()));
                bossRecvThread->start();
                sendString(socket, QString("shake hand over"));
            } else {
                cout << "plane sender" << endl;
                planeRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvPlaneActions, &recvPlaneMsg, this);
                connect(planeRecvThread, SIGNAL(finished()), planeRecvThread, SLOT(deleteLater()));
                planeRecvThread->start();
                sendString(socket, QString("shake hand over"));
            }
        } else if (sr == "client recver") {
            if ((CLIENT_TYPE)clientType == BOSS) {
                bossSendSocket = new QTcpSocket();
                bossSendSocket->setSocketDescriptor(socket->socketDescriptor());
                sendString(socket, QString("shake hand over"));
            } else {
                planeSendSocket = new QTcpSocket();
                planeSendSocket->setSocketDescriptor(socket->socketDescriptor());
                sendString(socket, QString("shake hand over"));
            }
        }
    }
}

void GameServer::calc() {
//    cout << "round = " << gameInfo.round << endl;
//    cout << "plane = " << gameInfo.planeX << "," << gameInfo.planeY << endl;

    int size = recvNewBullets.size();
    for (int i = cntRecvNewBulletsNum; i < size; i ++)
        if (isValidNewBullet(recvNewBullets[i]))
            newBullets.push_back(recvNewBullets[i]);
    cntRecvNewBulletsNum = size;

    size = recvPlaneActions.size();
    for (int i = cntRecvPlaneActionsNum; i < size; i ++) {
        if (isValidPlaneAction(recvPlaneActions[i]))
            planeActions.push_back(recvPlaneActions[i]);
    }
    cntRecvPlaneActionsNum = size;

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
//            printf("xp = %0.2lf, yp = %0.2lf\ndx = %0.2lf, dy = %0.2lf\n", xp, yp, dx, dy);
//            printf("xb = %0.2lf, yb = %0.2lf\nvx = %0.2lf, vy = %0.2lf\n", xb, yb, vx, vy);
//            printf("a = %0.2lf, b = %0.2lf, c = %0.2lf\n, t = %0.2lf, min = %0.2lf\n", a, b, c, t, min);
        }

        if (hit) gameInfo.gameStatus = BOSS_WIN;
        else if (gameInfo.round == 3000) gameInfo.gameStatus = PLANE_WIN;
    }
}

void GameServer::genRep(int move) {
    vector<NewBullet> tmp;
    for (int i = 0; i < newBullets.size(); i ++) {
        NewBullet& bullet = newBullets[i];
        if (bullet.initTime == gameInfo.round)
            tmp.push_back(bullet);
    }
    fprintf(repFile, "%d\n", tmp.size());
    fprintf(repFile, "%d %d\n", gameInfo.round, gameInfo.score);
    fprintf(repFile, "%s\n", recvBossMsg.toStdString().c_str());
    fprintf(repFile, "%s\n", recvPlaneMsg.toStdString().c_str());
    fprintf(repFile, "%lf %lf %d\n", gameInfo.planeX, gameInfo.planeY, move);
    for (int i = 0; i < tmp.size(); i ++)
        fprintf(repFile, "%lf %lf\n", tmp[i].vx, tmp[i].vy);
    fprintf(repFile, "\n");
}

void GameServer::send(QTcpSocket* bossSocket, QTcpSocket* planeSocket) {
    sendString(bossSocket, QString("actions"));
    sendString(planeSocket, QString("actions"));
    sendGameInfo(bossSocket, gameInfo);
    sendGameInfo(planeSocket, gameInfo);
    int size = newBullets.size();
    sendBossActions(bossSocket, newBullets, cntSendNewBulletsNum, size);
    sendBossActions(planeSocket, newBullets, cntSendNewBulletsNum, size);
    cntSendNewBulletsNum = size;
    size = planeActions.size();
    sendPlaneActions(bossSocket, planeActions, cntSendPlaneActionsNum, size);
    sendPlaneActions(planeSocket, planeActions, cntSendPlaneActionsNum, size);
    cntSendPlaneActionsNum = size;
    //cout << time(0) << " send round = " << gameInfo.round << endl;
}

bool GameServer::isValidNewBullet(const NewBullet &bullet) {
    if (bullet.initTime < gameInfo.round) return false;
    if (bullet.vy > 0) return false;
    double v = bullet.vx*bullet.vx + bullet.vy*bullet.vy;
    if (fabs(v - BULLET_V*BULLET_V) > EPSILON) return false;
    return true;
}

bool GameServer::isValidPlaneAction(const PlaneAction &action) {
    if (action.startTime < gameInfo.round) return false;
    double v = action.dx*action.dx + action.dy*action.dy;
    if (v > PLANE_V*PLANE_V) return false;
    return true;
}
