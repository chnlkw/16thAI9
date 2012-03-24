#include "network.h"

void sendString(QTcpSocket* socket, const QString& v) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << v;
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

bool recvString(QTcpSocket *socket, QString &v, int mode) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    int count = 0;
    while (socket->bytesAvailable() < (int)sizeof(quint32)) {
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
        count ++;
        if (mode == 1 && count == 300) return false;
    }
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    in >> v;
    return true;
}

void sendInt(QTcpSocket *socket, const int &v) {
    QDataStream out(socket);
    out << (qint32)v;
    socket->waitForBytesWritten(-1);
}

void recvInt(QTcpSocket *socket, int &v) {
    QDataStream in(socket);
    while (socket->bytesAvailable() < (int)sizeof(qint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    in >> v;
}

void sendBossActions(QTcpSocket *socket, const vector<NewBullet> &newBullets) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)newBullets.size();
    for (int i = 0; i < newBullets.size(); i ++) {
        out << newBullets[i].initTime;
        out << newBullets[i].vx;
        out << newBullets[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void sendBossActions(QTcpSocket *socket, const vector<NewBullet> &newBullets, int start, int end) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)(end - start);
    for (int i = start; i < end; i ++) {
        out << newBullets[i].initTime;
        out << newBullets[i].vx;
        out << newBullets[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvBossActions(QTcpSocket *socket, vector<NewBullet> &newBullets) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        NewBullet newBullet;
        in >> newBullet.initTime;
        in >> newBullet.vx;
        in >> newBullet.vy;
        newBullets.push_back(newBullet);
    }
}

void sendMoves(QTcpSocket *socket, const vector<Move>& moves) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)moves.size();
    for (int i = 0; i < moves.size(); i ++) {
        out << moves[i].startTime;
        out << moves[i].endTime;
        out << moves[i].vx;
        out << moves[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void sendMoves(QTcpSocket *socket, const vector<Move>& moves, int start, int end) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)(end - start);
    for (int i = start; i < end; i ++) {
        out << moves[i].startTime;
        out << moves[i].endTime;
        out << moves[i].vx;
        out << moves[i].vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvMoves(QTcpSocket *socket, vector<Move>& moves) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        Move move;
        in >> move.startTime;
        in >> move.endTime;
        in >> move.vx;
        in >> move.vy;
        moves.push_back(move);
    }
}

void sendSkills(QTcpSocket *socket, const vector<Skill>& skills) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)skills.size();
    for (int i = 0; i < skills.size(); i ++) {
        out << skills[i].startTime;
        out << (int)skills[i].type;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void sendSkills(QTcpSocket *socket, const vector<Skill>& skills, int start, int end) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << (quint32)(end - start);
    for (int i = start; i < end; i ++) {
        out << skills[i].startTime;
        out << (int)skills[i].type;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvSkills(QTcpSocket *socket, vector<Skill>& skills) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32))
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize)
        socket->waitForReadyRead(SOCKET_RECV_TIMEOUT);
    quint32 size;
    in >> size;
    for (int i = 0; i < size; i ++) {
        Skill skill;
        in >> skill.startTime;
        int skillType;
        in >> skillType;
        skill.type = (PLANE_SKILLS)skillType;
        skills.push_back(skill);
    }
}

void sendGameInfo(QTcpSocket *socket, const GameInfo &gameInfo) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint32)0;
    out << gameInfo.round;
    out << gameInfo.score;
    out << (int)gameInfo.gameStatus;
    out << gameInfo.planeSkillsNum[0];
    out << gameInfo.planeSkillsNum[1];
    out << gameInfo.planeX;
    out << gameInfo.planeY;
    out << (quint32)gameInfo.bullets.size();
    for (int i = 0; i < gameInfo.bullets.size(); i ++) {
        const Bullet& bullet = gameInfo.bullets[i];
        out << bullet.x;
        out << bullet.y;
        out << bullet.vx;
        out << bullet.vy;
    }
    out.device()->seek(0);
    out << (quint32)(block.size() - sizeof(quint32));
    socket->write(block);
    socket->waitForBytesWritten(-1);
}

void recvGameInfo(QTcpSocket *socket, GameInfo &gameInfo) {
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_0);
    while (socket->bytesAvailable() < (int)sizeof(quint32)) {
        if (!socket->waitForReadyRead(SOCKET_RECV_TIMEOUT)) {
            printf("error1\n");
        }
    }
    quint32 blockSize;
    in >> blockSize;
    while (socket->bytesAvailable() < blockSize) {
        if (!socket->waitForReadyRead(SOCKET_RECV_TIMEOUT)) {
            printf("error2\n");
        }
    }
    in >> gameInfo.round;
    in >> gameInfo.score;
    int gameStatus;
    in >> gameStatus;
    gameInfo.gameStatus = (GAME_STATUS)gameStatus;
    in >> gameInfo.planeSkillsNum[0];
    in >> gameInfo.planeSkillsNum[1];
    in >> gameInfo.planeX;
    in >> gameInfo.planeY;
    quint32 size;
    in >> size;
    gameInfo.bullets.clear();
    for (int i = 0; i < size; i ++) {
        Bullet bullet;
        in >> bullet.x;
        in >> bullet.y;
        in >> bullet.vx;
        in >> bullet.vy;
        gameInfo.bullets.push_back(bullet);
    }
    if (gameInfo.bullets.size() != size)
        printf("error!!!\n");
}
