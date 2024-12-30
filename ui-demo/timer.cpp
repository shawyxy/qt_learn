
#include "timer.h"
#include <QDebug>

Timer::Timer(QObject *parent)
    : QObject(parent), timer(new QTimer(this))
{
    // 连接计时器的超时信号到工作逻辑
    connect(timer, &QTimer::timeout, this, &Timer::workProgress);
}

Timer::~Timer()
{
    stopWork();
}

void Timer::startWork()
{
    // 启动计时器
    qDebug() << "Starting work...";
    timer->start(1000); // 设置计时器为每秒触发一次
}

void Timer::stopWork()
{
    // 停止计时器
    if (timer->isActive())
    {
        qDebug() << "Stopping work...";
        timer->stop();
    }
}
