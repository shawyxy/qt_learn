#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QString s = "这是一个中文字符串";
    qDebug() << s;

    w.show();
    return a.exec();
}
