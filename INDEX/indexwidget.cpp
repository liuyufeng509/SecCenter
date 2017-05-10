#include "indexwidget.h"
#include "ui_indexwidget.h"

IndexWidget::IndexWidget(ROLE curRole,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexWidget)
{
    ui->setupUi(this);
    this->curRole = curRole;
    initUi();
}

void IndexWidget::initUi()
{
    QFont font;
    font.setPointSize(11);
    font.setFamily("Cantarell");
    ui->textBrowser_1->setCurrentFont(font);
    ui->textBrowser->setCurrentFont(font);
    ui->textBrowser_2->setCurrentFont(font);
    QString uname = "";
    QString roleName = "";
    QString enforce = "";
    QString date = "2017-05-08";
    QString des = tr("    安全管理中心为安全操作系统的功能提供人机交互界面。包括系统管理员、安全管理员和审计管理员三个角色，分别提供不同的角色功能。分别提供功能如下：\n\
    安全管理员通过对系统中的主体、客体进行统一标记，对主体进行授权，配置一致的安全策略。安全管理员的管理功能有用户管理、安全状态查看、安全标签设置、安全策略查看和安全服务控制功能。详细信息请查看帮助。\n\
    系统管理员通过对系统的资源和运行进行配置、控制和管理，包括用户身份管理、系统资源配置、系统加载和启动、系统运行的异常处理等。系统管理员的管理功能包括用户管理、服务管理、系统日志查看和内核参数设置功能。详细信息请查看帮助。\n\
    审计管理员通过对分布在系统各个组成部分的安全审计机制集中管理，包括根据安全审计策略对审计记录进行分类；提供按时间段开启和关闭相应类型的安全审计机制；对各类审计记录进行存储、管理和查询等。对审计记录进行分析，并依据分析结果进行处理。\
审计管理员管理功能包括审计信息查询、审计信息展示、审计策略配置和审计配置信息设置。详细信息请查看帮助。");
    QString secDec = tr("\n    安全管理中心为安全操作系统的功能提供人机交互界面。包括系统管理员、安全管理员和审计管理员三个角色，分别提供不同的角色功能。当前登陆的用户为安全管理员，安全管理员介绍如下：\n\
    安全管理员通过对系统中的主体、客体进行统一标记，对主体进行授权，配置一致的安全策略。安全管理员的管理功能有用户管理、安全状态查看、安全标签设置、安全策略查看和安全服务控制功能。详细信息请查看帮助。");
    QString sysDec = tr("\n    安全管理中心为安全操作系统的功能提供人机交互界面。包括系统管理员、安全管理员和审计管理员三个角色，分别提供不同的角色功能。当前登陆的用户为系统管理员，系统管理员介绍如下：\n\
    系统管理员通过对系统的资源和运行进行配置、控制和管理，包括用户身份管理、系统资源配置、系统加载和启动、系统运行的异常处理等。系统管理员的管理功能包括用户管理、服务管理、系统日志查看和内核参数设置功能。详细信息请查看帮助。");
    QString audDec =tr("    安全管理中心为安全操作系统的功能提供人机交互界面。包括系统管理员、安全管理员和审计管理员三个角色，分别提供不同的角色功能。当前登陆的用户为审计管理员，审计管理员介绍如下：\n\
    审计管理员通过对分布在系统各个组成部分的安全审计机制集中管理，包括根据安全审计策略对审计记录进行分类；提供按时间段开启和关闭相应类型的安全审计机制；对各类审计记录进行存储、管理和查询等。对审计记录进行分析，并依据分析结果进行处理。\
审计管理员管理功能包括审计信息查询、审计信息展示、审计策略配置和审计配置信息设置。详细信息请查看帮助。");
//    curRole = ROOT;
    switch(curRole)
    {
    case ROOT:
        ui->textBrowser_1->setText(des);
        roleName = tr("超级用户root");
        break;
    case SECADMIN:
        ui->textBrowser_1->setText(secDec);
        roleName = tr("安全管理员");
        break;
    case SYSADMIN:
        ui->textBrowser_1->setText(sysDec);
        roleName = tr("系统管理员");
        break;
    case AUDADMIN:
        ui->textBrowser_1->setText(audDec);
        roleName = tr("审计管理员");
        break;
    default:
        break;
    }
    try
    {
        m_indexFunModel.getCurrentUserName(uname);
        m_indexFunModel.getEnforce(enforce);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }

    QString context = tr("\n当前用户：") + uname+"\n" + tr("用户角色：") + roleName+"\n" + tr("SELinux模式：")+enforce;
    ui->textBrowser->setText(context);
    if(uname=="root")
          context = tr("\n安全中心版本：v1.0\n")+ tr("运行模式：维护模式\n") + tr("发布日期：")+date;
    else
        context = tr("\n安全中心版本：v1.0\n")+ tr("运行模式：运行模式\n") + tr("发布日期：")+date;
    ui->textBrowser_2->setText(context);
}

IndexWidget::~IndexWidget()
{
    delete ui;
}
