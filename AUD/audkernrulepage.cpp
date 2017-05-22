#include "audkernrulepage.h"
#include "ui_audkernrulepage.h"
#define  RULE_CFG_FILE "/etc/audit/rules.d/audit.rules"
AudKernRulePage::AudKernRulePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudKernRulePage)
{
    ui->setupUi(this);

    QRegExp regExp("^0$|^\\+?[1-9]\\d*$");  //注意\为\\
    ui->max_noaud_buf_lineEdit->setValidator(new QRegExpValidator(regExp,this));
    ui->rate_limit_lineEdit->setValidator(new QRegExpValidator(regExp,this));
    kernAudParam.bignore = false;
    try
    {
        AudFunClass::getInstance()->getKernAudParam(kernAudParam);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
    updateUI();
}

void AudKernRulePage::saveDataFromUI()
{
    kernAudParam.backlog_limit = ui->max_noaud_buf_lineEdit->text();
    if(ui->silent_radioButton->isChecked())
        kernAudParam.fail_flag = "0";
    if(ui->printk_radioButton->isChecked())
        kernAudParam.fail_flag = "1";
    if(ui->panic_radioButton->isChecked())
        kernAudParam.fail_flag = "2";
    ui->enable_radioButton->isChecked()? kernAudParam.enable = "1":kernAudParam.enable="0";
    ui->audctl_i_radioButton->isChecked()?kernAudParam.bignore = true:kernAudParam.bignore=false;
    kernAudParam.rate_limit = ui->rate_limit_lineEdit->text();
}

void AudKernRulePage::updateUI()
{
    ui->max_noaud_buf_lineEdit->setText(kernAudParam.backlog_limit);
    kernAudParam.enable.toInt()==1? ui->enable_radioButton->setChecked(true):ui->disable_radioButton->setChecked(true);
    switch (kernAudParam.fail_flag.toInt()) {
    case 0:
    {
        ui->silent_radioButton->setChecked(true);
    }
        break;
    case 1:
    {
        ui->printk_radioButton->setChecked(true);
    }
        break;
    case 2:
    {
        ui->panic_radioButton->setChecked(true);
    }
        break;
    default:
        break;
    }
    kernAudParam.bignore? ui->audctl_i_radioButton->setChecked(true):ui->audctl_c_radioButton->setChecked(true);
    ui->rate_limit_lineEdit->setText(kernAudParam.rate_limit);
}

AudKernRulePage::~AudKernRulePage()
{
    delete ui;
}

void AudKernRulePage::on_aplButton_clicked()
{
    saveDataFromUI();
    try
    {
        AudFunClass::getInstance()->setKernAduParam(kernAudParam);
        infoMsgBox(tr("设置内核参数规则成功"));
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}

void AudKernRulePage::on_forEverButton_clicked()
{
    if(warnMsgBox(tr("确定要覆盖配置文件？"))==QMessageBox::Cancel)
        {
        return;
    }
    saveDataFromUI();
    QFile file(RULE_CFG_FILE);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream inout(&file);
        QString fileStr = inout.readAll();
        QStringList fileConList = fileStr.split('\n');
        QStringList resList;
        //保留原有的规则，但是覆盖内核参数。
        for(int i=0; i<fileConList.count();i++)
        {
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-b"))
                {
                fileConList[i] = "-b "+kernAudParam.backlog_limit;
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-r"))
                {
                 fileConList[i] = "-r "+kernAudParam.rate_limit;
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-e"))
                {
                fileConList[i] = "-e "+kernAudParam.enable;
            }
            if(fileConList[i].simplified().split(' ').count()==1 && fileConList[i].contains("-i"))
                {
                fileConList[i] = kernAudParam.bignore?" -i":" -c";
            }
            if(fileConList[i].simplified().split(' ').count()==1 && fileConList[i].contains("-c"))
                {
                fileConList[i] = kernAudParam.bignore?" -i":" -c";
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-f"))
                {
                fileConList[i] = "-f "+kernAudParam.fail_flag;
            }
        }
        fileStr = "";
        for(int i=0; i<resList.count(); i++)
            {
            fileStr += resList[i] + "\n";
        }
        inout<<fileStr;
        inout.flush();
        file.close();
        infoMsgBox(tr("保存到配置文件成功"));
    }
    else
    {
      errMsgBox(file.errorString());
    }
}
