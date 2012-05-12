#include <GameCenter.h>

GameCenter::GameCenter(View *view)
{
    this->view = view;

    GameInit();
}

void GameCenter::GameInit()
{
    view->setGeometry(41, 33, 701, 701);

    clear();
}

void GameCenter::addBullet(QPointF pos, QPointF v)
{
    Element &bullet = view->ElementList[view->n++];
    bullet.SetInit(QPointF(pos), QPointF(BULLET_WIDTH, BULLET_HEIGTH));
    bullet.SetV(v);
    bullet.A = Angle(v)/PI*180;
    bullet.moveto = 0;
    bullet.speed = true;

    double xx = sqrt(v.x()*v.x() + v.y()*v.y())/10;
    if (xx > 47)
        bullet.SetTex(21);
    else if (xx > 43)
        bullet.SetTex(22);
    else if (xx > 39)
        bullet.SetTex(23);
    else if (xx > 35)
        bullet.SetTex(24);
    else
        bullet.SetTex(25);
}

void GameCenter::addPlaneBullet(QPointF pos, QPointF goal)
{
    Element &bullet = view->ElementList[view->n++];
    bullet.SetInit(QPointF(pos), QPointF(PLANE_BULLET_WIDTH, PLANE_BULLET_HEIGHT));
    bullet.A = 0;
    bullet.SetTex(5);

    bullet.Start = pos + QPointF(0, 25);
    bullet.Goal = goal;
    bullet.Control = QPointF(bullet.Start.x(),0.1*bullet.Start.y()+0.9*bullet.Goal.y());
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
    view->n = 0;

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

    view->end = false;
    view->timer->start();
}

void GameCenter::ElementMoveTo(int i, QPointF pos, float time)
{
    view->ElementList[i].MoveTo(pos, time);
}

void GameCenter::Bomb(QPointF pos)
{
    view->setBomb(pos);
}

void GameCenter::SpeedUP(QPointF pos)
{
    view->setSpeedUP(pos);
}

void GameCenter::getBulletsPos(vector<QPointF> &pos) {
    for (int i = 3; i < view->n; i ++) {
        if (view->ElementList[i].moveto == 0)
            pos.push_back(view->ElementList[i].pos);
    }
}
