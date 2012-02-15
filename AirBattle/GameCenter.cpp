#include <GameCenter.h>

GameCenter::GameCenter(View *view)
{
    this->view = view;
    width = 450;
    height = 600;

    GameInit();
}

void GameCenter::GameInit()
{
    view->setGeometry(5, 28, width, height);

    clear();
}

void GameCenter::addBullet(QPointF pos, QPointF v)
{
    Element &bullet = view->ElementList[view->n++];
    bullet.SetInit(QPointF(pos), QPointF(BULLET_WIDTH, BULLET_HEIGTH));
    bullet.SetV(v);
    bullet.SetTex(2);
    bullet.A = Angle(v)/PI*180;
}

float GameCenter::Angle(QPointF a)
{
    if (fabs(a.x()) < 1e-6) return a.y() > 0 ? 0 : PI;
    float q = atan(a.y()/a.x()) - PI / 2.0;
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
    background.SetInit(QPointF(WIDTH/2, HEIGTH/2), QPointF(WIDTH, HEIGTH));
    background.SetTex(1);

    // 1£ºplayer
    Element &player = view->ElementList[view->n++];
    player.SetInit(player_pos, QPointF(PLAYER_WIDTH, PLAYER_HEIGTH));
    player.SetTex(3);

    // 2£ºboss
    Element &boss = view->ElementList[view->n++];
    boss.SetInit(boss_pos, QPointF(BOSS_WIDTH, BOSS_HEIGTH));
}

void GameCenter::ElementMoveTo(int i, QPointF pos, float time)
{
    view->ElementList[i].MoveTo(pos, time);
}
