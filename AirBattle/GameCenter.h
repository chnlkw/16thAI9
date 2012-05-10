#ifndef GAMECENTER_H
#define GAMECENTER_H

#include <View.h>
#include <vector>

using namespace std;

class GameCenter : public QObject
{
    Q_OBJECT
public:
    GameCenter(View *view);

private:
    void GameInit();

private:
    int width, height;
    View *view;
    bool pause;

private:
    float Angle(QPointF);

public:
    void addBullet(QPointF pos, QPointF v);
    void addPlaneBullet(QPointF pos, QPointF v);

    void init(QPointF player_pos, QPointF boss_pos);

    void clear();

    void ElementMoveTo(int i, QPointF pos, float time);

    void Pause();
    void Continue();
    void Record_Over();

    void Bomb();

    void getBulletsPos(vector<QPointF>& pos);
};

#endif // GAMECENTER_H
