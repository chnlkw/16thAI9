#ifndef AIRBATTLE_H
#define AIRBATTLE_H

#include <QMainWindow>
#include <GameCenter.h>
#include <Record.h>

namespace Ui {
    class AirBattle;
}

class AirBattle : public QMainWindow
{
    Q_OBJECT

public:
    explicit AirBattle(QWidget *parent = 0);
    ~AirBattle();

private:
    Ui::AirBattle *ui;
    View *view;
    GameCenter *gamecenter;
    Record *record;

private:
    void GameInit();
};

#endif // AIRBATTLE_H
