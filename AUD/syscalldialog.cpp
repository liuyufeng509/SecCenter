#include "syscalldialog.h"
#include "ui_syscalldialog.h"
#include "common.h"
SysCallDialog::SysCallDialog(SysCallRule &sysCallRule,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysCallDialog),
    sysCallRule(sysCallRule)
{
    ui->setupUi(this);
    this->setMaximumHeight(367);
    this->setMaximumWidth(555);
    ui->sys_call_et_TimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->sys_call_st_TimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_list_group_radioButton_clicked()));
    ui->label_47->setHidden(true);
    ui->label_48->setHidden(true);
    ui->sys_call_et_TimeEdit->setHidden(true);
    ui->sys_call_st_TimeEdit->setHidden(true);
    ui->user_syscall_radioButton->setHidden(true);
    ui->exclude_radioButton->setHidden(true);
    ui->label_51->setHidden(true);
    ui->msgtype_lineEdit->setHidden(true);
}

void SysCallDialog::on_list_group_radioButton_clicked()
{
    ui->msgtype_lineEdit->setEnabled(ui->exclude_radioButton->isChecked());
}

//void SysCallDialog::mouseReleaseEvent(QMouseEvent *e)
//{
//    qDebug()<<this->size().width()<<"  "<<this->size().height();
//}

SysCallDialog::~SysCallDialog()
{
    delete ui;
}

void SysCallDialog::on_okButton_clicked()
{
    if(ui->sys_call_st_TimeEdit->dateTime() > ui->sys_call_et_TimeEdit->dateTime())
        {
        errMsgBox(tr("起始时间不能超过结束时间"),this);
        return;
    }
    QString liststr="", actionstr="";
    actionstr =ui->never_radioButton->isChecked()?"never ":"always ";
    if(ui->entry_radioButton->isChecked())
    {
        liststr = QString("entry,") + actionstr;
    }else if(ui->exit_radioButton->isChecked())
    {
        liststr = QString("exit,")+actionstr;
    }else if(ui->user_syscall_radioButton->isChecked())
    {
        liststr = QString("user,")+actionstr;
    }else if(ui->exclude_radioButton->isChecked())
    {
        liststr = QString("exclude,")+actionstr+ (ui->msgtype_lineEdit->text().isEmpty()?"":" -F msgtype="+ui->msgtype_lineEdit->text());
    }

    sysCallRule.list = liststr;
    sysCallRule.sparam = ui->buttonGroup_3->checkedButton()->text();
    sysCallRule.ts = ui->sys_call_st_TimeEdit->text();
    sysCallRule.te = ui->sys_call_et_TimeEdit->text();
    accept();
//    QString cmdstr = "auditctl -a "+liststr +" -S "+ui->buttonGroup_3->checkedButton()->text()+
//            " -ts "+ui->sys_call_st_TimeEdit->text()+" -te "+ui->sys_call_et_TimeEdit->text();

//    try
//    {
//        QString rs;
//        AudFunClass::getInstance()->excuteAudCmd(cmdstr, tr("设置系统调用规则"), rs);
//        infoMsgBox(tr("设置系统调用规则成功"));
//    }catch(Exception exp)
//            {
//        errMsgBox(exp.getErroWhat());
//    }
}
void SysCallDialog::on_cancelButton_clicked()
{
    reject();
}
