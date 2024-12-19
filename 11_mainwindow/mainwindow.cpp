#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <string>
#include <QString>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置当前窗口的尺寸
    setMaximumSize(600, 600);
    setMinimumSize(QSize(300, 300));
    // 设置固定尺寸
    // setFixedSize(QSize(100, 100));

}

MainWindow::~MainWindow()
{
    delete ui;
}
// 移动窗口
void MainWindow::on_pushButton_1_clicked()
{
    // 获取当前窗口的位置
    QRect rect = this->frameGeometry();
    move(rect.topLeft() + QPoint(10, 20));
}

// 获取位置信息
void MainWindow::on_pushButton_2_clicked()
{
    QRect rect = this->frameGeometry();
    qDebug() << "左上角: " << rect.topLeft()
             << "右上角: " << rect.topRight()
             << "左下角: " << rect.bottomLeft()
             << "右下角: " << rect.bottomRight()
             << "宽度: " << rect.width()
             << "高度: " << rect.height();
}

// 修改
void MainWindow::on_pushButton_3_clicked()
{
    int x = 100 + rand() % 500;
    int y = 100 + rand() % 500;
    int width = this->width() - 50;
    int height = this->height() - 50;
    int i = 1;
    setGeometry(x, y, width, height);
    this->setWindowTitle("修改标题");
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QMenu menu;
        QAction *act = menu.addAction("打开文件");
        connect(act, &QAction::triggered, this, [=](){
            QMessageBox::information(this, "title", "打开文件");
        });
        menu.addAction("刷新");
        menu.addAction("拷贝");
        menu.addAction("剪切");
        menu.exec(QCursor::pos());
    }
}
