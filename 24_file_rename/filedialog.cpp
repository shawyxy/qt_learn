#include "filedialog.h"

#include<QListView>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QTranslator>
#include <QPushButton>
#include <QDialogButtonBox>

MultiFileDirDialog::MultiFileDirDialog(QWidget *parent)
    : QFileDialog(parent)
{

    setOption(QFileDialog::DontUseNativeDialog, true);
    //支持多选
    QListView *listView = findChild<QListView*>("listView");
    if (listView)
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QTreeView *treeView = findChild<QTreeView*>();
    if (treeView)
        treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QDialogButtonBox *button = findChild<QDialogButtonBox *>("buttonBox");

    if (button)
    {
        button->clear();
        QPushButton* accpetbutton = new QPushButton(button);
        accpetbutton->setText("确定");
        button->addButton((QAbstractButton*)accpetbutton, QDialogButtonBox::AcceptRole);
        QPushButton* cancel = new QPushButton(button);
        cancel->setText("取消");
        button->addButton((QAbstractButton*)cancel, QDialogButtonBox::RejectRole);
        disconnect(button, SIGNAL(accepted()), this, SLOT(accept()));//使链接失效
        connect(button, SIGNAL(accepted()), this, SLOT(go()));//改成自己的槽
    }
}

MultiFileDirDialog::~MultiFileDirDialog()
{

}

void  MultiFileDirDialog::go()
{
    m_listSelect = selectedFiles();
    QDialog::accept();
}
