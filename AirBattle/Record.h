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
    void setBossWord(QString);
    void setPlayerWord(QString);
    void recordOver();

public slots:
    void OnTimer();
    void ViewTimer();
    void OnViewFinished();

private:
    QStringList lines;
    int ip; // 指向当前待读行
    int time; // 第几回合
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
