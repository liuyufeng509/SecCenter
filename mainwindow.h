#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"commtab.h"
#include"tabauditpage.h"
#include"tabsecritypage.h"
#include"tabsyspage.h"
#include"common.h"
#include "indexwidget.h"
#include"mainfunclass.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:



private:
    Ui::MainWindow *ui;

     CommTab *tabCommPage;
     TabSecrityPage *tabSecrityPage;
     TabAuditPage *tabAuditPage;
     TabSysPage *tabSysPage;
     IndexWidget *indexWidget;

     MainFunClass m_MainModel;
     ROLE              m_curRole;
};

#endif // MAINWINDOW_H
