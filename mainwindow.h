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
#include "mytitlebar.h"
#include "sectabwidget.h"
#include "systabwidget.h"
#include "audtabwidget.h"

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
    void initTitleBar();
    void initCentralWidget();
    void paintEvent(QPaintEvent *event);
   // void loadStyleSheet(const QString &sheetName);
private slots:
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

    void tabChanged(int index);
protected:
    MyTitleBar* m_titleBar;

private:
    Ui::MainWindow *ui;

    SecTabWidget * secTabWidget;
    SysTabWidget * sysTabWidget;
    AudTabWidget *audTabWidget;
     CommTab *tabCommPage;
     TabSecrityPage *tabSecrityPage;
     TabAuditPage *tabAuditPage;
     TabSysPage *tabSysPage;
     IndexWidget *indexWidget;

     ROLE              m_curRole;
};

#endif // MAINWINDOW_H
