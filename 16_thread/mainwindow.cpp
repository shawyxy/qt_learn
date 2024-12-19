#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 1. 创建随机数生成线程对象
    GenThread *gen_thread = new GenThread();
    SortThread *sort_thread = new SortThread();
    // 2.1 主线程向子线程传递参数
    connect(ui->startButton, &QPushButton::clicked, this, [=](){
        emit sendNum(rand() % 1000 + 1);
        gen_thread->start(); // 启动生成随机数的线程,自动调用重写的run方法
    });
    // 2.2 子线程接收主线程的参数
    connect(this, &MainWindow::sendNum, gen_thread, &GenThread::recvNum); // 随机数的数量
    connect(this, &MainWindow::sendList, sort_thread, &SortThread::recvList); // 随机数列表
    // 3.1 随机数子线程向主线程返回结果随机数列表
    connect(gen_thread, &GenThread::genFinish, this, [=](QVector<int> list) {
        emit sendList(list);
        sort_thread->start();
        for (int i = 0; i < list.size(); i++)
        {
            ui->randomList->addItem(QString::number(list.at(i)));
        }
    });
    // 3.2 排序子线程向子线程返回有序列表
    connect(sort_thread, &SortThread::sortFinish, this, [=](QVector<int> list) {
        for (int i = 0; i < list.size(); i++)
        {
            ui->sortList->addItem(QString::number(list.at(i)));
        }
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


