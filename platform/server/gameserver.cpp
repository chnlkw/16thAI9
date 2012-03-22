#include "gameserver.h"

GameServer::GameServer(int gui, char* repFileName, int totRounds, int sleepTime) {
    gameInfo.gameStatus = INIT;
    gameInfo.score = 0;
    gameInfo.planeX = PLANE_INIT_X;
    gameInfo.planeY = PLANE_INIT_Y;
    memset(gameInfo.planeSkillsNum, 0, sizeof(gameInfo.planeSkillsNum));
    cntRecvNewBulletsNum = cntRecvMovesNum = cntRecvSkillsNum = 0;
    bossRecvFinish = planeRecvFinish = 0;
    prevBossRecv = prevPlaneRecv = 0;
    lastSpeedup = -10000;
    this->gui = gui;
    this->repFileName = repFileName;
    this->totRounds = totRounds;
    this->sleepTime = sleepTime;
}

void GameServer::run() {
    this->listen(SERVER_ADDR, SERVER_PORT);
    //cout << "begin shake hands" << endl;
    shakeHands();
    //cout << "shake hands over" << endl;

    gameInfo.gameStatus = BATTLE;

    repFile = fopen(repFileName, "w");
    fprintf(repFile, "%s\n", players[(int)BOSS].name.toStdString().c_str());
    fprintf(repFile, "%s\n", players[(int)PLANE].name.toStdString().c_str());
    fprintf(repFile, "\n");

    fprintf(repFile, "0\n");
    fprintf(repFile, "0 0\n");
    fprintf(repFile, "Init boss.\n");
    fprintf(repFile, "Init plane.\n");
    fprintf(repFile, "%lf %lf\n", PLANE_INIT_X, PLANE_INIT_Y);
    fprintf(repFile, "0 0\n");
    fprintf(repFile, "0 0\n");
    fprintf(repFile, "\n");

    for (gameInfo.round = 1; gameInfo.round <= totRounds; gameInfo.round ++) {
        printf("%d %d\n", gameInfo.round, gameInfo.score);
        send(); // send the info of round i
        if (gameInfo.gameStatus != BATTLE) break;
        for (int i = 0; i < 10; i ++) {
            Timer::msleep(sleepTime / 10);
            if (bossRecvFinish > prevBossRecv && planeRecvFinish > prevPlaneRecv) break;
        }
        prevBossRecv = bossRecvFinish;
        prevPlaneRecv = planeRecvFinish;
        //Timer::msleep(sleepTime);  // wait
        recv(); // recv the info of round >= i
        GameInfo cntGameInfo = gameInfo;
        vector<NewBullet> validNewBullets;
        updateGameInfo(validNewBullets);  // calc the info of round i+1
        judge(cntGameInfo, validNewBullets);   // judge if hit in [i, i + 1)
        genRep(cntGameInfo, validNewBullets);  // gen the rep of round i's starting
    }
    if (gameInfo.round == totRounds + 1) send();

    //printf("%d\n", gameInfo.score);

    sendString(bossSendSocket, QString("close"));
    sendString(planeSendSocket, QString("close"));
    if (gui) sendString(guiSendSocket, QString("close"));

    fclose(repFile);

    Timer::msleep(1000);
}

