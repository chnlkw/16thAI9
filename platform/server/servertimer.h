#ifndef SERVERTIMERTHREAD_H
#define SERVERTIMERTHREAD_H

#include <QThread>
#include <QTimer>

class ServerTimer : public QThread
{
    Q_OBJECT
public:
    explicit ServerTimer(QObject *parent = 0);
    static void msleep(unsigned long secs) { QThread::msleep(secs); }
};

#endif // SERVERTIMERTHREAD_H
