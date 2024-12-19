#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QVector>
class Generator : public QObject
{
    Q_OBJECT
public:
    explicit Generator(QObject *parent = nullptr);
public:
    void generate(int num);
signals:
    void genFinish(QVector<int> list);

};
class QuickSort : public QObject
{
    Q_OBJECT
public:
    explicit QuickSort(QObject *parent = nullptr);
public:
    void sort(QVector<int> list);
signals:
    void sortFinish(QVector<int> list);

};
#endif // MYTHREAD_H
