#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 2. 设置策略
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;
    QAction *action = menu.addAction("打开文件");
    connect(action, &QAction::triggered, this, [=](){
        QMessageBox::information(this, "选择信息", "打开文件失败");
    });
    menu.addAction("刷新");
    menu.addAction("拷贝");
    menu.exec(QCursor::pos());
}
