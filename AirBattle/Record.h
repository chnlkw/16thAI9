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

private slots:
    void OnTimer();

private:
    QStringList lines;
    QTimer *timer;
    int ip; // 指向当前待读行

    void DealFrame();

    QPointF player_pos, boss_pos;
};

#endif // RECORD_H
