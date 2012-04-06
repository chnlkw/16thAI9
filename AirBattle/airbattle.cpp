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
    view = new View(ui->frame);
    gamecenter = new GameCenter(view);
    record = new Record(gamecenter);
    filecenter = new FileCenter;
    this->setGeometry(100, 50, 768, 576);
    connect(view, SIGNAL(call_record()), record, SLOT(ViewTimer()));
    setFocusPolicy(Qt::StrongFocus);

    // 创建线程
    QThread * thread = new QThread(this);
    server = new Server;
    server->moveToThread(thread);

    connect(server, SIGNAL(change(int,int)), this, SLOT(recv_server(int,int)));
    connect(this, SIGNAL(reset()), server, SLOT(reset()));

    thread->start();
    // 线程创建完毕

    connect(record, SIGNAL(setTime(int)), this, SLOT(setTime(int)));
    connect(record, SIGNAL(setScore(int)), this, SLOT(setScore(int)));
    connect(record, SIGNAL(setPlayerWord(QString)), this, SLOT(setPlayerWord(QString)));
    connect(record, SIGNAL(setBossWord(QString)), this, SLOT(setBossWord(QString)));
    connect(record, SIGNAL(recordOver()), this, SLOT(Record_Over()));

    ui->frame->setGeometry(0, 0, 768, 576);
    ui->frame->hide();
    ui->frame2->setGeometry(0, 0, 768, 576);
    ui->frame2->hide();

    ui->left->hide();
    ui->right->hide();
    ui->SButton->hide();

    ui->left->setPixmap(QPixmap("res/left.png"));
    ui->right->setPixmap(QPixmap("res/right.png"));

    ui->menu->setPixmap(QPixmap("res/menu.png"));
    ui->menu->setGeometry(0, 0, 768, 576);
    //ui->menu->hide();

    ui->score->raise();
    ui->timem->raise();
    ui->times->raise();
    ui->timed->raise();
    ui->boss_name->raise();
    ui->player_name->raise();

    ui->score->setGeometry(616, 92, 120, 30);
    ui->timem->setGeometry(604, 200, 40, 40);
    ui->times->setGeometry(664, 200, 40, 40);
    ui->timed->setGeometry(725, 200, 20, 40);
    ui->boss_name->setGeometry(620, 280, 100, 80);
    ui->player_name->setGeometry(620, 410, 100, 80);

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

    on_AI_clicked();

    swaptoframe2();

    Door_Open();
}

