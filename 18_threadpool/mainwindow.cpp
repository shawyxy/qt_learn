#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QVector>
#include <QString>
#include <QThreadPool>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 1. 创建线程任务对象
    Generator *generate = new Generator();
    QuickSort *quickSort = new QuickSort();

    // 2.1 主线程向子线程传递参数
    connect(this, &MainWindow::sendNum, generate, &Generator::recvNum); // 随机数的数量
    connect(ui->startButton, &QPushButton::clicked, this, [=](){
        emit sendNum(QRandomGenerator::global()->bounded(50, 100));
        // 将任务对象加入到线程池中执行run方法
        QThreadPool::globalInstance()->start(generate);
    });

    // 2.2 子线程接收主线程的参数
    connect(this, &MainWindow::sendList, quickSort, &QuickSort::recvList); // 随机数列表
    // 3.1 随机数子线程向主线程返回结果随机数列表
    connect(generate, &Generator::genFinish, this, [=](QVector<int> list) {
        emit sendList(list);
        // 将任务对象加入到线程池中执行run方法
        QThreadPool::globalInstance()->start(quickSort);
        ui->randomList->clear();
        for (int i = 0; i < list.size(); i++)
        {
            ui->randomList->addItem(QString::number(list.at(i)));
        }
    });
    // 3.2 排序子线程向子线程返回有序列表
    connect(quickSort, &QuickSort::sortFinish, this, [=](QVector<int> list) {
        ui->sortList->clear();
        for (int i = 0; i < list.size(); i++)
        {
            ui->sortList->addItem(QString::number(list.at(i)));
        }
    });
    // 销毁线程任务对象
    connect(this, &MainWindow::destroyed, this, [=]() {
        generate->deleteLater();
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


