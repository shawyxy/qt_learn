#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("8989");
    ui->ip->setText("127.0.0.1");
    ui->disconnect->setDisabled(true);
    ui->sendMsg->setDisabled(true);
    setWindowTitle("客户端");

    // 监听该连接的数据是否就绪
    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray data = socket->readAll();
        ui->record->append("客户端: " + data);
    });
    // 状态栏
    status = new QLabel();
    status->setPixmap(QPixmap(":/disconnect.png").scaled(20, 20));
    ui->statusbar->addWidget(new QLabel("连接状态: "));
    ui->statusbar->addWidget(status);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendMsg_clicked()
{
    if (socket == nullptr)
    {
        QMessageBox::information(this, "提示", "当前无连接");
        return;
    }
    QString msg = ui->msg->toPlainText();
    if (msg.size() == 0)
    {
        QMessageBox::information(this, "提示", "输入不能为空");
    }
    else
    {
        ui->msg->clear();
        socket->write(msg.toUtf8());
        ui->record->append("客户端: " + msg);
    }
}

void MainWindow::on_connect_clicked()
{
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        QByteArray data = socket->readAll();
        ui->record->append("客户端: " + data);
    });
    // 连接关闭,改变状态
    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        ui->record->append("服务器关闭连接");
        socket->close();
        socket->deleteLater();
        socket = nullptr;

        status->setPixmap(QPixmap(":/disconnect.png").scaled(20, 20));
        ui->record->append("客户端与服务器断开连接");
        ui->connect->setDisabled(false);
        ui->disconnect->setDisabled(true);
    });
    // 连接
    connect(socket, &QTcpSocket::connected, this, [=]() {
        status->setPixmap(QPixmap(":/connect.png").scaled(20, 20));
        ui->record->append("与服务器连接成功");
        ui->connect->setDisabled(true);
        ui->disconnect->setDisabled(false);
        ui->sendMsg->setEnabled(true);
    });

    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    socket->connectToHost(QHostAddress(ip), port);
}


void MainWindow::on_disconnect_clicked()
{
    ui->record->append("客户端主动关闭连接");
    socket->close();
    socket->deleteLater();
    socket = nullptr;

    ui->record->clear();
    ui->connect->setEnabled(true);
    ui->disconnect->setDisabled(true);
}

