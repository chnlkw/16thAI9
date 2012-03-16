#include <QtGui/QApplication>
#include "airbattle.h"
 #include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"main thread: "<< QThread::currentThreadId();

    AirBattle w;
    w.show();

    return a.exec();
}
