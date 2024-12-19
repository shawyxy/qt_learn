#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>

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

public slots:
    void readFilesName();
    void search();
    void preview();
    void rename();
    void unDo();

    void highlightSearch(const QString &pattern);
    void highlightPreview(const QString &content);
private:
    QLineEdit *lineEdit_path;



    // 查找部分
    QRadioButton *radioButton_sContent;
    QLineEdit *lineEdit_contentMatch;

    QRadioButton *radioButton_sType;
    QLineEdit *lineEdit_typeMatch;

    QRadioButton *radioButton_sPrefix;
    QLineEdit *lineEdit_prefixMatch;

    QRadioButton *radioButton_sSuffix;
    QLineEdit *lineEdit_suffixMatch;

    QRadioButton *radioButton_sRegex;
    QLineEdit *lineEdit_regexMatch;

    QPushButton *button_sSearch;
    QPushButton *button_sReset;

    // 替换部分
    QRadioButton *radioButton_rContent;
    QLineEdit *lineEdit_contentReplace;

    QRadioButton *radioButton_rPrefix;
    QLineEdit *lineEdit_prefixReplace;

    QRadioButton *radioButton_rSuffix;
    QLineEdit *lineEdit_suffixReplace;

    QRadioButton* radioButton_rNumber;
    QLineEdit *lineEdit_number;

    QPushButton *button_rApplyPreview;
    QPushButton *button_rApply;
    QPushButton *button_rReset;
    QPushButton *button_withdraw;



private:
    // 选择的文件完整路径
    QStringList *oriFilePaths;
    // 列表
    QListWidget *oriFileList;
    QListWidget *distFileList;
    // 用于撤销: 新旧文件路径
    QVector<QString> *oldPaths;
    QVector<QString> *newPaths;


};
#endif // MAINWINDOW_H
