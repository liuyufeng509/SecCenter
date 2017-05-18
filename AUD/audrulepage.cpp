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
    updateRuleList();
    ui->ruleListWidget->clear();
    ui->ruleListWidget->addItems(ruleList);

   connect(ui->ruleListWidget, SIGNAL(currentTextChanged(const QString &)), this, SLOT(enableItemEditable()));
}

void AudRulePage::updateRuleList()
{
    try
    {
        AudFunClass::getInstance()->getCurrentRules(ruleList);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
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

void AudRulePage::on_upButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要移动的规则"));
        return;
    }
    if(row==0)
    {
        infoMsgBox(tr("选中规则已经处于最上层"));
        return;
    }

    QListWidgetItem *item = ui->ruleListWidget->takeItem(row);
    ui->ruleListWidget->insertItem(row-1, item);
    ui->ruleListWidget->setCurrentRow(row-1);
}

void AudRulePage::on_downButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要移动的规则"));
        return;
    }
    if(row+1==ui->ruleListWidget->count())
    {
        infoMsgBox(tr("选中规则已经处于最底层"));
        return;
    }

    QListWidgetItem *item = ui->ruleListWidget->takeItem(row);
    ui->ruleListWidget->insertItem(row+1, item);
    ui->ruleListWidget->setCurrentRow(row+1);
}

void AudRulePage::enableItemEditable()
{
    if(ui->ruleListWidget->currentItem()==NULL)
        return;
    ui->ruleListWidget->currentItem()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}

void AudRulePage::on_modButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要修改的规则"));
        return;
    }
    ui->ruleListWidget->currentItem()->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
    ui->ruleListWidget->editItem(ui->ruleListWidget->currentItem());
}

void AudRulePage::on_delButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要删除的规则"));
        return;
    }
    ui->ruleListWidget->takeItem(row);
}
