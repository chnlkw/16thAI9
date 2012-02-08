#include <QtGui/QApplication>
#include "airbattle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AirBattle w;
    w.show();

    return a.exec();
}
