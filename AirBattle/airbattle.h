#ifndef AIRBATTLE_H
#define AIRBATTLE_H

#include <QMainWindow>
#include <QModelIndex>
#include <QTextCodec>
#include <QPropertyAnimation>
#include <QThread>
#include <QKeyEvent>

#include "filecenter.h"
#include "server.h"
#include "Record.h"
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

signals:
    void reset();

public slots:
    void setTime(int);
    void setScore(int);
    void setBossWord(QString);
    void setPlayerWord(QString);
    void setBossName(QString);
    void setPlayerName(QString);

    void setState(QString);
    void setVS(QString, QString);
    void setRecord(QString);

private slots:
    void on_record_clicked();
    void on_AI_clicked();
    void on_start_clicked();
    void on_list_pressed(const QModelIndex &index);
    void on_list_entered(const QModelIndex &index);
    void on_list_clicked(const QModelIndex &index);

    void Door_Open_End();
    void Door_Close_End();
    void Record_Over();

    void on_viewstart_clicked();
    void on_viewback_clicked();

    void recv_server(int,int);

    void on_SButton_clicked();
    void on_SButton_Removed();

private:
    Ui::AirBattle *ui;
    View *view;
    GameCenter *gamecenter;
    Record *record;
    HumanConsole* humanConsole;
    FileCenter *filecenter;
    Server *server;

    QString p1, p2;
    QString rec;
    int state;
    // 0: 显示AI列表
    // 1: 显示录像列表
    // 2: 单击播放录像以后
    // 3: 单击对战AI以后
    // 4: 尚未播放录像
    // 5: 正在播放录像
    // 6: 录像播放完毕
    // 7: 暂停播放录像
    // 8: 尚未开始人机对战


private:
    void GameInit();
    void fight(QString, QString);
    void playrecord(QString);

    void Door_Open(int Delay = 1500);
    void Door_Close(int Delay = 1500);
    void SButton_Drop(int Delay = 1500);

    void swaptoframe();
    void swaptoframe2();
};

#endif // AIRBATTLE_H
