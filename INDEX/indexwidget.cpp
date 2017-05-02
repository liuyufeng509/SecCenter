#include "indexwidget.h"
#include "ui_indexwidget.h"

IndexWidget::IndexWidget(ROLE curRole,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IndexWidget)
{
    ui->setupUi(this);
    QString uname = "";
    QString roleName = "";
    QString enforce = "";
    switch(curRole)
    {
    case ROOT:
        ui->textEdit->setText(tr("    root用户具有所有角色的权限，能够使用安全管理员、系统管理员、审计管理员以及常规操作的所有功能。"));
        roleName = tr("超级用户root");
        break;
    case SECADMIN:
        ui->textEdit->setText(tr("    安全管理员提供身份鉴别、用户标识、程序可信执行等安全设置和操作相关功能。"));
        roleName = tr("安全管理员");
        break;
    case SYSADMIN:
        ui->textEdit->setText(tr("   系统管理员提供开关机、启动停止服务、用户管理等一系列系统管理操作。 "));
        roleName = tr("系统管理员");
        break;
    case AUDIADMIN:
        ui->textEdit->setText(tr("    审计管理员负责审计系统的相关功能，比如查看审计信息、生成审计报表、修改审计规则等。"));
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

    QString context = tr("当前用户：") + uname+"\n" + tr("用户角色：") + roleName+"\n" + tr("SELinux模式：")+enforce;
    ui->textBrowser->setText(context);
    if(uname=="root")
          context = tr("安全中心版本：v1.0\n")+ tr("安全中心模式：维护模式\n") + tr("SELinux模式：")+enforce;
    else
        context = tr("安全中心版本：v1.0\n")+ tr("安全中心模式：运行模式\n") + tr("SELinux模式：")+enforce;
    ui->textBrowser_2->setText(context);

}

IndexWidget::~IndexWidget()
{
    delete ui;
}
