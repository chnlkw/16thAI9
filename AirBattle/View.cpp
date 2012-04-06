#include <View.h>
#include <QDebug>

View::View(QWidget *parent)
{
    setParent(parent);
    GameInit();
    show();
}

void View::GameInit()
{
    nowtex = -1;
    n = 0;

    // ����ʱ��
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
    loadSingleTexture("res/player.png", texture[3]);
    loadSingleTexture("res/frame.png", texture[4]);
    loadSingleTexture("res/plane_bullet.png", texture[5]);

    // ������ͼ
    loadSingleTexture("res/line0.png", texture[10]);
    loadSingleTexture("res/line1.png", texture[11]);
    loadSingleTexture("res/line2.png", texture[12]);
    loadSingleTexture("res/line3.png", texture[13]);
    loadSingleTexture("res/line4.png", texture[14]);
    loadSingleTexture("res/line5.png", texture[15]);
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

void View::DrawElement(Element *a)
{
    QPointF pos = ViewToOpenGL(a->pos);
    QPointF size = SizeToOpenGL(a->size);
    float w = size.x() / 2.0;
    float h = size.y() / 2.0;
    int u = a->tex;

    glPushMatrix();
    glTranslated((GLdouble)pos.x(), (GLdouble)pos.y(), -12.07);
    glRotated(a->A, 0, 0, 1);

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

    // ��Ԫ��
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < n; i++)
        DrawElement(&ElementList[i]);
}

void View::OnTimer()
{
    if (is_Bomb)
    {
        // �ƶ�ElementList[4]
        ElementList[3].go(1.0/50.0);

        // ��������ӵ�
        for (int i = 4; i < n; i++)
            if (ElementList[i].tex == 2)
                if (ElementList[i].pos.y()+20 >= ElementList[3].pos.y())
                    ElementList[i--] = ElementList[--n];

        // �ж��Ƿ��������
        if (ElementList[3].pos.y() <= -25) is_Bomb = false;
        update();
        return;
    }
    // �ƶ�
    for (int i = 0; i < n; i++)
        ElementList[i].go(1.0/50.0);

    // �������Ԫ��
    for (int i = 4; i < n; i++)
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

void View::Pause()
{
    timer->stop();
}

void View::Continue()
{
    timer->start();
}

void View::speedUp()
{
    if (time_int > 1)
    {
        time_int /=2;
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

void View::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Up:
        speedUp();
        break;

    case Qt::Key_Down:
        speedDown();
        break;
    }
    qDebug() << e->key();
}

void View::Bomb()
{

}
