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
#include <qgl.h>
#include <QRgb>
#include <QKeyEvent>

class View : public QGLWidget
{
    Q_OBJECT
public:
    View(QWidget* parent = 0);

public:
    Element ElementList[MAX_ELEMENT_NUM];
    bool is_Bomb;
    int n;

    void Pause();
    void Continue();

signals:
    void call_record();

public:
    QTimer* timer;
    int time_int;
    void speedUp();
    void speedDown();

    void keyPressEvent(QKeyEvent *e);
    void Bomb();

private slots:
    void OnTimer();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void GameInit();

    void DrawElement(Element* a);

    QPointF ViewToOpenGL(QPointF);
    QPointF SizeToOpenGL(QPointF);

    GLuint texture[100]; // Ã˘Õº
    int nowtex; // µ±«∞Œ∆¿Ì
    void loadSingleTexture(QString file, GLuint &u, double alpha = 1);
    void loadGLTextures();
};

#endif // VIEW_H
