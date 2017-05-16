#include "audsvrctlpage.h"
#include "ui_audsvrctlpage.h"
#define SEV_NAME  "auditd"
AudSvrCtlPage::AudSvrCtlPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudSvrCtlPage)
{
    ui->setupUi(this);
    if(SecFunClass::getInstance()->servRunState(tr(SEV_NAME))!=RUNNING)
    {
        ui->closeAduButton->setText(tr("开启审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       尚未运行"));
    }else
    {
        ui->closeAduButton->setText(tr("关闭审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       正在运行"));
    }
}

AudSvrCtlPage::~AudSvrCtlPage()
{
    delete ui;
}

void AudSvrCtlPage::on_closeAduButton_clicked()
{
    try
    {
        if(ui->closeAduButton->text() == tr("开启审计服务"))
        {
            SecFunClass::getInstance()->startOrStopService(SEV_NAME, 0);
            ui->closeAduButton->setText(tr("关闭审计服务"));
            ui->cur_audstatus_label->setText(tr("当前状态：       正在运行"));
        }else
        {
            SecFunClass::getInstance()->startOrStopService(SEV_NAME,1);
            stop_service(SEV_NAME);
            ui->closeAduButton->setText(tr("开启审计服务"));
            ui->cur_audstatus_label->setText(tr("当前状态：       尚未运行"));
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}
