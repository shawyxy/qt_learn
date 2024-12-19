#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QListWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDirIterator>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QButtonGroup>
#include <QListWidgetItem>
#include "filedialog.h"

#include <QDebug>
#define log qDebug() << __FILE__ << ":" << __LINE__ << " "

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置窗口大小
    this->setMinimumSize(700, 500);
    this->setMaximumSize(1400, 1000);

    // 第一层：路径选择控件
    lineEdit_path = new QLineEdit(this);
    QPushButton *button_selectPath = new QPushButton("浏览", this);

    QHBoxLayout *hBoxLayout_path = new QHBoxLayout();
    hBoxLayout_path->addWidget(lineEdit_path);
    hBoxLayout_path->addWidget(button_selectPath);

    // 第二层左侧：查找部分
    QLabel *label_search = new QLabel("查找", this);
    label_search->setFont(QFont("", 12));

    radioButton_sContent = new QRadioButton("查找", this);
    lineEdit_contentMatch = new QLineEdit(this);
    radioButton_sContent->setChecked(true); // 默认选中
    // button_sSearch->click();

    radioButton_sType = new QRadioButton("类型匹配", this);
    lineEdit_typeMatch = new QLineEdit(this);

    radioButton_sPrefix = new QRadioButton("前缀匹配", this);
    lineEdit_prefixMatch = new QLineEdit(this);

    radioButton_sSuffix = new QRadioButton("后缀匹配", this);
    lineEdit_suffixMatch = new QLineEdit(this);

    radioButton_sRegex = new QRadioButton("正则匹配", this);
    lineEdit_regexMatch = new QLineEdit(this);

    button_sSearch = new QPushButton("查找", this);
    button_sReset = new QPushButton("重置", this);

    // 第二层右侧：替换部分
    QLabel *label_replace = new QLabel("替换", this);
    label_replace->setFont(QFont("", 12));

    radioButton_rContent = new QRadioButton("替换为", this);
    lineEdit_contentReplace = new QLineEdit(this);

    radioButton_rPrefix = new QRadioButton("添加前缀", this);
    lineEdit_prefixReplace = new QLineEdit(this);

    radioButton_rSuffix = new QRadioButton("添加后缀", this);
    lineEdit_suffixReplace = new QLineEdit(this);

    radioButton_rNumber = new QRadioButton("添加编号", this);
    lineEdit_number = new QLineEdit(this);
    lineEdit_number->setPlaceholderText("默认从 (1) 开始编号");

    button_rApplyPreview = new QPushButton("预览", this);
    button_rApply = new QPushButton("应用", this);
    button_rReset = new QPushButton("重置", this);
    button_withdraw = new QPushButton("撤销", this);
    button_rReset->setDisabled(true);

    // 划分按钮组
    QButtonGroup *leftButtonGroup = new QButtonGroup(this);
    QButtonGroup *rightButtonGroup = new QButtonGroup(this);

    leftButtonGroup->addButton(radioButton_sContent);
    leftButtonGroup->addButton(radioButton_sType);
    leftButtonGroup->addButton(radioButton_sPrefix);
    leftButtonGroup->addButton(radioButton_sSuffix);
    leftButtonGroup->addButton(radioButton_sRegex);

    rightButtonGroup->addButton(radioButton_rContent);
    rightButtonGroup->addButton(radioButton_rPrefix);
    rightButtonGroup->addButton(radioButton_rSuffix);
    rightButtonGroup->addButton(radioButton_rNumber);

    // leftButtonGroup->setExclusive(true);
    // rightButtonGroup->setExclusive(true);

    // 左侧：查找控件水平布局

    QHBoxLayout *hBoxLayout_sContent = new QHBoxLayout();
    hBoxLayout_sContent->addWidget(radioButton_sContent);
    hBoxLayout_sContent->addWidget(lineEdit_contentMatch);

    QHBoxLayout *hBoxLayout_sType = new QHBoxLayout();
    hBoxLayout_sType->addWidget(radioButton_sType);
    hBoxLayout_sType->addWidget(lineEdit_typeMatch);

    QHBoxLayout *hBoxLayout_sPrefix = new QHBoxLayout();
    hBoxLayout_sPrefix->addWidget(radioButton_sPrefix);
    hBoxLayout_sPrefix->addWidget(lineEdit_prefixMatch);

    QHBoxLayout *hBoxLayout_sSuffix = new QHBoxLayout();
    hBoxLayout_sSuffix->addWidget(radioButton_sSuffix);
    hBoxLayout_sSuffix->addWidget(lineEdit_suffixMatch);

    QHBoxLayout *hBoxLayout_sRegex = new QHBoxLayout();
    hBoxLayout_sRegex->addWidget(radioButton_sRegex);
    hBoxLayout_sRegex->addWidget(lineEdit_regexMatch);

    QVBoxLayout *vBoxLayout_leftControls = new QVBoxLayout();
    vBoxLayout_leftControls->addWidget(label_search);
    vBoxLayout_leftControls->addLayout(hBoxLayout_sContent);
    vBoxLayout_leftControls->addLayout(hBoxLayout_sType);
    vBoxLayout_leftControls->addLayout(hBoxLayout_sPrefix);
    vBoxLayout_leftControls->addLayout(hBoxLayout_sSuffix);
    vBoxLayout_leftControls->addLayout(hBoxLayout_sRegex);

    QVBoxLayout *vBoxLayout_leftButtons = new QVBoxLayout();
    vBoxLayout_leftButtons->addWidget(button_sSearch);
    vBoxLayout_leftButtons->addWidget(button_sReset);

    QHBoxLayout *hBoxLayout_left = new QHBoxLayout();
    hBoxLayout_left->addLayout(vBoxLayout_leftControls);
    hBoxLayout_left->addLayout(vBoxLayout_leftButtons);

    // 右侧：替换控件水平布局
    QHBoxLayout *hBoxLayout_content = new QHBoxLayout();
    hBoxLayout_content->addWidget(radioButton_rContent);
    hBoxLayout_content->addWidget(lineEdit_contentReplace);

    QHBoxLayout *hBoxLayout_sPrefixReplace = new QHBoxLayout();
    hBoxLayout_sPrefixReplace->addWidget(radioButton_rPrefix);
    hBoxLayout_sPrefixReplace->addWidget(lineEdit_prefixReplace);

    QHBoxLayout *hBoxLayout_sSuffixReplace = new QHBoxLayout();
    hBoxLayout_sSuffixReplace->addWidget(radioButton_rSuffix);
    hBoxLayout_sSuffixReplace->addWidget(lineEdit_suffixReplace);

    QHBoxLayout *hBoxLayout_sNumber = new QHBoxLayout();
    hBoxLayout_sNumber->addWidget(radioButton_rNumber);
    hBoxLayout_sNumber->addWidget(lineEdit_number);

    QVBoxLayout *vBoxLayout_rightControls = new QVBoxLayout();
    vBoxLayout_rightControls->addWidget(label_replace);
    vBoxLayout_rightControls->addLayout(hBoxLayout_content);
    vBoxLayout_rightControls->addLayout(hBoxLayout_sPrefixReplace);
    vBoxLayout_rightControls->addLayout(hBoxLayout_sSuffixReplace);
    vBoxLayout_rightControls->addLayout(hBoxLayout_sNumber);

    QVBoxLayout *vBoxLayout_rightButtons = new QVBoxLayout();
    vBoxLayout_rightButtons->addWidget(button_rApplyPreview);
    vBoxLayout_rightButtons->addWidget(button_rApply);
    vBoxLayout_rightButtons->addWidget(button_rReset);
    vBoxLayout_rightButtons->addWidget(button_withdraw);

    QHBoxLayout *hBoxLayout_right = new QHBoxLayout();
    hBoxLayout_right->addLayout(vBoxLayout_rightControls);
    hBoxLayout_right->addLayout(vBoxLayout_rightButtons);

    // 第二层整体布局

    QHBoxLayout *hBoxLayout_control = new QHBoxLayout();
    hBoxLayout_control->addLayout(hBoxLayout_left);
    hBoxLayout_control->addLayout(hBoxLayout_right);

    // 第三层：文件列表布局
    oriFileList = new QListWidget(this);
    distFileList = new QListWidget(this);

    QHBoxLayout *hBoxLayout_list = new QHBoxLayout();
    hBoxLayout_list->addWidget(oriFileList);
    hBoxLayout_list->addWidget(distFileList);

    // 总体布局
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(hBoxLayout_path);
    mainLayout->addLayout(hBoxLayout_control);
    mainLayout->addLayout(hBoxLayout_list);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 文件相关
    oriFilePaths = new QStringList();
    oldPaths = new QVector<QString>();
    newPaths = new QVector<QString>();

    ///////////////////////////信号和槽///////////////////////////////
    // 选择文件
    connect(button_selectPath, &QPushButton::clicked, this, &MainWindow::readFilesName);
    // 搜索
    connect(button_sSearch, &QPushButton::clicked, this, &MainWindow::search);
    // 预览
    connect(button_rApplyPreview, &QPushButton::clicked, this, &MainWindow::preview);
    // 重命名文件
    connect(button_rApply, &QPushButton::clicked, this, &MainWindow::rename);
    // 重置
    connect(button_sReset, &QPushButton::clicked, this, [=]() {
        lineEdit_contentMatch->clear();
        lineEdit_typeMatch->clear();
        lineEdit_prefixMatch->clear();
        lineEdit_suffixMatch->clear();
        lineEdit_regexMatch->clear();
    });
    connect(button_rReset, &QPushButton::clicked, this, [=]() {
        lineEdit_contentReplace->clear();
        lineEdit_prefixReplace->clear();
        lineEdit_suffixReplace->clear();

        distFileList->clear();
    });
    // 撤销
    connect(button_withdraw, &QPushButton::clicked, this, &MainWindow::unDo);


    //////////////////////////////按钮可见性///////////////////////////////////
    connect(button_rApply, &QPushButton::clicked, button_rReset, [=]() {
        button_rReset->setEnabled(true);
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::readFilesName()
{
    QString selectedPath = QFileDialog::getExistingDirectory(this, tr("选择目录"), QDir::homePath());
    if (!selectedPath.isEmpty())
    {
        // 如果选择了一个目录，则递归获取该目录下的所有文件
        oriFilePaths->clear();
        QDirIterator it(selectedPath, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            oriFilePaths->append(it.next());
        }
        if (!oriFilePaths->isEmpty())
        {
            std::sort(oriFilePaths->begin(), oriFilePaths->end());
            // 更新路径显示
            lineEdit_path->setText(selectedPath);
            oriFileList->clear();
            for (const auto& path : *oriFilePaths)
            {
                QFileInfo file(path);
                oriFileList->addItem(file.fileName());
            }
        }
        else
        {
            QMessageBox::information(this, tr("信息"), tr("目录中没有文件"));
        }
    }
    else
    {
        // 如果选择的是文件
        *oriFilePaths = QFileDialog::getOpenFileNames(this, tr("选择文件"), "./test", tr("All Files (*.*)"));
        if (!oriFilePaths->isEmpty())
        {
            std::sort(oriFilePaths->begin(), oriFilePaths->end());
            // 获取文件所在目录
            QFileInfo fileInfo((*oriFilePaths)[0]);
            lineEdit_path->setText(fileInfo.path());
            // 将选择的文件显示到左侧列表中
            oriFileList->clear();
            for (const auto& path : *oriFilePaths)
            {
                QFileInfo file(path);
                oriFileList->addItem(file.fileName());
            }
        }
    }
}

void MainWindow::search()
{
    oriFileList->clear();
    // 预处理
    QVector<QString> names;
    QVector<QString> baseNames;
    QVector<QString> types;
    for (auto &path : *oriFilePaths)
    {
        QFileInfo fileInfo(path);
        names.push_back(fileInfo.fileName());
        baseNames.push_back(fileInfo.baseName());
        types.push_back(fileInfo.suffix());
    }

    if (radioButton_sContent->isChecked()) // 内容匹配
    {
        QString pattern = lineEdit_contentMatch->text();
        for (auto &name : names)
        {
            QString baseName = name.section('.', 0, 0);
            QString s = "<font color=\"red\"> xxx </font>";
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(s);
            item->setData(Qt::DisplayRole, s);
            item->setData(Qt::TextFormat(Qt::RichText), Qt::RichText);

            if (baseName.contains(pattern))
            {
                oriFileList->addItem(name);
            }
        }
    }
    else if (radioButton_sType->isChecked()) // 类型匹配
    {
        QString reqType = lineEdit_typeMatch->text();
        for (int i = 0; i < names.size(); i++)
        {
            QString type = types[i];
            QString name = names[i];
            // qDebug() << reqType << " " << type;
            if (type == reqType)
            {
                oriFileList->addItem(name);
            }
        }
    }
    else if (radioButton_sPrefix->isChecked()) // 前缀匹配
    {
        QString prefix = lineEdit_prefixMatch->text();
        for (auto &path : *oriFilePaths)
        {
            QFileInfo fileInfo(path);
            QString name = fileInfo.fileName();
            if (name.startsWith(prefix))
            {
                oriFileList->addItem(name);
            }
        }
    }
    else if (radioButton_sSuffix->isChecked()) // 后缀匹配
    {
        QString suffix = lineEdit_suffixMatch->text();
        for (auto &path : *oriFilePaths)
        {
            QFileInfo fileInfo(path);
            QString name = fileInfo.fileName();
            if (name.endsWith(suffix))
            {
                oriFileList->addItem(name);
            }
        }
    }
    else if (radioButton_sRegex->isChecked()) // 正则匹配
    {
        QString pattern = lineEdit_regexMatch->text();
        QRegularExpression regex(pattern);
        for (auto &path : *oriFilePaths)
        {
            QFileInfo fileInfo(path);
            QString nameWithoutSfx = fileInfo.completeBaseName();
            QString name = fileInfo.fileName();
            QRegularExpressionMatch match = regex.match(nameWithoutSfx);
            if (match.isValid() == false)
            {
                QMessageBox::information(this, "提示", "正则表达式不合法");
                break;
            }
            if (match.hasMatch())
            {
                oriFileList->addItem(name);
            }
        }
    }
}

void MainWindow::preview()
{
    distFileList->clear();
    if (radioButton_rContent->isChecked()) // 内容匹配
    {
        QString content = lineEdit_contentMatch->text();
        QString replace = lineEdit_contentReplace->text();
        for (int i = 0; i < oriFileList->count(); i++)
        {
            auto item = oriFileList->item(i);
            QString name = item->text();
            name.replace(content, replace);
            distFileList->addItem(name);
        }
    }
    else if (radioButton_rPrefix->isChecked())
    {
        QString prefix = lineEdit_prefixReplace->text();
        for (int i = 0; i < oriFileList->count(); i++)
        {
            QString oldName = oriFileList->item(i)->text();
            QString newName = prefix + oldName;
            distFileList->addItem(newName);
        }
    }
    else if (radioButton_rSuffix->isChecked())
    {
        QString suffix = lineEdit_suffixReplace->text();
        for (int i = 0; i < oriFileList->count(); i++)
        {
            QString oldName = oriFileList->item(i)->text();
            QString newName = oldName + suffix;
            distFileList->addItem(newName);
        }
    }
    else if (radioButton_sRegex->isChecked() && radioButton_rContent->isChecked())
    {
        QString pattern = lineEdit_regexMatch->text();
        QString connent = lineEdit_contentReplace->text();
        QRegularExpression regex(pattern);
        for (int i = 0; i < oriFileList->count(); i++)
        {
            QString oldName = oriFileList->item(i)->text();
            QString newName = oldName.replace(regex, connent);
            distFileList->addItem(newName);
        }
    }
    else if (radioButton_rNumber->isChecked())
    {
        int index = 1;
        QString num = lineEdit_number->text();
        if (num.size())
        {
            index = num.toInt();
        }
        for (auto &path : *oriFilePaths)
        {
            QFileInfo fileInfo(path);
            QString baseName = fileInfo.completeBaseName();
            QString suffix = fileInfo.suffix();
            QString newName;
            if (suffix.size() != 0)
            {
                newName = QString("%1(%2).%3").arg(baseName).arg(index++).arg(suffix);
            }
            else
            {
                newName = QString("%1(%2)").arg(baseName).arg(index++);
            }
            distFileList->addItem(newName);
        }
    }
}

void MainWindow::rename()
{
    if (radioButton_rContent->isChecked()) // 内容替换
    {
        for (int i = 0; i < distFileList->count(); i++)
        {
            QString oldName = oriFileList->item(i)->text();
            QString oldPath = lineEdit_path->text() + "/" + oldName;
            QString newName = distFileList->item(i)->text();
            QString newPath = lineEdit_path->text() + "/" + newName;
            // qDebug() << oldPath;
            // qDebug() << newPath;
            QFile::rename(oldPath, newPath);

            oldPaths->push_back(oldPath);
            newPaths->push_back(newPath);
        }
    }
    else if (radioButton_rPrefix->isChecked()) // 添加前缀
    {
        QString prefix = lineEdit_prefixReplace->text();
        for (int i = 0; i < distFileList->count(); i++)
        {
            QString oldName = oriFileList->item(i)->text();
            QString oldPath = lineEdit_path->text() + "/" + oldName;
            QString newName = prefix + oldName;
            QString newPath = lineEdit_path->text() + "/" + newName;
            QFile::rename(oldPath, newPath);

            oldPaths->push_back(oldPath);
            newPaths->push_back(newPath);
        }
    }
    else if (radioButton_rSuffix->isChecked()) // 添加后缀
    {
        QString suffix = lineEdit_suffixReplace->text();
        for (int i = 0; i < distFileList->count(); i++)
        {
            QString oldName = oriFileList->item(i)->text();
            QString oldPath = lineEdit_path->text() + "/" + oldName;
            QString newName = oldName + suffix;
            QString newPath = lineEdit_path->text() + "/" + newName;
            QFile::rename(oldPath, newPath);

            oldPaths->push_back(oldPath);
            newPaths->push_back(newPath);
        }
    }
    else if (radioButton_rNumber->isChecked())
    {
        for (int i = 0; i < oriFilePaths->size(); i++)
        {
            QFileInfo fileInfo(oriFilePaths->at(i));
            QString oriPath = fileInfo.absoluteFilePath();
            QString path = oriPath.left(oriPath.lastIndexOf("/"));
            QString newName = distFileList->item(i)->text();
            QString newPath = path + "/" + newName;

            bool ok = QFile::rename(oriPath, newPath);
            if (!ok)
            {
                qDebug() << "修改文件名错误";
            }
            log << oriPath << '\n' << newName;
            oldPaths->push_back(oriPath);
            newPaths->push_back(newPath);
        }
    }
    if (oriFilePaths->size() != 0)
    {
        QMessageBox::information(this, "提示", "修改文件成功");
    }
}

void MainWindow::unDo()
{
    for (int i = 0; i < oldPaths->size(); i++)
    {
        QFile::rename(newPaths->at(i), oldPaths->at(i));
    }
    if (oriFilePaths->size() != 0)
    {
        distFileList->clear();
        QMessageBox::information(this, "提示", "已撤销更改");
    }
    button_rReset->setEnabled(false);
}

void MainWindow::highlightSearch(const QString &pattern)
{
    oriFileList->clear();  // 清空原有列表项

    QRegularExpression regex(pattern);  // 创建正则表达式

    for (const QString &path : *oriFilePaths) {
        QFileInfo fileInfo(path);
        QString fileName = fileInfo.fileName();  // 获取文件名

        // 使用正则表达式查找匹配的部分
        QRegularExpressionMatchIterator matchIterator = regex.globalMatch(fileName);
        QString highlightedText = fileName;
        int offset = 0;  // 用于调整文本替换后的偏移量

        // 遍历所有匹配项并替换为红色显示
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            QString matchedSubstring = match.captured(0);  // 获取匹配的子串
            QString replacement = "<font color=\"red\">" + matchedSubstring + "</font>";  // 用红色显示匹配的部分

            // 替换文本中的匹配项
            highlightedText.replace(match.capturedStart(0) + offset, matchedSubstring.length(), replacement);
            offset += replacement.length() - matchedSubstring.length();  // 更新偏移量
        }

        // 将高亮后的文本添加到 QListWidget 中
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(highlightedText);  // 设置文本为高亮显示
        item->setTextAlignment(Qt::AlignLeft);  // 设置文本对齐方式

        // 设置富文本格式
        item->setData(Qt::DisplayRole, highlightedText);  // 设置文本内容
        item->setData(Qt::TextFormat(), Qt::RichText);  // 设置文本格式为富文本

        oriFileList->addItem(item);  // 将项添加到列表
    }
}

void MainWindow::highlightPreview(const QString &content)
{
    distFileList->clear();  // 清空预览列表

    for (int i = 0; i < oriFileList->count(); ++i) {
        QListWidgetItem *item = oriFileList->item(i);
        QString originalText = item->text();  // 获取原始文件名

        // 将要替换的部分高亮为红色
        QString replacedText = originalText;
        replacedText.replace(content, "<font color=\"red\">" + content + "</font>");

        // 创建新的列表项并添加到预览列表
        QListWidgetItem *previewItem = new QListWidgetItem();
        previewItem->setText(replacedText);  // 设置替换后的文本
        previewItem->setTextAlignment(Qt::AlignLeft);  // 设置文本对齐方式

        // 设置富文本格式
        previewItem->setData(Qt::DisplayRole, replacedText);  // 设置文本内容
        previewItem->setData(Qt::TextFormat(Qt::RichText), Qt::RichText);  // 设置文本格式为富文本

        distFileList->addItem(previewItem);  // 将项添加到预览列表
    }
}



