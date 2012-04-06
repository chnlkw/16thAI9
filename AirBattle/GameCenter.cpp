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
    view->setGeometry(31, 25, width, height);

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
    bullet.SetInit(QPointF(pos), QPointF(PLANE_BULLET_WIDTH, PLANE_BULLET_HEIGHT));
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

    // 3£º´óÕÐ
    Element &big = view->ElementList[view->n++];
    big.SetInit(QPointF(300, 1025), QPointF(1500, 20));
    for (int i = 0; i < 6; i++) big.pushTex(i + 10);
    big.startTex();
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
    view->time_int = 20;
    view->Continue();
    view->is_Bomb = false;
}

void GameCenter::Record_Over()
{
    view->Pause();
}

void GameCenter::Bomb()
{
    view->is_Bomb = true;
    view->ElementList[3].pos = QPointF(300, 1030);
    ElementMoveTo(3, QPointF(300, -30), 1.5);
}
