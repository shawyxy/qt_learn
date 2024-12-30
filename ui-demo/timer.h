
#ifndef THREADS_H
#define THREADS_H

#include <QObject>
#include <QTimer>
#include <QThread>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr);
    ~Timer();

public slots:
    void startWork();
    void stopWork();

signals:
    void workProgress();

private:
    QTimer *timer;
};

#endif // THREADS_H
