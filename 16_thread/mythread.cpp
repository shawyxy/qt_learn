#include "MyThread.h"
#include <QDebug>
#include <QVector>
#include <QtAlgorithms>
#include <iostream>
GenThread::GenThread(QObject *parent)
    : QThread{parent}
{}

void GenThread::run()
{
    qDebug() << "生成随机数线程的地址 GenThread(): " << QThread::currentThread();
    QVector<int> list;
    QElapsedTimer time;
    for (int i = 0; i < my_num; i++)
    {
        list.push_back(rand() % 1000);
    }
    auto cost = time.elapsed();
    qDebug() << "生成随机数耗费: " << cost << " 毫秒";
    emit genFinish(list);
}

void GenThread::recvNum(int num)
{
    my_num = num;
}

SortThread::SortThread(QObject *parent)
    : QThread{parent}
{}

void SortThread::run()
{
    // sort(my_list.begin(), my_list.end(), std::less<int>());
    sort(my_list.begin(), my_list.end(), std::greater<int>());
    emit sortFinish(my_list);
}

void SortThread::recvList(QVector<int> list)
{
    my_list = list;
}
