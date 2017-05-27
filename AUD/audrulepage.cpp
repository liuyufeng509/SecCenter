#include "audrulepage.h"
#include "ui_audrulepage.h"
#include "filerulesdialog.h"
#include "syscalldialog.h"
#include "customruledialog.h"
#include <QFile>
#define  RULE_CFG_FILE "/etc/audit/rules.d/audit.rules"
AudRulePage::AudRulePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudRulePage)
{
    ui->setupUi(this);
    updateUI();
   connect(ui->ruleListWidget, SIGNAL(currentTextChanged(const QString &)), this, SLOT(enableItemEditable()));
}

void AudRulePage::updateRuleList()
{
    try
    {
        AudFunClass::getInstance()->getCurrentRules(ruleList);
        //处理万一文件名中含有空格的情况
//        for(int i=0; i<ruleList.count(); i++)
//        {
//            int bindex = ruleList[i].indexOf("-w ");
//            int eindex = ruleList[i].indexOf(" -p ");
//            if(ruleList[i].count(" -p ")>1)
//            {
//                qDebug()<<tr("文件名或关键字中含有 -p ");
//            }
//            if(ruleList[i].count("-w ")>1)
//                {
//                qDebug()<<tr("文件名或关键字中含有-w ");
//            }

//            if(bindex==-1 || eindex==-1 || (eindex-bindex)<0)
//                {
//                continue;
//            }
//            qDebug()<<ruleList[i].mid(bindex+3,eindex-bindex);
//        }
        if(ruleList.count()==1&& ruleList[0]=="No rules")
            ruleList.clear();
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
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
        FileRulesDialog fileRuleDialog(fileRule, this);
        if(fileRuleDialog.exec()==QDialog::Accepted)
        {
            QString fileR = "-w \""+fileRule.file_name+"\""+
                    (fileRule.key_word.isEmpty()?"":" -k "+fileRule.key_word)+
                    (fileRule.auth.isEmpty()? " -p rwxa":" -p "+fileRule.auth)+
                    +" -ts "+fileRule.ts_time+" -te "+fileRule.te_time;
            ui->ruleListWidget->addItem(fileR);
            isModify= true;
        }
    }else if(ui->sysCallRadio->isChecked())
        {
        SysCallDialog sysCalDlg(sysCallRule,this);
        if(sysCalDlg.exec()==QDialog::Accepted)
        {
            QString sysR ="-a "+sysCallRule.list +" -S "+sysCallRule.sparam+
                                             " -ts "+sysCallRule.ts+" -te "+sysCallRule.te;
            ui->ruleListWidget->addItem(sysR);
            isModify= true;
        }
    }else if(ui->customRadio->isChecked())
        {
        CustomRuleDialog cusRuleDlg(this);
        if(cusRuleDlg.exec()==QDialog::Accepted)
        {
            QString cusR = cusRuleDlg.getCustomRule();
            ui->ruleListWidget->addItem(cusR);
            isModify= true;
        }
    }
}

void AudRulePage::on_cancelButton_clicked()
{
    if(isModify)
    {
        if(warnMsgBox(tr("确定要放弃所作的操作？"), this)==QMessageBox::Cancel)
            return;
        else
            {
            ui->ruleListWidget->clear();
            ui->ruleListWidget->addItems(ruleList);
        }
    }else
        {
        infoMsgBox(tr("尚未对审计规则作出修改"), this);
    }
}

void AudRulePage::on_upButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要移动的规则"), this);
        return;
    }
    if(row==0)
    {
        infoMsgBox(tr("选中规则已经处于最上层"), this);
        return;
    }

    QListWidgetItem *item = ui->ruleListWidget->takeItem(row);
    ui->ruleListWidget->insertItem(row-1, item);
    ui->ruleListWidget->setCurrentRow(row-1);
    isModify = true;
}

