#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "recvfile.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("9090");
    setWindowTitle("服务端");
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, [=]() {
        socket = server->nextPendingConnection();
        // 创建子线程,通过重载构造函数的参数传递主线程中创建的socket对象指针
        RecvFile *thread = new RecvFile(socket);

        thread->start();

        connect(thread, &RecvFile::recvFinished, this, [=]() {
            thread->quit();
            thread->wait();
            thread->deleteLater();
            QMessageBox::information(this, "提醒", "文件接收完毕");
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    server->listen(QHostAddress::Any, port);
}

