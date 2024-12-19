#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    // 1. 如果窗口设置该策略，鼠标右键点击，该函数被Qt框架调用
    void contextMenuEvent(QContextMenuEvent *event);

    // 该策略会在鼠标右键点击时产生信号,仅发送信号,需要自定义右键菜单槽函数
signals:
    // void QWidget::customContextMenuRequested(const QPoint &pos);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
