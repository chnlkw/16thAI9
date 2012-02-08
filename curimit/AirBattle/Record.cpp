#include <Record.h>
#include <QTextStream>
#include <QDebug>
#include <QFile>

Record::Record(GameCenter *game)
{
    gamecenter = game;
    GameInit();
}

void Record::GameInit()
{
    timer = new QTimer;
    timer->setInterval(1000*GAMESPEED);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

bool Record::Play_Record(QString filename)
{
    lines.clear();
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) return false;
    QTextStream stream(&file);
    QString line;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        if (line == "") continue;
        //qDebug() << line;
        lines += line;
    }
    file.close();

    // ��ʼ����¼��
    gamecenter->clear();
    ip = 0;
    timer->start();

    return true;
}

void Record::OnTimer()
{
    if (ip >= lines.size())
    {
        timer->stop();
        return;
    }
    bool flag = (ip == 0);

    DealFrame();
    if (flag)
    {
        gamecenter->init(player_pos, boss_pos);
    }
    else
    {
        gamecenter->ElementMoveTo(1, player_pos, GAMESPEED);
        gamecenter->ElementMoveTo(2, boss_pos, GAMESPEED);
    }
}

void Record::DealFrame()
{
    QString tmp = lines[ip++];
    int n = tmp.toInt();
    QString boss = lines[ip++];
    QString player = lines[ip++];

    float x,y,vx,vy;
    // ��ȡboss����
    tmp = lines[ip++];
    sscanf(tmp.toStdString().c_str(), "%f%f", &x, &y);

    boss_pos.setX(x);
    boss_pos.setY(y);
    // ��ȡplayer����
    tmp = lines[ip++];
    sscanf(tmp.toStdString().c_str(), "%f%f", &x, &y);
    player_pos.setX(x);
    player_pos.setY(y);

    // ���������ӵ�
    for (int i = 0; i < n; i++)
    {
        tmp = lines[ip++];
        sscanf(tmp.toStdString().c_str(), "%f%f%f%f", &x, &y, &vx, &vy);
        gamecenter->addBullet(QPointF(x,y), QPointF(vx, vy));
    }
}
