#include <Element.h>

float xAngle(QPointF a)
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

Element::Element()
{
    speed = false;
    tex = 0;
}

void Element::SetInit(QPointF p, QPointF s, float a)
{
    pos = p;
    size = s;
    A = a;
    speed = false;
    tex = 0;
    moveto = 0;
}

void Element::SetV(QPointF vv)
{
    speed = true;
    v = vv;
}

float length(QPointF a)
{
    return sqrt(a.x()*a.x() + a.y()*a.y());
}

void Element::go(float delta)
{
    if (!speed) return;

    if (moveto == 0)
    {
        pos += v * delta;
    }

    if (moveto == 1)
    {
        float len = length(Goal - pos);
        float vlen = length(v * delta);
        if (vlen >= len)
        {
            pos = Goal;
            speed = false;
        }
        else
        {
            pos += v * delta;
        }
    }

    if (moveto == 2)
    {
        t = t + 0.015;
        v = Start*2*(t-1) + Control*2*(1-2*t) + Goal*2*t;
        A = xAngle(v)/PI*180;
        if (t >= 1)
        {
            pos = Goal;
            speed = false;
        }
        else
        {
            pos = Start * (1-t)*(1-t) + Control*2*t*(1-t) + Goal*t*t;
        }
    }
}

void Element::SetPos(QPointF p)
{
    pos = p;
}

void Element::MoveTo(QPointF p, float time)
{
    Goal = p;
    v = (Goal - pos)/time;
    speed = true;
    moveto = 1;
}

bool Element::visual()
{
    if (moveto == 2 && !speed) return false;
    // x方向
    if (pos.x()+size.x()/2.0+WIDTH1/8 < 0) return false;
    if (pos.x()-size.x()/2.0-WIDTH1/8 > WIDTH1) return false;

    // y方向
    if (pos.y()+size.y()/2.0+HEIGTH1/8 < 0) return false;
    if (pos.y()-size.y()/2.0-HEIGTH1/8 > HEIGTH1) return false;

    return true;
}

void Element::SetTex(int u)
{
    tex = u;
}
