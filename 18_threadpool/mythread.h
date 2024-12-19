#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QVector>
#include <QObject>
#include <QRunnable>

class Generator : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Generator(QObject *parent = nullptr);

    void run();
signals:
    void genFinish(QVector<int> list);
public slots:
    void recvNum(int num);
private:
    int my_num;
};
class QuickSort : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit QuickSort(QObject *parent = nullptr);

    void run();
signals:
    void sortFinish(QVector<int> list);
public slots:
    void recvList(QVector<int> list);
private:
    QVector<int> my_list;
};
#endif // MYTHREAD_H
