#include <QObject>
#include <QFileDialog>
class MultiFileDirDialog : public QFileDialog
{
    Q_OBJECT

public:
    MultiFileDirDialog(QWidget *parent);
    ~MultiFileDirDialog();
    QStringList m_listSelect;
signals:

public slots :
    void go();

private:

};
