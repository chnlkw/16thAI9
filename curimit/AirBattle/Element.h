#ifndef ELEMENT_H
#define ELEMENT_H

#include <const.h>
#include <qmath.h>
#include <QPointF>

class Element
{
public:
    QPointF pos; // ��������
    QPointF size; // size.x��ȣ�size.y�߶�
    float A; // ��ת�Ƕ�

    QPointF Goal; // Ŀ��λ��

    QPointF v; // �ٶ�
    bool speed; // �Ƿ����ƶ��ٶ�
    bool moveto; // �Ƿ����ƶ���

    int tex; // ��ͼ

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
