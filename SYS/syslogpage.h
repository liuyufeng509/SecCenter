#ifndef SYSLOGPAGE_H
#define SYSLOGPAGE_H

#include <QWidget>
#include<QFileSystemModel>
#include<QTreeView>
#include<QTextBrowser>
#include<QFile>
#include<QTextStream>
#include<QPushButton>
#include<QLineEdit>
#include<QComboBox>

namespace Ui {
class SysLogPage;
}

class SysLogPage : public QWidget
{
    Q_OBJECT

public:
    explicit SysLogPage(QWidget *parent = 0);
    ~SysLogPage();

public slots:
    void browserIndex(QModelIndex index);
    void findnext();
     void findPre();

private:
    Ui::SysLogPage *ui;
    QFileSystemModel *model;
    QTreeView *treeView;
    QTextBrowser *browser;
    QPushButton *nexButton;
    QPushButton *preButton;
   // QLineEdit *lineEdit;
    QComboBox *comBox;
};

#endif // SYSLOGPAGE_H
