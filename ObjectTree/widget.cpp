#include "widget.h"
#include "ui_widget.h"

#include <QLabel>
#include <iostream>

// 自定义标签类
class MyLabel : public QLabel { // 需要手动添加QLabel头文件
public:
    MyLabel(Widget* parent) : QLabel(parent) { // 注意参数是Widget*
        // std::cout << "MyLable()" << std::endl;
    }
    ~MyLabel() {
        // std::cout << "~MyLable()" << std::endl;
    }
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 创建自定义标签对象
    MyLabel* label = new MyLabel(this); // 建立父子关系
}

Widget::~Widget()
{
    delete ui;
}
