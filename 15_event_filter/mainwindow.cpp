#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 1. 给textEdit安装过滤器
    // 事件的流向: textEdit->this(窗口)
    ui->textEdit->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // 判断对象和事件
    if (watched == ui->textEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEv = (QKeyEvent*)event;
        if (keyEv->key() == Qt::Key_Enter ||         // 小键盘确认
            keyEv->key() == Qt::Key_Return)     // 大键盘回车
        {
            qDebug() << "我是回车, 被按下了...";
            return true;
        }
    }
    return false;
}
