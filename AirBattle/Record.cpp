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
    timer->setInterval(1000*GAMESPEED+0.01);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    connect(gamecenter, SIGNAL(Finished()), this, SLOT(OnViewFinished()));
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

    // 开始播放录像
    gamecenter->clear();
    ip = 0;
    Play_Before();

    return true;
}

void Record::Play_Before()
{
    boss = lines[0];
    player = lines[1];
    boss_pos = QPointF(500, 800);
    ip = 2;
    time = 0;

    OnTimer();
}

void Record::Play()
{
    timer->start();
    gamecenter->Continue();
}

void Record::OnTimer()
{
    if (ip >= lines.size())
    {
        timer->stop();
        gamecenter->Record_Over();
        return;
    }
    bool flag = (time == 0);

    emit setTime(time);

    DealFrame(flag);
    if (flag)
    {
        gamecenter->init(player_pos, boss_pos);
    }
    else
    {
        gamecenter->ElementMoveTo(1, player_pos, GAMESPEED);
    }
    time++;
    if (ip + 4 > lines.size())
    {
        timer->stop();
        gamecenter->Record_Over();
        return;
    }
}

void Record::DealFrame(bool tag)
{
    QString tmp = lines[ip++];
    int n = tmp.toInt();

    qDebug() << n;

    tmp = lines[ip++];
    int r, s;
    sscanf(tmp.toStdString().c_str(), "%d%d", &r, &s);
    emit setScore(s);

    QString boss = lines[ip++];
    QString player = lines[ip++];

    emit setBossWord(boss);
    emit setPlayerWord(player);

    float x,y,vx,vy;

    // 读取player坐标
    int flag;
    tmp = lines[ip++];
    sscanf(tmp.toStdString().c_str(), "%f%f%d", &x, &y, &flag);
    player_pos.setX(x);
    player_pos.setY(y);

    if (flag==1)
        gamecenter->addPlaneBullet(player_pos ,boss_pos);
    ip++;
    ip++;

    // 创建所有子弹
    for (int i = 0; i < n; i++)
    {
        tmp = lines[ip++];
        x = boss_pos.x(), y = boss_pos.y();
        sscanf(tmp.toStdString().c_str(), "%f%f", &vx, &vy);
        vx *= 10, vy *= 10;
        gamecenter->addBullet(QPointF(x, y), QPointF(vx, vy));
    }

    if (!tag && ip+4 < lines.size())
    {
        tmp = lines[ip+4];
        sscanf(tmp.toStdString().c_str(), "%f%f%d", &x, &y, &flag);
        player_pos.setX(x);
        player_pos.setY(y);
    }
}

void Record::Pause()
{
    timer->stop();
    gamecenter->Pause();
}

void Record::Continue()
{
    timer->start();
    gamecenter->Continue();
}

void Record::OnViewFinished()
{
    emit recordOver();
}
