#ifndef AIRBATTLE_H
#define AIRBATTLE_H

#include <QMainWindow>
#include <GameCenter.h>
#include <QModelIndex>
#include <Record.h>
#include <QTextCodec>
#include <QPropertyAnimation>
#include <QThread>
#include <QKeyEvent>

#include "humanconsole.h"

namespace Ui {
    class AirBattle;
}

class MyTimer : public QThread
{
    Q_OBJECT
public:
    explicit MyTimer(QObject *parent = 0) : QThread(parent) {}
    static void msleep(unsigned long secs) { QThread::msleep(secs); }
};

class AirBattle : public QMainWindow
{
    Q_OBJECT

public:
    explicit AirBattle(QWidget *parent = 0);
    ~AirBattle();
    void roundTimeOut();

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

public slots:
    void setTime(int);
    void setScore(int);
    void setBossName(QString);
    void setPlayerName(QString);

    void setValue1(int);
    void setValue2(int);

private:
    Ui::AirBattle *ui;
    View *view;
    GameCenter *gamecenter;
    Record *record;

    QString p1, p2;
    QString rec;

    HumanConsole* humanConsole;

private:
    void GameInit();
    void fight(QString, QString);
    void playrecord(QString);

    void start();
};

#endif // AIRBATTLE_H
