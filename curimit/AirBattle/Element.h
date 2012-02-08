#ifndef ELEMENT_H
#define ELEMENT_H

#include <const.h>
#include <qmath.h>
#include <QPointF>

class Element
{
public:
    QPointF pos; // 中心坐标
    QPointF size; // size.x宽度，size.y高度
    float A; // 旋转角度

    QPointF Goal; // 目标位置

    QPointF v; // 速度
    bool speed; // 是否有移动速度
    bool moveto; // 是否是移动到

    int tex; // 贴图

    Element();

    void SetInit(QPointF p, QPointF s, float a = 0);

    void SetV(QPointF vv);

    void go(float delta);

    void SetPos(QPointF p);

    void SetTex(int u);

    void MoveTo(QPointF p, float time);

    bool visual();
};

#endif // ELEMENT_H
