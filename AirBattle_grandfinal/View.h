#ifndef VIEW_H
#define VIEW_H

#include <const.h>
#include <QObject>
#include <QPointF>
#include <Element.h>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QPixmap>
#include <QtOpenGL>
#include <QRgb>
#include <QKeyEvent>

#define TEX0_WIDTH 8
#define TEX0_HEIGHT 25

class View : public QGLWidget
{
    Q_OBJECT
public:
    View(QWidget* parent = 0);

public:
    Element ElementList[MAX_ELEMENT_NUM];
    int is_Bomb;
    int n;
    bool end; // 游戏结束

    Element GIF[MAX_ELEMENT_NUM];
    int GN;
    double GT;
    QPointF GPos;
    static const double during = 100;

signals:
    void call_record();

public:
    QTimer* timer;
    int time_int;
    void speedUp();
    void speedDown();

    void Bomb();
    void setBomb(QPointF);
    void SpeedUP();
    void setSpeedUP(QPointF);

private slots:
    void OnTimer();
    void stop();
    bool isStop();
    void STOP();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GameInit();

    void DrawElement(Element* a, bool alpha = false);

    QPointF ViewToOpenGL(QPointF);
    QPointF SizeToOpenGL(QPointF);

    GLuint texture[100]; // 贴图
    int nowtex; // 当前纹理
    int SPEED_COUNT; // 计数器
    void loadSingleTexture(QString file, GLuint &u, double alpha = 1);
    void loadGLTextures();
};

#endif // VIEW_H
