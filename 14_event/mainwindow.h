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

// 重写事件处理器函数
protected:
    void closeEvent(QCloseEvent *ev);
    void resizeEvent(QResizeEvent *ev);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H