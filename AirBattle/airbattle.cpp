#include "airbattle.h"
#include "ui_airbattle.h"

AirBattle::AirBattle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AirBattle)
{
    ui->setupUi(this);
    GameInit();
}

AirBattle::~AirBattle()
{
    delete ui;
}


void AirBattle::GameInit()
{
    view = new View(this);
    gamecenter = new GameCenter(view);
    record = new Record(gamecenter);
    this->setGeometry(100, 50, WIDTH + 100, HEIGTH + 33);

    record->Play_Record("record.txt");
}
