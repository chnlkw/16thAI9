#include "gameserver.h"

GameServer::GameServer(int gui) {
    gameInfo.gameStatus = INIT;
    gameInfo.score = 0;
    gameInfo.planeX = PLANE_INIT_X;
    gameInfo.planeY = PLANE_INIT_Y;
    memset(gameInfo.planeSkillsNum, 0, sizeof(gameInfo.planeSkillsNum));
    cntRecvNewBulletsNum = cntRecvMovesNum = cntRecvSkillsNum = 0;
    lastSpeedup = 10000;
    hasGui = gui;
}

void GameServer::run() {
    this->listen(SERVER_ADDR, SERVER_PORT);
    cout << "begin shake hands" << endl;
    shakeHands();
    cout << "shake hands over" << endl;

    gameInfo.gameStatus = BATTLE;

    repFile = fopen("record.txt", "w");
    fprintf(repFile, "%s\n", players[(int)BOSS].name.toStdString().c_str());
    fprintf(repFile, "%s\n", players[(int)PLANE].name.toStdString().c_str());
    fprintf(repFile, "\n");

    for (gameInfo.round = 1; gameInfo.round <= 3000; gameInfo.round ++) {
        printf("%d %d\n", gameInfo.round, gameInfo.score);
        send(); // send the info of round i
        if (gameInfo.gameStatus != BATTLE) break;
        Timer::msleep(100);  // wait
        recv(); // recv the info of round >= i
        GameInfo cntGameInfo = gameInfo;
        vector<int> newBulletsId;
        updateGameInfo(newBulletsId);  // calc the info of round i+1
        judge(cntGameInfo, newBulletsId);   // judge if hit in [i, i + 1)
        genRep(cntGameInfo, newBulletsId);  // gen the rep of round i
    }

    sendString(bossSendSocket, QString("close"));
    sendString(planeSendSocket, QString("close"));
    if (hasGui) sendString(guiSendSocket, QString("close"));

    fclose(repFile);

    Timer::msleep(1000);
}

