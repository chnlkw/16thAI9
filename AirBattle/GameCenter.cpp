#include <GameCenter.h>

GameCenter::GameCenter(View *view)
{
    this->view = view;
    width = 525;
    height = width;

    GameInit();
    view->Pause(); // Ä¬ÈÏÔÝÍ£
}

void GameCenter::GameInit()
{
    view->setGeometry(55, 27, width, height);
    connect(view, SIGNAL(Finished()), this, SLOT(RecvFinished()));

    clear();
}

void GameCenter::addBullet(QPointF pos, QPointF v)
{
    Element &bullet = view->ElementList[view->n++];
    bullet.SetInit(QPointF(pos), QPointF(BULLET_WIDTH, BULLET_HEIGTH));
    bullet.SetV(v);
    bullet.SetTex(2);
    bullet.A = Angle(v)/PI*180;
    bullet.moveto = 0;
    bullet.speed = true;
}

void GameCenter::addPlaneBullet(QPointF pos, QPointF goal)
{
    Element &bullet = view->ElementList[view->n++];
    bullet.SetInit(QPointF(pos), QPointF(20, 35));
    bullet.A = 0;
    bullet.SetTex(5);

    bullet.Start = pos;
    bullet.Goal = goal;
    bullet.Control = QPointF(pos.x(),goal.y());
    bullet.moveto = 2;
    bullet.speed = true;
    bullet.t = 0;
}

float GameCenter::Angle(QPointF a)
{
    if (fabs(a.x()) < 1e-6) return a.y() > 0 ? 0 : PI;
    float q;
    if (a.x() > 0)
        q = atan(a.y()/a.x());
    else
        q = atan(a.y()/a.x()) + PI;
    q = q - PI / 2;
    return q;
}

void GameCenter::clear()
{
    view->n = 0;
}

void GameCenter::init(QPointF player_pos, QPointF boss_pos)
{
    // 0£º±³¾°
    Element &background = view->ElementList[view->n++];
    background.SetInit(QPointF(WIDTH1/2, HEIGTH1/2), QPointF(WIDTH1, HEIGTH1));
    background.SetTex(0);

    // 1£ºplayer
    Element &player = view->ElementList[view->n++];
    player.SetInit(player_pos, QPointF(PLAYER_WIDTH, PLAYER_HEIGTH));
    player.SetTex(3);

    // 2£ºboss
    Element &boss = view->ElementList[view->n++];
    boss.SetInit(boss_pos, QPointF(BOSS_WIDTH, BOSS_HEIGTH));
    boss.SetTex(1);
}

void GameCenter::ElementMoveTo(int i, QPointF pos, float time)
{
    view->ElementList[i].MoveTo(pos, time);
}

void GameCenter::Pause()
{
    view->Pause();
}

void GameCenter::Continue()
{
    view->Continue();
}

void GameCenter::Record_Over()
{
    view->check = true;
    view->Pause();
}

void GameCenter::RecvFinished()
{
    emit Finished();
}
