#include "mythread.h"
#include <QDebug>
#include <QVector>


Generator::Generator(QObject *parent)
    : QObject{parent}
{}

void Generator::generate(int num)
{
    qDebug() << "生成随机数线程的地址 Generator(): " << QThread::currentThread();
    QVector<int> list;
    QElapsedTimer time;
    for (int i = 0; i < num; i++)
    {
        list.push_back(rand() % 1000);
    }
    auto cost = time.elapsed();
    qDebug() << "生成随机数耗费: " << cost << " 毫秒";
    emit genFinish(list);
}


QuickSort::QuickSort(QObject *parent)
    : QObject{parent}
{}

void QuickSort::sort(QVector<int> list)
{
    std::sort(list.begin(), list.end(), std::greater<int>());
    emit sortFinish(list);
}