void GameServer::shakeHands() {
    for (int i = 0; i < 4 + gui; i ++) {
        //cout << i << endl;
        while (!this->waitForNewConnection(5000)) cout << "waiting connection" << endl;
        QTcpSocket* socket = this->nextPendingConnection();

        //cout << "new connection: " << socket->socketDescriptor() << endl;

        sendString(socket, QString("accepted"));

        //cout << "send accepted" << endl;

        QString sr;
        recvString(socket, sr);

        //cout << "recv " << sr.toStdString() << endl;

        //players[clientType].socketDescriptor = socket->socketDescriptor();
        if (sr == "client sender") {
            int clientType;
            recvInt(socket, clientType);
            //cout << "recv " << clientType << endl;
            recvString(socket, players[clientType].name);
            //cout << "recv " << players[clientType].name.toStdString() << endl;
            if ((CLIENT_TYPE)clientType == BOSS) {
                bossRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvNewBullets, &recvBossMsg, &bossRecvFinish, this);
                connect(bossRecvThread, SIGNAL(finished()), bossRecvThread, SLOT(deleteLater()));
                bossRecvThread->start();
                sendString(socket, QString("shake hand over"));
            } else {
                planeRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvMoves, &recvSkills, &recvPlaneMsg, &planeRecvFinish, this);
                connect(planeRecvThread, SIGNAL(finished()), planeRecvThread, SLOT(deleteLater()));
                planeRecvThread->start();
                sendString(socket, QString("shake hand over"));
            }
        } else if (sr == "client recver") {
            int clientType;
            recvInt(socket, clientType);
            //cout << "recv " << clientType << endl;
            if ((CLIENT_TYPE)clientType == BOSS) {
                bossSendSocket = new QTcpSocket();
                bossSendSocket->setSocketDescriptor(socket->socketDescriptor());
                sendString(socket, QString("shake hand over"));
            } else {
                planeSendSocket = new QTcpSocket();
                planeSendSocket->setSocketDescriptor(socket->socketDescriptor());
                sendString(socket, QString("shake hand over"));
            }
        } else if (sr == "gui") {
            guiSendSocket = new QTcpSocket();
            guiSendSocket->setSocketDescriptor(socket->socketDescriptor());
            sendString(socket, QString("shake hand over"));
        }
    }
}

void GameServer::judge(const GameInfo& cntGameInfo, const vector<NewBullet>& validNewBullets) {
    if (useBomb) return;

    double xp = cntGameInfo.planeX, yp = cntGameInfo.planeY;
    double dx = cntMoveX, dy = cntMoveY;

    bool hit = false;
    for (int i = 0; i < cntGameInfo.bullets.size(); i ++) {
        const Bullet& bullet = cntGameInfo.bullets[i];
        double xb = bullet.x, yb = bullet.y;
        double vx = bullet.vx, vy = bullet.vy;
        double a = SQR(vx - dx) + SQR(vy - dy);
        double b = 2*((xb - xp)*(vx - dx) + (yb - yp)*(vy - dy));
        int type = getBulletType(bullet.vx, bullet.vy);
        double c = SQR(xb - xp) + SQR(yb - yp) - SQR(BULLET_R[type]);
        double min = MIN(a + b + c, c);
        double t = -b / (2*a);
        if (0 <= t && t <= 1)
            min = MIN(min, a*t*t + b*t + c);
        if (min <= 0) {
            hit = true;
//            printf("hit: bullet: (%0.2lf, %0.2lf), (%0.2lf, %0.2lf)\n", bullet.x, bullet.y, bullet.vx, bullet.vy);
//            printf("plane: (%0.2lf, %0.2lf), (%0.2lf, %0.2lf)\n", xp, yp, dx, dy);
//            system("pause");
            break;
        }
    }

    if (!hit) {
        for (int i = 0; i < validNewBullets.size(); i ++) {
            const NewBullet& bullet = validNewBullets[i];
            double xb = BULLET_X, yb = BULLET_Y;
            double vx = bullet.vx, vy = bullet.vy;
            double a = SQR(vx - dx) + SQR(vy - dy);
            double b = 2*((xb - xp)*(vx - dx) + (yb - yp)*(vy - dy));
            int type = getBulletType(bullet.vx, bullet.vy);
            double c = SQR(xb - xp) + SQR(yb - yp) - SQR(BULLET_R[type]);
            double min = MIN(a + b + c, c);
            double t = -b / (2*a);
            if (0 <= t && t <= 1)
                min = MIN(min, a*t*t + b*t + c);
            if (min <= 0) {
                hit = true;
                break;
            }
        }
    }

    if (hit) gameInfo.gameStatus = BOSS_WIN;
    else if (gameInfo.round == totRounds) gameInfo.gameStatus = PLANE_WIN;
}

