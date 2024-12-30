#ifndef TIMER_H
#define TIMER_H

#include <QDebug>
#include <QObject>
#include <QTimer>

class Timer : public QObject {
  Q_OBJECT
public:
  explicit Timer(QObject *parent = nullptr) : QObject(parent) {}
  void timerStart() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Timer::onTimeout);
    timer->start(1000);
  }

signals:
  void timeoutSignal();

public slots:
  void onTimeout() { emit timeoutSignal(); }

private:
  QTimer *timer;
};

#endif // TIMER_H
