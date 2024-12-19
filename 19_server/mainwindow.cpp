#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->port->setText("8989");
    ui->sendMsg->setDefault(false);
    setWindowTitle("服务器");

    // 服务器监听对象
    server = new QTcpServer(this);
    // 监听连接的到来
    connect(server, &QTcpServer::newConnection, this, [=]() {
        // 获取连接的socket对象
        socket = server->nextPendingConnection();
        ui->record->append("与客户端连接成功");
        status->setPixmap(QPixmap(":/connect.png").scaled(20, 20));
        // 监听该连接的数据是否就绪
        connect(socket, &QTcpSocket::readyRead, this, [=]() {
            QByteArray data = socket->readAll();
            ui->record->append("客户端: " + data);
        });
        // 连接关闭,释放资源,改变状态,清空历史记录
        connect(socket, &QTcpSocket::disconnected, this, [=]() {
            socket->close();
            socket->deleteLater();
            socket = nullptr;

            ui->record->clear();
            status->setPixmap(QPixmap(":/disconnect.png").scaled(20, 20));
        });
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

void MainWindow::on_setListen_clicked()
{
    // 从文本编辑框获取端口
    unsigned short port = ui->port->text().toUShort();
    // 监听
    server->listen(QHostAddress::Any, port);
    // 设置按钮为不可用状态
    ui->setListen->setDisabled(true);
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
        ui->record->append("服务器: " + msg);
    }
}
