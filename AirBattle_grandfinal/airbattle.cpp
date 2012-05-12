#include "airbattle.h"
#include "ui_airbattle.h"
#include <QDate>
#include <QTime>
#include <QFile>

const char TextCodec[] = "UTF-8";

AirBattle::AirBattle(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AirBattle)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName(TextCodec));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(TextCodec));
    QTextCodec::setCodecForTr(QTextCodec::codecForName(TextCodec));

    GameInit();
    setFixedSize(1024, 768);
    showFullScreen();
}

AirBattle::~AirBattle()
{
    delete ui;
}


void AirBattle::roundTimeOut()
{
    record->OnTimer();
}

void AirBattle::GameInit()
{
    view = new View(this);
    gamecenter = new GameCenter(view);
    record = new Record(gamecenter);

    /* Human Console */
    humanConsole = new HumanConsole(gamecenter);
    connect(view, SIGNAL(call_record()), humanConsole, SLOT(onTimer()));
    connect(humanConsole, SIGNAL(gameOver()), view, SLOT(stop()));
    connect(humanConsole, SIGNAL(setBossName(QString)), this, SLOT(setBossName(QString)));
    connect(humanConsole, SIGNAL(setPlayerName(QString)), this, SLOT(setPlayerName(QString)));
    connect(humanConsole, SIGNAL(setTime(int)), this, SLOT(setTime(int)));
    connect(humanConsole, SIGNAL(setScore(int)), this, SLOT(setScore(int)));
    connect(humanConsole, SIGNAL(setValue1(int)), this, SLOT(setValue1(int)));
    connect(humanConsole, SIGNAL(setValue2(int)), this, SLOT(setValue2(int)));

    /* Record */
    setFocusPolicy(Qt::StrongFocus);
    connect(view, SIGNAL(call_record()), record, SLOT(ViewTimer()));
    connect(record, SIGNAL(setTime(int)), this, SLOT(setTime(int)));
    connect(record, SIGNAL(setScore(int)), this, SLOT(setScore(int)));
    connect(record, SIGNAL(setValue1(int)), this, SLOT(setValue1(int)));
    connect(record, SIGNAL(setValue2(int)), this, SLOT(setValue2(int)));
    connect(record, SIGNAL(setBossName(QString)), this, SLOT(setBossName(QString)));
    connect(record, SIGNAL(setPlayerName(QString)), this, SLOT(setPlayerName(QString)));
    connect(record, SIGNAL(stop()), view, SLOT(stop()));

    ui->menu->setPixmap(QPixmap("res/menu.png"));
    ui->menu->setGeometry(0, 0, 1024, 768);
    //ui->menu->hide();

    ui->score->raise();
    ui->timem->raise();
    ui->times->raise();
    ui->timed->raise();
    ui->boss_name->raise();
    ui->player_name->raise();

    ui->score->setGeometry(815, 105, 180, 50);
    ui->timem->setGeometry(805, 238, 80, 40);
    ui->times->setGeometry(885, 238, 80, 40);
    ui->timed->setGeometry(968, 238, 40, 40);
    ui->boss_name->setGeometry(790, 425, 208, 80);
    ui->player_name->setGeometry(790, 610, 208, 80);
    ui->value1->setGeometry(848, 318, 80, 40);
    ui->value2->setGeometry(963, 318, 80, 40);

    // 显示时间、分数、AI名称等字体
    QPalette p = palette();
    QPalette q = palette();
    QPalette r = palette();

    p.setColor(QPalette::WindowText, QColor(229, 5, 35));
    q.setColor(QPalette::WindowText, QColor(201, 201, 201));
    r.setColor(QPalette::WindowText, QColor(229, 5, 35));

    ui->score->setPalette(p);
    ui->timem->setPalette(q);
    ui->times->setPalette(q);
    ui->timed->setPalette(q);
    ui->boss_name->setPalette(r);
    ui->player_name->setPalette(r);
    ui->value1->setPalette(q);
    ui->value2->setPalette(q);

    record->Play_Record("replay.txt");
}

void AirBattle::setTime(int x)
{
    int n = x;
    int min = n/600;
    int sec = n%600/10;
    int dd = n%10;
    if (min < 10)
        ui->timem->setText("0" + QString::number(min));
    else ui->timem->setText(QString::number(min));
    if (sec < 10)
        ui->times->setText("0" + QString::number(sec));
    else ui->times->setText(QString::number(sec));
    ui->timed->setText(QString::number(dd));
}

void AirBattle::setScore(int x)
{
    QString ans = QString::number(x);
    if (x < 10000) ans = "0" + ans;
    if (x < 1000) ans = "0" + ans;
    if (x < 100) ans = "0" + ans;
    if (x < 10) ans = "0" + ans;
    ui->score->setText(ans);
}

void AirBattle::setBossName(QString x)
{
    ui->boss_name->setText(x);
}

void AirBattle::setPlayerName(QString x)
{
    ui->player_name->setText(x);
}

void AirBattle::setValue1(int x)
{
    ui->value1->setText(QString::number(x/2));
}

void AirBattle::setValue2(int x)
{
    ui->value2->setText(QString::number(x/5));
}


void AirBattle::playrecord(QString file)
{
    file = "replay/" + file + ".txt";
    setBossName(record->boss);
    setPlayerName(record->player);
    record->Play_Record(file);
}

void AirBattle::keyPressEvent(QKeyEvent *e)
{
    static bool started = false;
    //if (started) return;
    switch(e->key())
    {
    case Qt::Key_Space:
        started = true;
        start();
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        view->speedUp();
        break;
    case Qt::Key_Minus:
        view->speedDown();
        break;
    case Qt::Key_R:
        humanConsole->restart();
        break;
    case Qt::Key_Up:
        humanConsole->keyUp = true;
        break;
    case Qt::Key_Down:
        humanConsole->keyDown = true;
        break;
    case Qt::Key_Left:
        humanConsole->keyLeft = true;
        break;
    case Qt::Key_Right:
        humanConsole->keyRight = true;
        break;
    case Qt::Key_Shift:
        humanConsole->keyShift = true;
        break;
    case Qt::Key_Z:
        humanConsole->keyZ = true;
        break;
    case Qt::Key_X:
        humanConsole->keyX = true;
        break;
    }
}

void AirBattle::keyReleaseEvent(QKeyEvent *e) {
    switch(e->key()) {
    case Qt::Key_Up:
        humanConsole->keyUp = false;
        break;
    case Qt::Key_Down:
        humanConsole->keyDown = false;
        break;
    case Qt::Key_Left:
        humanConsole->keyLeft = false;
        break;
    case Qt::Key_Right:
        humanConsole->keyRight = false;
        break;
    case Qt::Key_Shift:
        humanConsole->keyShift = false;
        break;
    case Qt::Key_Z:
        humanConsole->keyZ = false;
        break;
    case Qt::Key_X:
        humanConsole->keyX = false;
        break;
    }
}

void AirBattle::start()
{
//    record->Play();
    humanConsole->run();
}
