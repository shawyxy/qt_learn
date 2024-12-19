#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>

class MyButton : public QWidget
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

protected:
    // 重写事件处理函数
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void enterEvent(QEnterEvent* ev);
    void leaveEvent(QEvent* ev);
    // 将图像绘制到组件中
    void paintEvent(QPaintEvent* ev);

signals:
    // 自定义信号
    void myClicked();

private:
    // 存放图像
    QPixmap m_pixmap;
    // QPixmap m_normal;
    // QPixmap m_enter;
    // QPixmap m_leave;
    // QPixmap m_press;
};

#endif // MYBUTTON_H
