#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget, &MyButton::myClicked, this, [=](){
        QMessageBox::information(this, "点击", "自定义按钮被点击");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 每个组件都有一个event事件处理器

// 关闭当前窗口之前事件分发器会调用closeEvent
void MainWindow::closeEvent(QCloseEvent *ev)
{
    QMessageBox::Button btn = QMessageBox::question(this, "关闭窗口", "确认关闭窗口?");
    if (btn == QMessageBox::Yes)
    {
        // 接受并处理事件
        ev->accept();
    }
    else
    {
        // 忽略并向上传递事件
        ev->ignore();
    }
}

// 当组件大小发生变化，事件分发器会调用resizeEvent
void MainWindow::resizeEvent(QResizeEvent *ev)
{
    qDebug() << "oldSize: " << ev->oldSize()
    << "currentSize: " << ev->size();
}
