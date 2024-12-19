#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include "sendfile.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("客户端");
    ui->ip->setText("127.0.0.1");
    ui->port->setText("9090");

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    // 创建任务对象
    SendFile *sendFile = new SendFile();
    // 创建线程对象
    QThread *thread = new QThread();
    // 将任务对象加入到线程中
    sendFile->moveToThread(thread);
    // 注册回调函数
    // 需要注意是子线程接收信号并调用槽函数,所以槽函数要在子线程中定义
    connect(this, &MainWindow::startConnect, sendFile, &SendFile::connectServer);

    // 处理子线程发送的信号
    connect(sendFile, &SendFile::connected, this, [=]() {
        QMessageBox::information(this, "提示", "与服务器建立连接");
    });

    connect(this, &MainWindow::startSend, sendFile, &SendFile::sendFile);

    connect(sendFile, &SendFile::curPercent, ui->progressBar, &QProgressBar::setValue);

    connect(sendFile, &SendFile::disconnected, this, [=]() {
        // 释放资源
        thread->quit();
        thread->wait();
        sendFile->deleteLater();
        thread->deleteLater();
    });
    // 启动线程
    thread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    // 主线程需要用信号通知线程连接服务器
    emit startConnect(ip, port);
}


void MainWindow::on_selectFile_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    if (path.isEmpty())
    {
        QMessageBox::warning(this, "警告", "文件路径不能为空");
        return;
    }
    ui->filePath->setText(path);
}


void MainWindow::on_sendFile_clicked()
{
    emit startSend(ui->filePath->text());
}

