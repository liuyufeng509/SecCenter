#ifndef LOGMAINWINDOW_H
#define LOGMAINWINDOW_H

#include <QMainWindow>
#include<QDirModel>
#include<QTreeView>
#include<QTextBrowser>
#include<QFile>
#include<QTextStream>
#include<QPushButton>
#include<QLineEdit>

namespace Ui {
class LogMainWindow;
}

class LogMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogMainWindow(QWidget *parent = 0);
    ~LogMainWindow();

public slots:
    void browserIndex(QModelIndex index);
    void findnext();
     void findPre();

private:
    Ui::LogMainWindow *ui;
    QDirModel *model;
    QTreeView *treeView;
    QTextBrowser *browser;
    QPushButton *nexButton;
     QPushButton *preButton;
    QLineEdit *lineEdit;
};

#endif // LOGMAINWINDOW_H
