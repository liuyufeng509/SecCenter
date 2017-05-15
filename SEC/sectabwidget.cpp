#include "sectabwidget.h"

SecTabWidget::SecTabWidget(QWidget *parent):QTabWidget(parent)
{
    thread = new QThread;
    SecFunClass::getInstance()->moveToThread(thread);
    thread->start();
    indexPage = new IndexWidget(SECADMIN, this);
    helpPage = new HelpPage(SECADMIN, this);
    usrMgrPage = new UserMgrPage(this);
    secStaPage = new SecStatusPage(this);
    secTagPage = new SecTagPage(this);
    secRulesPage = new SecRulesPage(this);
    svrCtrlPage = new SvrCtrlPage(this);

    addTab(indexPage,tr("欢迎"));
    addTab(secStaPage, tr("安全状态"));
    addTab(usrMgrPage, tr("用户管理"));
    addTab(secTagPage, tr("安全标签设置"));
    addTab(secRulesPage, tr("安全策略"));
    addTab(svrCtrlPage, tr("服务控制"));
    addTab(helpPage,tr("帮助"));

  //  connect(secStaPage, &SecStatusPage::freshClicked, svrCtrlPage, &SvrCtrlPage::freshPolCtl);
   connect(secStaPage, SIGNAL(freshClicked(SecStatus)), svrCtrlPage, SLOT(freshPolCtl(SecStatus)));
   connect(this, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));

}

void SecTabWidget::tabChanged(int index)
{
    if(index == 4)
    {
        secRulesPage->getAllRules();
    }
}

void SecTabWidget::UpdateIndex()
{
    indexPage->initUi();
}

SecTabWidget::~SecTabWidget()
{

}