void AirBattle::setTime(int x)
{
    int n = x;
    int min = n/600;
    int sec = n%600/10;
    int dd = n%10;
    if (sec < 10)
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

void AirBattle::setBossWord(QString x)
{
    ui->boss_word->setText(x);
}

void AirBattle::setPlayerName(QString x)
{
    ui->player_name->setText(x);
}

void AirBattle::setPlayerWord(QString x)
{
    ui->player_word->setText(x);
}

void AirBattle::on_record_clicked()
{
    state = 1;
    p1 = p2 = "";
    ui->start->setText(tr("Replay!"));

    QStringList ans = filecenter->Record_List();
    ui->list->clear();
    ui->list->addItems(ans);
    if (ans.size() != 0)
    {
        rec = ans[0];
        ui->list->setCurrentRow(0);
    }
    setRecord(rec);
}

void AirBattle::on_AI_clicked()
{
    state = 0;
    rec = "";
    ui->start->setText(tr("Fight!"));

    QStringList ans = filecenter->AI_List();
    ui->list->clear();
    ui->list->addItems(ans);
    if (ans.size() != 0)
    {
        p1 = p2 = ans[0];
        ui->list->setCurrentRow(0);
    }
    setVS(p1, p2);
}

void AirBattle::on_start_clicked()
{
    if (state == 0)
    {
        if (p1 == "" || p2 == "") return;
        QString Boss = "ai/" + p1 + "/aiBoss.dll";
        QString Player = "ai/" + p2 + "/aiPlane.dll";
        QString gb = "bin/aiBoss.dll";
        QString gp = "bin/aiPlane.dll";

        if (QFile::exists(gb)) QFile::remove(gb);
        if (QFile::exists(gp)) QFile::remove(gp);

        QFile::copy(Boss, gb);
        QFile::copy(Player, gp);

        emit reset();

        state = 3;
        Door_Close();
    }

    if (state == 1)
    {
        if (rec == "") return;

        ui->viewstart->setText(tr("Play"));
        state = 4;
        playrecord(rec);
        Door_Close();
    }
}

void AirBattle::playrecord(QString file)
{
    file = "replay/" + file + ".txt";
    setBossName(record->boss);
    setPlayerName(record->player);
    record->Play_Record(file);
}

void AirBattle::setState(QString x)
{
    ui->State_Label->setText(x);
}

void AirBattle::setVS(QString a, QString b)
{
    p1 = a, p2 = b;
    setState(a + " vs " + b);
}

void AirBattle::setRecord(QString a)
{
    rec = a;
    setState(a);
}

void AirBattle::on_list_pressed(const QModelIndex &index)
{
    if (state == 0)
    {
        p1 = ui->list->item(index.row())->text();
        p2 = "?";
        setVS(p1, p2);
    }
}

void AirBattle::on_list_entered(const QModelIndex &index)
{
    if (state == 0)
    {
        p2 = ui->list->item(index.row())->text();
        setVS(p1, p2);
    }
}

void AirBattle::on_list_clicked(const QModelIndex &index)
{
    if (state == 1)
    {
        rec = ui->list->item(index.row())->text();
        setRecord(rec);
    }
}

void AirBattle::Door_Open(int Delay)
{
    ui->left->setGeometry(0, 0, 768, 288);
    ui->right->setGeometry(0, 288, 768, 288);
    ui->left->show();
    ui->right->show();

    QEasingCurve curve(QEasingCurve::InOutCubic);

    QPropertyAnimation *left = new QPropertyAnimation(ui->left, "geometry");
    left->setDuration(Delay);
    left->setEndValue(QRect(0, -288, 768, 288));
    left->setEasingCurve(curve);
    left->start();

    QPropertyAnimation *right = new QPropertyAnimation(ui->right, "geometry");
    right->setDuration(Delay);
    right->setEndValue(QRect(0, 288*2, 768, 288));
    right->setEasingCurve(curve);
    right->start();
    connect(right, SIGNAL(finished()), this, SLOT(Door_Open_End()));
}

void AirBattle::Door_Close(int Delay)
{
    ui->left->setGeometry(0, -288, 768, 288);
    ui->right->setGeometry(0, 288*2, 768, 288);
    ui->left->show();
    ui->right->show();

    QEasingCurve curve(QEasingCurve::OutBounce);
    curve.setAmplitude(0.3);

    QPropertyAnimation *left = new QPropertyAnimation(ui->left, "geometry");
    left->setDuration(Delay);
    left->setEndValue(QRect(0, 0, 768, 288));
    left->setEasingCurve(curve);
    left->start();

    QPropertyAnimation *right = new QPropertyAnimation(ui->right, "geometry");
    right->setDuration(Delay);
    right->setEndValue(QRect(0, 288, 768, 288));
    right->setEasingCurve(curve);
    right->start();
    connect(right, SIGNAL(finished()), this, SLOT(Door_Close_End()));
}

void AirBattle::Door_Open_End()
{
    if (state == 4)
    {
        playrecord(rec);
        record->Play();
    }
}

void AirBattle::Door_Close_End()
{
    if (state == 1)
    {
        swaptoframe2();
        Door_Open();
        on_AI_clicked();
        return;
    }

    if (state == 3)
    {
        QString boss      = "start bin/userAI_Boss.exe bin/aiBoss.dll";
        QString player    = "start bin/userAI_Plane.exe bin/aiPlane.dll";
        QString plateform = "start bin/platform.exe 1 replay.txt 3000 100";

        system(plateform.toStdString().c_str());
        system(boss.toStdString().c_str());
        system(player.toStdString().c_str());

        //SButton_Drop();
        return;
    }

    if (state == 4)
    {
        swaptoframe();
        Door_Open();
        return;
    }

    if (state == -1)
    {
        swaptoframe2();
        Door_Open();
        on_AI_clicked();
        return;
    }
}

void AirBattle::SButton_Drop(int Delay)
{
    QEasingCurve curve(QEasingCurve::OutElastic);
    curve.setAmplitude(0.5);

    ui->SButton->setGeometry(QRect(-100,400,100,100));
    ui->SButton->show();
    QPropertyAnimation *SButton = new QPropertyAnimation(ui->SButton, "geometry");
    SButton->setDuration(Delay);
    SButton->setEndValue(QRect(325, 400, 100, 100));
    SButton->setEasingCurve(curve);
    SButton->start();
}

void AirBattle::swaptoframe()
{
    ui->frame2->hide();
    ui->frame->show();
}

void AirBattle::swaptoframe2()
{
    ui->frame->hide();
    ui->frame2->show();
}

void AirBattle::on_viewstart_clicked()
{
    if (rec == "") return;

    if (state == 4)
    {
        ui->viewstart->setText(tr("Pause"));
        record->Play();
        state = 5;
        return;
    }

    if (state == 5)
    {
        state = 7;
        return;
    }

    if (state == 6)
    {
        playrecord(rec);
        ui->viewstart->setText(tr("Pause"));
        record->Play();
        state = 5;
        return;
    }

    if (state == 7)
    {
        state = 5;
        return;
    }
}

void AirBattle::Record_Over()
{
//    state = 6;
//    ui->viewstart->setText(tr("Again"));

    MyTimer::msleep(2000);
    state = 1;
    Door_Close();
}

void AirBattle::on_viewback_clicked()
{
    if (state == 5)
    {
    }

    Door_Close();
    state = 1;
}

void AirBattle::recv_server(int round, int score)
{
    if (round == 0 && score == 0)
    {
        int m = QDate::currentDate().month();
        int d = QDate::currentDate().day();
        int h = QTime::currentTime().hour();
        int mm = QTime::currentTime().minute();
        int s = QTime::currentTime().second();
        QString ans;
        ans =  QString::number(m) + "-" + QString::number(d) + " " + QString::number(h) + "-" + QString::number(mm) + "-" + QString::number(s) + " " + p1 + " vs " + p2;
        QFile::copy("replay.txt", "replay/" + ans + ".txt");
        QFile::remove("replay.txt");
        //SButton_Drop();
        rec = ans;

        ui->viewstart->setText(tr("Play"));
        state = 4;
        playrecord(rec);

        swaptoframe();
        Door_Open();
    }
    else
    {
        //ui->left->setText(QString::number(round));
        //ui->right->setText(QString::number(score));
    }
}


void AirBattle::on_SButton_clicked()
{
    QEasingCurve curve(QEasingCurve::InOutQuint);

    ui->SButton->setGeometry(QRect(325, 400, 100, 100));
    ui->SButton->show();
    QPropertyAnimation *SButton = new QPropertyAnimation(ui->SButton, "geometry");
    SButton->setDuration(500);
    SButton->setEndValue(QRect(325,751,100,100));
    SButton->setEasingCurve(curve);
    SButton->start();
}

void AirBattle::on_SButton_Removed()
{
    ui->SButton->hide();

    ui->viewstart->setText(tr("Play"));
    state = 4;
    playrecord(rec);

    swaptoframe();
    Door_Open();
}

void AirBattle::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Up:
        view->speedUp();
        break;

    case Qt::Key_Down:
        view->speedDown();
        break;
    case Qt::Key_Escape:
        view->timer->stop();
        state = -1;
        Door_Close();
        break;
    case Qt::Key_Space:
        if (view->timer->isActive())
            view->timer->stop();
        else
            view->timer->start();
    }
    qDebug() << e->key();
}
