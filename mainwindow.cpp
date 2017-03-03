#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<stdio.h>
#include<QString>
#include<QStringList>
#include"commtab.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    tabSecrityPage = new TabSecrityPage(ui->tabWidget);
    tabSysPage = new TabSysPage(ui->tabWidget);
    tabAuditPage = new TabAuditPage(ui->tabWidget);
    tabCommPage = new CommTab(ui->tabWidget);
    indexWidget = new IndexWidget(ui->tabWidget);

    ui->tabWidget->addTab(indexWidget, tr("欢迎信息"));
    ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
    ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
    ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
    ui->tabWidget->addTab(tabCommPage, tr("常规"));
    switch(get_user_role())
    {
    case ROOT:
        break;
    case SECADMIN:
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSysPage));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabAuditPage));
        break;
    case SYSADMIN:
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSecrityPage));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabAuditPage));
        break;
    case AUDIADMIN:
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSecrityPage));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSysPage));
        break;
    case NORMALUSER:
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSecrityPage));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSysPage));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabAuditPage));
        break;
    default:
        break;
    }
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