void GameServer::updateGameInfo(vector<NewBullet>& validNewBullets) {
    // Use skills
    useBomb = false;
    for (int i = 0; i < skills.size(); i ++) {
        const Skill& skill = skills[i];
        if (skill.startTime != gameInfo.round) continue;
        if (gameInfo.planeSkillsNum[(int)skill.type] == 0) continue;
        switch (skill.type) {
        case SPEEDUP:
            lastSpeedup = gameInfo.round;
            break;
        case BOMB:
            useBomb = true;
            break;
        }
        gameInfo.planeSkillsNum[(int)skill.type] --;
    }

    // Calculate planeX, planeY
    double vx = 0, vy = 0;
    vector<Move> validMoves;
    for (int i = 0; i < moves.size(); i ++) {
        const Move& move = moves[i];
        if (move.endTime <= gameInfo.round) continue;
        if (move.startTime == gameInfo.round &&
                SQR(move.vx) + SQR(move.vy) > SQR(PLANE_V) + EPSILON && lastSpeedup + SPEEDUP_TIME <= gameInfo.round)
            continue;
        validMoves.push_back(move);
        if (move.startTime <= gameInfo.round) {
            vx = move.vx;
            vy = move.vy;
        }
    }

    moves = validMoves;
    double planeX = gameInfo.planeX + vx, planeY = gameInfo.planeY + vy;

    if (planeX < 0) planeX = 0;
    if (planeX > WIDTH) planeX = WIDTH;
    if (planeY < 0) planeY = 0;
    if (planeY > HEIGHT) planeY = HEIGHT;

    cntMoveX = planeX - gameInfo.planeX;
    cntMoveY = planeY - gameInfo.planeY;

    gameInfo.planeX = planeX;
    gameInfo.planeY = planeY;

    if (cntMoveX == 0 && cntMoveY == 0) {
        gameInfo.score ++;
        if (gameInfo.score > 0 && gameInfo.score % GAIN_SPEEDUP == 0)
            gameInfo.planeSkillsNum[0] ++;
        if (gameInfo.score > 0 && gameInfo.score % GAIN_BOMB == 0)
            gameInfo.planeSkillsNum[1] ++;
    }

    // Calculate bullets
    vector<Bullet> bullets;
    if (!useBomb) {
        int cntBulletsNum[5], limitBulletsNum[5];
        memset(cntBulletsNum, 0, sizeof(cntBulletsNum));
        for (int i = 0; i < 5; i ++)
            limitBulletsNum[i] = (int)(BULLET_INIT_LIMIT[i] * BULLET_INCREASE(gameInfo.round));

        for (int i = 0; i < gameInfo.bullets.size(); i ++) {
            int type = getBulletType(gameInfo.bullets[i].vx, gameInfo.bullets[i].vy);
            double x = gameInfo.bullets[i].x;
            double y = gameInfo.bullets[i].y;
            x += gameInfo.bullets[i].vx;
            y += gameInfo.bullets[i].vy;
            if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
                cntBulletsNum[type] ++;
                Bullet bullet;
                bullet.x = x;
                bullet.y = y;
                bullet.vx = gameInfo.bullets[i].vx;
                bullet.vy = gameInfo.bullets[i].vy;
                bullets.push_back(bullet);
            }
        }

        for (int j = 0; j < 5; j ++) {
            for (int i = 0; i < newBullets[gameInfo.round][j].size(); i ++) {
                if (cntBulletsNum[j] >= limitBulletsNum[j]) break;
                NewBullet& newBullet = newBullets[gameInfo.round][j][i];
                int type = getBulletType(newBullet.vx, newBullet.vy);
                double x = BULLET_X, y = BULLET_Y;
                x += newBullet.vx;
                y += newBullet.vy;
                if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
                    validNewBullets.push_back(newBullet);
                    cntBulletsNum[type] ++;
                    Bullet bullet;
                    bullet.x = x;
                    bullet.y = y;
                    bullet.vx = newBullet.vx;
                    bullet.vy = newBullet.vy;
                    bullets.push_back(bullet);
                }
            }
        }
    }
    gameInfo.bullets = bullets;
}


