#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<stdio.h>
#include<QString>
#include<QStringList>
#include"commtab.h"
#include"qreadconfig.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //ui->label->setHidden(true);
    QReadConfig::getInstance()->readConfigFile();
    //this->setWindowFlags(Qt::FramelessWindowHint);
	//获取当前用户角色，如果失败，捕获异常，提示错误信息
    try
    {
        m_curRole = m_MainModel.getUserRole();
        if(m_curRole == ERROR)
        {
            m_curRole = ROOT;
        }
        switch(m_curRole)
        {
        case ROOT:
            tabSecrityPage = new TabSecrityPage(ui->tabWidget);
            tabSysPage = new TabSysPage(ui->tabWidget);
            tabAuditPage = new TabAuditPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
            ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
            ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
            break;
        case SECADMIN:
            tabSecrityPage = new TabSecrityPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
            break;
        case SYSADMIN:
            tabSysPage = new TabSysPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
            break;
        case AUDIADMIN:
            tabAuditPage = new TabAuditPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
            break;
        case ERROR:
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);
            ui->tabWidget->addTab(indexWidget, tr("首页"));
            break;
        default:
            break;
        }
        ui->tabWidget->setCurrentIndex(0);
    }catch(Exception exp)
    {
        messageBox(exp.getErroWhat());
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
