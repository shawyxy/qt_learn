#include "widget.h"
#include "ui_widget.h"
#include <QTimer>
#include <QTime>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 定时器
    QTimer *timer = new QTimer(this);
    // 设置精度
    timer->setTimerType(Qt::PreciseTimer);
    // 点击按钮, 触发定时器
    connect(ui->pushButton1, &QPushButton::clicked, this, [=](){
        // 定时器正在运行
        if (timer->isActive())
        {
            ui->pushButton1->setText("开始");
            timer->stop();
        }
        else
        {
            ui->pushButton1->setText("关闭");
            timer->start(1000);
        }
    });
    // 定时器超时, 打印当前时间
    connect(timer, &QTimer::timeout, this, [=](){
        QTime time = QTime::currentTime();
        // 格式化:时分秒/毫秒
        QString timestr = time.toString("hh:mm:ss.zzz");
        // 显示时间
        ui->label1->setText(timestr);
    });


    /////////////////////////////////////
    QTimer *onceTimer = new QTimer(this);
    // 设置一次性计时器
    onceTimer->setSingleShot(true);
    // 点击按钮, 触发定时器
    connect(ui->pushButton2, &QPushButton::clicked, this, [=](){
        ui->pushButton2->setText("关闭");
        onceTimer->start(1000);
    });
    // 定时器超时, 打印当前时间
    connect(onceTimer, &QTimer::timeout, this, [=](){
        QTimer::singleShot(2000, this, [=](){
            QTime time = QTime::currentTime();
            // 格式化:时分秒/毫秒
            QString timestr = time.toString("hh:mm:ss.zzz");
            // 显示时间
            ui->label2->setText(timestr);
            ui->pushButton2->setText("开始");
        });
    });
}

Widget::~Widget()
{
    delete ui;
}
