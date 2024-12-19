#include "mybutton.h"
#include <QPainter>

MyButton::MyButton(QWidget *parent)
    : QWidget{parent}
{
    m_pixmap.load(":/apple.jpg");
    setFixedSize(m_pixmap.size());
}

void MyButton::mousePressEvent(QMouseEvent* ev)
{
    Q_UNUSED(ev);
    m_pixmap.load(":/egg.jpg");
    // 它是QWidget的成员函数，会自动调用paintEvent
    update();
    // 发射自定义信号
    emit myClicked();
}
void MyButton::mouseReleaseEvent(QMouseEvent* ev)
{
    Q_UNUSED(ev);
    m_pixmap.load(":/apple.jpg");
    update();
}
void MyButton::enterEvent(QEnterEvent* ev)
{
    Q_UNUSED(ev);
    m_pixmap.load(":/pear.jpg");
    update();
}
void MyButton::leaveEvent(QEvent* ev)
{
    Q_UNUSED(ev);
    m_pixmap.load(":/apple.jpg");
    update();
}
void MyButton::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev);
    // 在当前组件的矩形区域绘制图像
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
}
