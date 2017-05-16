#include "audrulepage.h"
#include "ui_audrulepage.h"
#include "filerulesdialog.h"
#include "syscalldialog.h"
#include "customruledialog.h"
AudRulePage::AudRulePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudRulePage)
{
    ui->setupUi(this);
    try
    {
        AudFunClass::getInstance()->getCurrentRules(ruleList);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
    ui->ruleListWidget->clear();
    ui->ruleListWidget->addItems(ruleList);
}

AudRulePage::~AudRulePage()
{
    delete ui;
}

void AudRulePage::on_addButton_clicked()
{
    //添加规则
    if(ui->fileRadio->isChecked())
    {
        FileRulesDialog fileRuleDialog(this);
        fileRuleDialog.exec();
    }else if(ui->sysCallRadio->isChecked())
        {
        SysCallDialog sysCalDlg(this);
        sysCalDlg.exec();
    }else if(ui->customRadio->isChecked())
        {
        CustomRuleDialog cusRuleDlg(this);
        cusRuleDlg.exec();
    }
}

void AudRulePage::on_cancelButton_clicked()
{
    warnMsgBox(tr("确定要放弃所作的操作？"));
}
