#include "audtabwidget.h"
#include "qreadconfig.h"
AudTabWidget::AudTabWidget(QWidget *parent):QTabWidget(parent)
{
    indexPage = new IndexWidget(AUDADMIN, this);
    helpPage = new HelpPage(AUDADMIN, this);
    if(QReadConfig::getInstance()->audCfgInfo.audctl)
        audSvrCtlPage = new AudSvrCtlPage(this);
    audInfoPage = new AudInfoPage(this);
    audRulePage = new AudRulePage(this);
    audKernRulePage = new AudKernRulePage(this);
    audConfPage = new AudConfPage(this);

    addTab(indexPage,tr("欢迎"));
    if(QReadConfig::getInstance()->audCfgInfo.audctl)
        addTab(audSvrCtlPage,tr("审计服务"));
    addTab(audInfoPage,tr("审计信息"));
    addTab(audRulePage,tr("审计规则"));
    addTab(audKernRulePage,tr("内核参数"));
    addTab(audConfPage,tr("审计配置"));
    addTab(helpPage,tr("帮助"));

}

void AudTabWidget::UpdateIndex()
{
    indexPage->initUi();
}

AudTabWidget:: ~AudTabWidget()
{

}
