#include <View.h>
#include <QDebug>


View::View(QWidget *parent)
{
    setParent(parent);
    GameInit();
    show();
}

double sqr(double x)
{
    return x*x;
}

double dist(QPointF a, QPointF b)
{
    return sqrt(sqr(a.x() - b.x()) + sqr(a.y() - b.y()));
}

void View::GameInit()
{
    nowtex = -1;
    n = 0;
    GN = 0;
    is_Bomb = 0;
    end = false;
    SPEED_COUNT = -1;

    // 定义时钟
    timer = new QTimer;
    time_int = 20;
    timer->setInterval(time_int);
    timer->start();
    timer->connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void View::loadSingleTexture(QString file, GLuint &u, double alpha)
{
    QPixmap buf;
    QImage tex;
    if (!buf.load(file))
    {
        QImage dummy(128, 128, QImage::Format_ARGB32);
        dummy.fill(0);
        buf.fromImage(dummy);
        qDebug() << "error!";
    }
    QImage tmp = buf.toImage();
    if (alpha < 0.9999)
    {
        for (int i = 0; i < tmp.width(); i++)
            for (int j = 0; j < tmp.height(); j++)
            {
                QColor color = tmp.pixel(i, j);
                int r = qRed(tmp.pixel(QPoint(i,j))), b = qBlue(tmp.pixel(QPoint(i,j))), g = qGreen(tmp.pixel(QPoint(i,j))), a = qAlpha(tmp.pixel(QPoint(i,j)));
                if (a == 0) continue;
                color.setRgba(qRgba(r,g,b,a));
                tmp.setPixel(i, j, color.rgba());
            }
    }
    tex = QGLWidget::convertToGLFormat(tmp);
    glGenTextures(1, &u);
    glBindTexture(GL_TEXTURE_2D, u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void View::loadGLTextures()
{
    loadSingleTexture("res/background.png", texture[0]);
    loadSingleTexture("res/boss.png", texture[1]);
    loadSingleTexture("res/bullet.png", texture[2], 0.5);
    loadSingleTexture("res/plane.png", texture[3]);
    loadSingleTexture("res/frame.png", texture[4]);
    loadSingleTexture("res/plane_bullet.png", texture[5]);
    loadSingleTexture("res/tex0.png", texture[6]);
    loadSingleTexture("res/plane_speedup.png", texture[7]);

    // 大招贴图
    loadSingleTexture("res/line0.png", texture[10]);
    loadSingleTexture("res/line1.png", texture[11]);
    loadSingleTexture("res/line2.png", texture[12]);
    loadSingleTexture("res/line3.png", texture[13]);
    loadSingleTexture("res/line4.png", texture[14]);
    loadSingleTexture("res/line5.png", texture[15]);

    // 子弹
    loadSingleTexture("res/bullet_1.png", texture[21]);
    loadSingleTexture("res/bullet_2.png", texture[22]);
    loadSingleTexture("res/bullet_3.png", texture[23]);
    loadSingleTexture("res/bullet_4.png", texture[24]);
    loadSingleTexture("res/bullet_5.png", texture[25]);
}

void View::initializeGL()
{
    loadGLTextures();
    glEnable(GL_TEXTURE_2D);

    glShadeModel(GL_SMOOTH);
    glClearColor(1,1,1,0);
    glClearDepth(1);
    //glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3f);

    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glHint(GL_LINE_SMOOTH, GL_NICEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glDisable(GL_DEPTH_TEST);

    //glEnableClientState( GL_VERTEX_ARRAY );

    //glEnableClientState( GL_TEXTURE_COORD_ARRAY );
}

void View::resizeGL(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, (GLint)w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void View::DrawElement(Element *a, bool alpha)
{
    QPointF pos = ViewToOpenGL(a->pos);
    QPointF size = SizeToOpenGL(a->size);
    float w = size.x() / 2.0;
    float h = size.y() / 2.0;
    int u = a->tex;

    glPushMatrix();
    glTranslated((GLdouble)pos.x(), (GLdouble)pos.y(), -12.07);
    glRotated(a->A, 0, 0, 1);

    if (alpha)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 1.0, 0.5);
        glBegin(GL_QUADS);
            glVertex3f(-w, h, 0);
            glVertex3f(w, h, 0);
            glVertex3f(w, -h, 0);
            glVertex3f(-w, -h, 0);
        glEnd();
        glDisable(GL_BLEND);
    }

    if (u != nowtex)
    {
        glBindTexture(GL_TEXTURE_2D, texture[u]);
        nowtex = u;
    }

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex3f(-w, h, 0);
        glTexCoord2f(1, 1); glVertex3f(w, h, 0);
        glTexCoord2f(1, 0); glVertex3f(w, -h, 0);
        glTexCoord2f(0, 0); glVertex3f(-w, -h, 0);
    glEnd();

    glPopMatrix();
}

void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 画元素
    for (int i = 0; i < 3; i++)
        DrawElement(&ElementList[i]);

    for (int i = 3; i < n; i++)
        DrawElement(&ElementList[i]);

    for (int i = 0 ; i < GN; i++)
        DrawElement(&GIF[i]);

}

void View::OnTimer()
{
    if (end && isStop()) return;

    if (is_Bomb == 1)
    {
        Bomb();
        update();
        return;
    }
    if (is_Bomb == 2)
    {
        SpeedUP();
        update();
        return;
    }
    if (SPEED_COUNT >= 0)
    {
        if (SPEED_COUNT == 0) ElementList[1].tex = 3;
        SPEED_COUNT--;
    }
    // 移动
    for (int i = 0; i < n; i++)
        ElementList[i].go(1.0/50.0);

    // 清除出界元素
    for (int i = 3; i < n; i++)
        if (!ElementList[i].visual())
            ElementList[i--] = ElementList[--n];
    update();
    emit call_record();
}

QPointF View::ViewToOpenGL(QPointF tmp)
{
    tmp.setX((tmp.x() - WIDTH1/2) / 100.0);
    tmp.setY((tmp.y() - HEIGTH1/2) / 100.0);
    return tmp;
}

QPointF View::SizeToOpenGL(QPointF tmp)
{
    tmp.setX(tmp.x() / 100.0);
    tmp.setY(tmp.y() / 100.0);
    return tmp;
}

void View::speedUp()
{
    if (time_int > 1)
    {
        time_int /= 2;
        timer->setInterval(time_int);
        timer->stop();
        timer->start();
    }
}

void View::speedDown()
{
    if (time_int < 10000)
    {
        time_int *= 2;
        timer->setInterval(time_int);
    }
}

void View::Bomb()
{
    for (int i = 0; i < GN; i++)
        GIF[i].go(0.6 / during);

    if (GT >= 0.2)
    {
        double dd = dist(GIF[0].pos, GPos);
        for (int i = 3; i < n; i++)
        {
            if (ElementList[i].moveto == 2) continue;
            double ddd = dist(ElementList[i].pos, GPos);
            if (ddd > dd) continue;
            ElementList[i--] = ElementList[--n];
        }
    }

    // 判断是否结束大招
    if (GT >= 1)
    {
        is_Bomb = 0;
        for (int i = 3; i < n; i++)
        {
            if (ElementList[i].moveto == 2) continue;
            ElementList[i--] = ElementList[--n];
        }
        GN = 0;
    }
    GT = GT + 0.6 / during;
}

void View::setBomb(QPointF pos)
{
    is_Bomb = 1;
    GT = 0;
    GN = 250;
    GPos = pos;
    for (int i = 0; i < GN; i++)
    {
        double t = (double) i / (double) GN;
        double r = 800;
        double xx = (sin(t*2*PI*4))*r;
        double x = r * cos(t*2*PI);
        double y = r * sin(t*2*PI);

        QPointF delta;
        delta.setX(x);
        delta.setY(y);

        Element& bullet = GIF[i];
        bullet.SetInit(QPointF(pos), QPointF(TEX0_WIDTH, TEX0_HEIGHT));
        bullet.A = 0;
        bullet.SetTex(6);

        bullet.pos = pos + delta/2;
        bullet.v = -delta*2;
        bullet.moveto = 3;
        bullet.speed = true;
        bullet.t = 0;
    }
}

void View::SpeedUP()
{
    for (int i = 0; i < GN; i++)
        GIF[i].go(1.0 / during);

    // 判断是否结束大招
    if (GT >= 1)
    {
        is_Bomb = 0;
        GN = 0;
        SPEED_COUNT = 50*3;
        ElementList[1].tex = 7;
    }
    GT = GT + 1.0 / during;
}


void View::setSpeedUP(QPointF pos)
{
    is_Bomb = 2;
    GT = 0;
    GN = 250;
    GPos = pos;
    for (int i = 0; i < GN; i++)
    {
        double t = (double) i / (double) GN;
        double r = 800;
        double xx = (0.2+0.8*fabs(sin(t*2*PI*4)))*r;
        double x = xx * cos(t*2*PI);
        double y = xx * sin(t*2*PI);

        QPointF delta;
        delta.setX(x);
        delta.setY(y);

        Element& bullet = GIF[i];
        bullet.SetInit(QPointF(pos), QPointF(TEX0_WIDTH, TEX0_HEIGHT));
        bullet.A = 0;
        bullet.SetTex(6);

        bullet.pos = pos + delta;
        bullet.v = -delta;
        bullet.moveto = 3;
        bullet.speed = true;
        bullet.t = 0;
    }
}

void View::stop()
{
    end = true;
}

bool View::isStop()
{
    QPointF now = ElementList[1].pos;
    for (int i = 3; i < n; i++)
    {
        if (ElementList[i].moveto == 2) continue;
        QPointF bullet = ElementList[i].pos;
        if (dist(now, bullet) < 100)
        {
            timer->stop();
//            STOP();
            return true;
        }
    }
    return false;
}

void View::STOP()
{

}
