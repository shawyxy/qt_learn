#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QVector>
class GenThread : public QThread
{
    Q_OBJECT
public:
    explicit GenThread(QObject *parent = nullptr);
protected:
    void run();
signals:
    void genFinish(QVector<int> list);
public slots:
    void recvNum(int num);
private:
    int my_num;
};
class SortThread : public QThread
{
    Q_OBJECT
public:
    explicit SortThread(QObject *parent = nullptr);
protected:
    void run();
signals:
    void sortFinish(QVector<int> list);
public slots:
    void recvList(QVector<int> list);
private:
    QVector<int> my_list;
};
#endif // MYTHREAD_H
