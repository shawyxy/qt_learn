#include "widget.h"
#include "ui_widget.h"
#include "QLabel"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 推荐使用在堆上创建标签对象，正确
    QLabel* label = new QLabel(this);
    label->setText("Hello World_2");
    // 在栈上创建对象，错误
    // QLabel label;
    // label.setText("Hello World_2");
}

Widget::~Widget()
{
    delete ui;
}