void GameServer::genRep(const GameInfo& cntGameInfo, const vector<NewBullet>& validNewBullets) {
    fprintf(repFile, "%d\n", validNewBullets.size());
    fprintf(repFile, "%d %d\n", cntGameInfo.round, cntGameInfo.score);
    if (recvBossMsg == "") recvBossMsg = "NULL";
    if (recvPlaneMsg == "") recvPlaneMsg = "NULL";
    fprintf(repFile, "%s\n", recvBossMsg.replace("\r", " ").replace("\n", " ").toStdString().c_str());
    fprintf(repFile, "%s\n", recvPlaneMsg.replace("\r", " ").replace("\n", " ").toStdString().c_str());
    fprintf(repFile, "%lf %lf %d\n", cntGameInfo.planeX, cntGameInfo.planeY, gameInfo.score - cntGameInfo.score);
    fprintf(repFile, "%d %d\n", cntGameInfo.planeSkillsNum[0], cntGameInfo.planeSkillsNum[1]);
    int useSpeedup = (lastSpeedup == gameInfo.round);
    fprintf(repFile, "%d %d\n", useSpeedup, useBomb);
    for (int i = 0; i < validNewBullets.size(); i ++)
        fprintf(repFile, "%lf %lf\n", validNewBullets[i].vx, validNewBullets[i].vy);
    fprintf(repFile, "\n");
}

void GameServer::send() {
    sendString(bossSendSocket, QString("actions"));
    sendString(planeSendSocket, QString("actions"));
    //printf("send bullet.size = %d\n", gameInfo.bullets.size());
    sendGameInfo(bossSendSocket, gameInfo);
    sendGameInfo(planeSendSocket, gameInfo);
    if (gui) {
        sendString(guiSendSocket, QString("actions"));
        sendInt(guiSendSocket, gameInfo.round);
        sendInt(guiSendSocket, gameInfo.score);
    }
}

void GameServer::recv() {
    // new bullets
    int size = recvNewBullets.size();
    for (int i = cntRecvNewBulletsNum; i < size; i ++)
        if (isValidNewBullet(recvNewBullets[i]))
            newBullets[recvNewBullets[i].initTime][getBulletType(recvNewBullets[i].vx, recvNewBullets[i].vy)].push_back(recvNewBullets[i]);
    cntRecvNewBulletsNum = size;

    // moves
    size = recvMoves.size();
    for (int i = cntRecvMovesNum; i < size; i ++) {
        //printf("%lf %lf ", recvMoves[i].vx, recvMoves[i].vy);
        if (isValidMove(recvMoves[i])) {
            moves.push_back(recvMoves[i]);
            //printf("valid\n");
        } else {
            //printf("invalid\n");
        }
    }
    cntRecvMovesNum = size;

    // skills
    size = recvSkills.size();
    for (int i = cntRecvSkillsNum; i < size; i ++) {
        if (isValidSkill(recvSkills[i]))
            skills.push_back(recvSkills[i]);
    }
    cntRecvSkillsNum = size;
}

int GameServer::getBulletType(double vx, double vy) {
    for (int i = 0; i < 5; i ++)
        if (fabs(SQR(vx)+SQR(vy) - SQR(BULLET_V[i])) < EPSILON)
            return i;
    return -1;
}


bool GameServer::isValidNewBullet(const NewBullet &bullet) {
    if (bullet.initTime < gameInfo.round || bullet.initTime > totRounds) return false;
    //if (bullet.vy > 0) return false;
    if (getBulletType(bullet.vx, bullet.vy) == -1) return false;
    return true;
}

bool GameServer::isValidMove(const Move &move) {
    if (move.startTime < gameInfo.round) return false;
    double v = SQR(move.vx) + SQR(move.vy);
    if (v > SQR(PLANE_V * 2) + EPSILON) return false;
    return true;
}

bool GameServer::isValidSkill(const Skill &skill) {
    if (skill.startTime < gameInfo.round) return false;
    return true;
}