void GameServer::shakeHands() {
    for (int i = 0; i < 4 + hasGui; i ++) {
        this->waitForNewConnection(-1);
        QTcpSocket* socket = this->nextPendingConnection();
        sendString(socket, QString("accepted"));

        QString sr;
        recvString(socket, sr);

        cout << sr.toStdString() << endl;

        //players[clientType].socketDescriptor = socket->socketDescriptor();
        if (sr == "client sender") {
            int clientType;
            recvInt(socket, clientType);
            recvString(socket, players[clientType].name);
            if ((CLIENT_TYPE)clientType == BOSS) {
                bossRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvNewBullets, &recvBossMsg, this);
                connect(bossRecvThread, SIGNAL(finished()), bossRecvThread, SLOT(deleteLater()));
                bossRecvThread->start();
                sendString(socket, QString("shake hand over"));
            } else {
                planeRecvThread = new ServerReceiverThread(socket->socketDescriptor(), &recvMoves, &recvSkills, &recvPlaneMsg, this);
                connect(planeRecvThread, SIGNAL(finished()), planeRecvThread, SLOT(deleteLater()));
                planeRecvThread->start();
                sendString(socket, QString("shake hand over"));
            }
        } else if (sr == "client recver") {
            int clientType;
            recvInt(socket, clientType);
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

void GameServer::judge(const GameInfo& cntGameInfo, const vector<int>& newBulletsId) {
    double xp = cntGameInfo.planeX, yp = cntGameInfo.planeY;
    double dx = 0, dy = 0;
    for (int i = 0; i < moves.size(); i ++) {
        Move& act = moves[i];
        if (act.startTime <= cntGameInfo.round) {
            dx = act.vx;
            dy = act.vy;
        }
    }

    bool hit = false;
    for (int i = 0; i < cntGameInfo.bullets.size(); i ++) {
        const Bullet& bullet = cntGameInfo.bullets[i];
        double xb = bullet.x, yb = bullet.y;
        double vx = bullet.vx, vy = bullet.vy;
        double a = SQR(vx - dx) + SQR(vy - dy);
        double b = 2*((xb - xp)*(vx - dx) + (yb - yp)*(vy - dy));
        int type = getBulletType(bullet.vx, bullet.vy);
        double c = SQR(xb - xp) + SQR(yb - yp) - SQR(BULLET_R[type]);
        double min = MIN(a*0.01 + b*0.1 + c, c);
        double t = -b / (2*a);
        if (0 <= t && t <= 0.1)
            min = MIN(min, a*t*t + b*t + c);
        if (min <= 0) {
            hit = true;
            break;
        }
    }

    if (!hit) {
        for (int i = 0; i < newBulletsId.size(); i ++) {
            NewBullet& bullet = newBullets[i];
            double xb = BULLET_X, yb = BULLET_Y;
            double vx = bullet.vx, vy = bullet.vy;
            double a = SQR(vx - dx) + SQR(vy - dy);
            double b = 2*((xb - xp)*(vx - dx) + (yb - yp)*(vy - dy));
            int type = getBulletType(bullet.vx, bullet.vy);
            double c = SQR(xb - xp) + SQR(yb - yp) - SQR(BULLET_R[type]);
            double min = MIN(a*0.01 + b*0.1 + c, c);
            double t = -b / (2*a);
            if (0 <= t && t <= 0.1)
                min = MIN(min, a*t*t + b*t + c);
            if (min <= 0) {
                hit = true;
                break;
            }
        }
    }

    if (hit) gameInfo.gameStatus = BOSS_WIN;
    else if (gameInfo.round == 3000) gameInfo.gameStatus = PLANE_WIN;
}

void GameServer::updateGameInfo(vector<int>& newBulletsId) {
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
                SQR(move.vx) + SQR(move.vy) > SQR(PLANE_V) && lastSpeedup + SPEEDUP_TIME <= gameInfo.round)
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

    if (gameInfo.round > 1 && vx == 0 && vy == 0)
        gameInfo.score ++;
    if (gameInfo.score > 0 && gameInfo.score % 50 == 0)
        gameInfo.planeSkillsNum[0] ++;
    if (gameInfo.score > 0 && gameInfo.score % 100 == 0)
        gameInfo.planeSkillsNum[1] ++;
    gameInfo.planeX = planeX;
    gameInfo.planeY = planeY;

    // Calculate bullets
    vector<Bullet> bullets;
    if (!useBomb) {
        int cntBulletsNum[5], limitBulletsNum[5];
        memset(cntBulletsNum, 0, sizeof(cntBulletsNum));
        for (int i = 0; i < 5; i ++)
            limitBulletsNum[i] = (int)(BULLET_INIT_LIMIT[i] * BULLET_INCREASE(gameInfo.round + 1));
        for (int i = 0; i < newBullets.size(); i ++) {
            const NewBullet& nb = newBullets[i];
            if (nb.initTime > gameInfo.round + 1 ||
                    (gameInfo.round + 1 - nb.initTime) * BULLET_V[0] > sqrt(WIDTH*WIDTH + HEIGHT*HEIGHT)) continue;
            int type = getBulletType(nb.vx, nb.vy);
            if (cntBulletsNum[type] >= limitBulletsNum[type]) continue;

            if (nb.initTime == gameInfo.round)
                newBulletsId.push_back(i);

            double x = BULLET_X, y = BULLET_Y;
            x += nb.vx * (gameInfo.round + 1 - nb.initTime);
            y += nb.vy * (gameInfo.round + 1 - nb.initTime);
            if (0 <= x && x <= WIDTH && 0 <= y && y <= HEIGHT) {
                cntBulletsNum[type] ++;
                Bullet bullet;
                bullet.x = x;
                bullet.y = y;
                bullet.vx = nb.vx;
                bullet.vy = nb.vy;
                bullets.push_back(bullet);
            }
        }
    }
    gameInfo.bullets = bullets;
}


void GameServer::genRep(const GameInfo& cntGameInfo, const vector<int>& newBulletsId) {
    fprintf(repFile, "%d\n", newBulletsId.size());
    fprintf(repFile, "%d %d\n", cntGameInfo.round, cntGameInfo.score);
    fprintf(repFile, "%s\n", recvBossMsg.toStdString().c_str());
    fprintf(repFile, "%s\n", recvPlaneMsg.toStdString().c_str());
    fprintf(repFile, "%lf %lf %d\n", cntGameInfo.planeX, cntGameInfo.planeY, gameInfo.score - cntGameInfo.score);
    fprintf(repFile, "%d %d\n", cntGameInfo.planeSkillsNum[0], cntGameInfo.planeSkillsNum[1]);
    int useSpeedup = (lastSpeedup == gameInfo.round);
    fprintf(repFile, "%d %d\n", useSpeedup, useBomb);
    for (int i = 0; i < newBulletsId.size(); i ++) {
        int t = newBulletsId[i];
        fprintf(repFile, "%lf %lf\n", newBullets[t].vx, newBullets[t].vy);
    }
    fprintf(repFile, "\n");
}

void GameServer::send() {
    sendString(bossSendSocket, QString("actions"));
    sendString(planeSendSocket, QString("actions"));
    sendGameInfo(bossSendSocket, gameInfo);
    sendGameInfo(planeSendSocket, gameInfo);
    if (hasGui) {
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
            newBullets.push_back(recvNewBullets[i]);
    cntRecvNewBulletsNum = size;

    // moves
    size = recvMoves.size();
    for (int i = cntRecvMovesNum; i < size; i ++) {
        if (isValidMove(recvMoves[i]))
            moves.push_back(recvMoves[i]);
    }
    cntRecvMovesNum = size;

    // skills
    size = recvSkills.size();
    for (int i = cntRecvSkillsNum; i < size; i ++) {
        if (isValidSkill(recvSkills[i]))
            skills.push_back(skills[i]);
    }
}

bool GameServer::isValidNewBullet(const NewBullet &bullet) {
    if (bullet.initTime < gameInfo.round) return false;
    if (bullet.vy > 0) return false;
    if (getBulletType(bullet.vx, bullet.vy) == -1) return false;
    return true;
}

bool GameServer::isValidMove(const Move &move) {
    if (move.startTime < gameInfo.round) return false;
    double v = SQR(move.vx) + SQR(move.vy);
    if (v > SQR(PLANE_V * 2)) return false;
    return true;
}

bool GameServer::isValidSkill(const Skill &skill) {
    if (skill.startTime < gameInfo.round) return false;
    return true;
}
