#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <assert.h>
#include "const.h"
#include <QThread>
#include <QTimer>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

class ServerTimer : public QThread
{
    Q_OBJECT
public:
    explicit ServerTimer(QObject *parent = 0);
    static void msleep(unsigned long secs) { QThread::msleep(secs); }
};

#endif // UTILS_H
