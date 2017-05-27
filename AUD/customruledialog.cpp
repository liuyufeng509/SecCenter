#include "customruledialog.h"
#include "ui_customruledialog.h"
#include "common.h"
CustomRuleDialog::CustomRuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomRuleDialog)
{
    ui->setupUi(this);

}

CustomRuleDialog::~CustomRuleDialog()
{
    delete ui;
}

QString CustomRuleDialog::getCustomRule()
{
    return customRule;
}

void CustomRuleDialog::on_cancelButton_clicked()
{
    reject();
}

void CustomRuleDialog::on_okButton_clicked()
{
    customRule = "";
    if(ui->cusRulelineEdit->text().contains("auditctl"))
    {
        infoMsgBox( tr("不需要写auditctl命令，直接写规则"), this);
        return;
    }

    if(ui->cusRulelineEdit->text().isEmpty())
    {
        infoMsgBox( tr("规则信息为空，请填写内容"), this);
        return;
    }

    customRule = ui->cusRulelineEdit->text();
    if(customRule.contains("-W "))
        {
        infoMsgBox( tr("删除操作请在主界面选中规则，点击[删除]按钮"), this);
        return;
    }
    accept();
}
