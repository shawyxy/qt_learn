#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QString>
#include <QThread>
#include <QObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 1. 创建任务对象
    Generator *generator = new Generator();
    QuickSort *quickSort = new QuickSort();
    // 2. 创建子线程对象
    QThread *gen_thread = new QThread();
    QThread *sort_thread = new QThread();
    // 3. 将任务对象移动到线程中
    generator->moveToThread(gen_thread);
    quickSort->moveToThread(sort_thread);

    connect(ui->startButton, &QPushButton::clicked, this, [=]() {
        emit sendNum(rand() % 1000 + 1);
        gen_thread->start(); // 启动生成随机数的线程,自动调用重写的run方法
    });

    connect(this, &MainWindow::sendNum, generator, &Generator::generate); // 随机数的数量
    connect(this, &MainWindow::sendList, quickSort, &QuickSort::sort); // 随机数列表
    // 3.1 随机数子线程向主线程返回结果随机数列表
    connect(generator, &Generator::genFinish, this, [=](QVector<int> list) {
        emit sendList(list);
        sort_thread->start();
        for (int i = 0; i < list.size(); i++)
        {
            ui->randomList->addItem(QString::number(list.at(i)));
        }
    });
    // 3.2 排序子线程向子线程返回有序列表
    connect(quickSort, &QuickSort::sortFinish, this, [=](QVector<int> list) {
        for (int i = 0; i < list.size(); i++)
        {
            ui->sortList->addItem(QString::number(list.at(i)));
        }
    });
    // 释放任务对象和线程对象的资源
    // 线程对象也可以通过对象树释放
    connect(this, &MainWindow::destroyed, this, [=]() {
        gen_thread->quit();
        gen_thread->wait();
        gen_thread->deleteLater();

        sort_thread->quit();
        sort_thread->wait();
        sort_thread->deleteLater();

        generator->deleteLater();
        quickSort->deleteLater();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recvList(QVector<int> list)
{
    for (int i = 0; i < list.size(); i++)
    {
        ui->randomList->addItem(QString::number(list.at(i)));
    }
}


