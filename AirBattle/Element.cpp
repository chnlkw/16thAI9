#include <Element.h>

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
    if (speed)
    {
        pos += v * delta;
    }
    if (moveto)
    {
        float len = length(Goal - pos);
        float vlen = length(v * delta);
        if (vlen >= len)
        {
            pos = Goal;
            moveto = false;
        }
        else
        {
            pos += v * delta;
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
    moveto = true;
}

bool Element::visual()
{
    // x方向
    if (pos.x()+size.x()/2.0+WIDTH/8 < 0) return false;
    if (pos.x()-size.x()/2.0-WIDTH/8 > WIDTH) return false;

    // y方向
    if (pos.y()+size.y()/2.0+HEIGTH/8 < 0) return false;
    if (pos.y()-size.y()/2.0-HEIGTH/8 > HEIGTH) return false;

    return true;
}

void Element::SetTex(int u)
{
    tex = u;
}
