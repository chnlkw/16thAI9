#ifndef ELEMENT_H
#define ELEMENT_H

#include <const.h>
#include <qmath.h>
#include <QPointF>
#include <QVector>

class Element
{
public:
    QPointF pos; // ��������
    QPointF size; // size.x��ȣ�size.y�߶�
    float A; // ��ת�Ƕ�

    QPointF Start;
    QPointF Goal; // Ŀ��λ��
    QPointF Control;
    double t;

    QPointF v; // �ٶ�
    bool speed; // �Ƿ����ƶ��ٶ�
    int moveto; // �Ƿ����ƶ���


    int tex; // ��ͼ

    // �������
    QVector<int> TEX;
    int gif_count;
    bool gif;

    // �ӿں���
    void clearTex();
    void pushTex(int);
    void startTex();
    void stopTex(int);

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
