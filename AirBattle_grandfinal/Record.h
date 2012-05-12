#ifndef RECORD_H
#define RECORD_H

#include <GameCenter.h>
#include <QStringList>

class Record : public QObject
{
    Q_OBJECT

public:
    GameCenter *gamecenter;

    Record(GameCenter *game);

    bool Play_Record(QString file);

    void GameInit();

    void Play_Before();
    void Play();

    QString boss, player;

signals:
    void setScore(int);
    void setTime(int);
    void setBossName(QString);
    void setPlayerName(QString);
    void setValue1(int);
    void setValue2(int);
    void stop();

public slots:
    void OnTimer();
    void ViewTimer();

private:
    QStringList lines;
    int ip; // ָ��ǰ������
    int time; // �ڼ��غ�
    bool pause;
    bool timer_state;
    int timer_count;
    int SpeedUP, Bomb;

    void DealFrame(bool tag = false);
    void timer_start();
    void timer_stop();

    QPointF player_pos, boss_pos;
};

#endif // RECORD_H
