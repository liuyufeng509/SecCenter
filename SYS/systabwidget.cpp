#include "systabwidget.h"

SysTabWidget::SysTabWidget(QWidget *parent):QTabWidget(parent)
{
    indexPage = new IndexWidget(SYSADMIN, this);
    helpPage = new HelpPage(SYSADMIN, this);
    userMgrPage = new SysUserMgrPage(this);
    svrMgrPage = new SvrMgrPage(this);
    sysLogPage = new SysLogPage(this);
    kernalPamPage = new KernParmMngWidget(this);

    addTab(indexPage,tr("欢迎"));
    addTab(userMgrPage, tr("用户管理"));
    addTab(svrMgrPage, tr("服务管理"));
    addTab(sysLogPage, tr("系统日志"));
    addTab(kernalPamPage, tr("内核参数"));
    addTab(helpPage,tr("帮助"));

    connect(this, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
}

void SysTabWidget::tabChanged(int index)
{
    if(index == 2)
    {
        svrMgrPage->getAllServices();
    }
}

void SysTabWidget::UpdateIndex()
{
    indexPage->initUi();
}

SysTabWidget::~SysTabWidget()
{

}
