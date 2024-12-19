#include "mythread.h"
#include <QDebug>
#include <QVector>
#include <QRandomGenerator>

Generator::Generator(QObject *parent)
    : QObject{parent}, QRunnable{}
{
    setAutoDelete(false);
}

void Generator::run()
{
    qDebug() << "生成随机数线程的地址 GenThread(): " << QThread::currentThread();
    QVector<int> list;
    QElapsedTimer time;
    for (int i = 0; i < my_num; i++)
    {
        list.push_back(QRandomGenerator::global()->bounded(100, 10000));
    }
    auto cost = time.elapsed();
    qDebug() << "生成随机数耗费: " << cost << " 毫秒";
    emit genFinish(list);
}

void Generator::recvNum(int num)
{
    my_num = num;
}

QuickSort::QuickSort(QObject *parent)
    : QObject{parent}, QRunnable{}
{
    setAutoDelete(false);
}

void QuickSort::run()
{
    // sort(my_list.begin(), my_list.end(), std::less<int>());
    sort(my_list.begin(), my_list.end(), std::greater<int>());
    emit sortFinish(my_list);
}

void QuickSort::recvList(QVector<int> list)
{
    my_list = list;
}