void AudRulePage::on_downButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要移动的规则"), this);
        return;
    }
    if(row+1==ui->ruleListWidget->count())
    {
        infoMsgBox(tr("选中规则已经处于最底层"), this);
        return;
    }

    QListWidgetItem *item = ui->ruleListWidget->takeItem(row);
    ui->ruleListWidget->insertItem(row+1, item);
    ui->ruleListWidget->setCurrentRow(row+1);
    isModify = true;
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
        infoMsgBox(tr("请选中要修改的规则"), this);
        return;
    }
    ui->ruleListWidget->currentItem()->setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable);
    ui->ruleListWidget->editItem(ui->ruleListWidget->currentItem());
    isModify = true;
}

void AudRulePage::on_delButton_clicked()
{
    int row = ui->ruleListWidget->currentRow();
    if(row<0)
        {
        infoMsgBox(tr("请选中要删除的规则"), this);
        return;
    }
    ui->ruleListWidget->takeItem(row);
    isModify = true;
}

void AudRulePage::on_nowButton_clicked()
{
//    if(!isModify)
//    {
//        infoMsgBox(tr("当前规则未作更改"));
//        return;
//    }
    try
    {
        AudFunClass::getInstance()->delAllRules();
        QStringList ruleListTmp;
        for(int i=0; i<ui->ruleListWidget->count();i++)
            {
            ruleListTmp<<ui->ruleListWidget->item(i)->text();
        }
        AudFunClass::getInstance()->setRuleList(ruleListTmp);
        isModify = false;
        ruleList = ruleListTmp;
        infoMsgBox(tr("审计规则设置成功"), this);
        updateUI();
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void AudRulePage::updateUI()
{
    updateRuleList();
    ui->ruleListWidget->clear();
    ui->ruleListWidget->addItems(ruleList);
    isModify = false;
}

void AudRulePage::on_freshButton_clicked()
{
    if(isModify)
        {
        if(warnMsgBox(tr("刷新操作将覆盖所修改的内容，请确保刷新前是否已经保存？"), this)==QMessageBox::Cancel)
            return;
        else
            {
            updateUI();
        }
    }else
        {
        updateUI();
    }
}

bool AudRulePage::isRuleExist(QString rule, QStringList ruleList)
{
    bool res = false;
    for(int i=0; i<ruleList.count(); i++)
    {
        if(ruleList[i].simplified() == rule.simplified())
            {
            res = true;
            break;
        }
    }
    return res;
}

void AudRulePage::on_forEverButton_clicked()
{
    if(warnMsgBox(tr("确定要覆盖配置文件？"), this)==QMessageBox::Cancel)
        {
        return;
    }
    ruleList.clear();
    for(int i=0; i<ui->ruleListWidget->count();i++)
        {
        ruleList<<ui->ruleListWidget->item(i)->text();
    }
    isModify = false;
    QFile file(RULE_CFG_FILE);
    QString fileStr = "";
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream inout(&file);
        fileStr = inout.readAll();
        QStringList fileConList = fileStr.split('\n');

        QStringList resList;
        //保留内核参数不能被覆盖
        for(int i=0; i<fileConList.count();i++)
        {
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-b"))
                {
                resList<<fileConList[i];
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-r"))
                {
                resList<<fileConList[i];
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-e"))
                {
                resList<<fileConList[i];
            }
            if(fileConList[i].simplified().split(' ').count()==1 && fileConList[i].contains("-i"))
                {
                resList<<fileConList[i];
            }
            if(fileConList[i].simplified().split(' ').count()==1 && fileConList[i].contains("-c"))
                {
                resList<<fileConList[i];
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-f"))
                {
                resList<<fileConList[i];
            }
        }
        resList<<ruleList;
        fileStr = "";
        for(int i=0; i<resList.count(); i++)
            {
            fileStr += resList[i] + "\n";
        }
        file.close();
//        infoMsgBox(tr("保存到配置文件成功"));
    }
    else
    {
      errMsgBox(file.errorString(), this);
      return;
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream inout(&file);
        inout<<fileStr;
        inout.flush();
        file.close();
        infoMsgBox(tr("保存到配置文件成功"), this);
    }
    else
    {
      errMsgBox(file.errorString(), this);
      return;
    }
}
