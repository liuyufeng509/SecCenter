#include "audkernrulepage.h"
#include "ui_audkernrulepage.h"
#define  RULE_CFG_FILE "/etc/audit/rules.d/audit.rules"

AudKernRulePage::AudKernRulePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudKernRulePage)
{
    ui->setupUi(this);

    QIntValidator * v = new QIntValidator (0, Max_INPUT, this);
     ui->max_noaud_buf_lineEdit->setValidator(v);
     ui->rate_limit_lineEdit->setValidator(v);

    kernAudParam.bignore = false;
    try
    {
        AudFunClass::getInstance()->getKernAudParam(kernAudParam);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
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
        infoMsgBox(tr("设置内核参数规则成功"), this);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void AudKernRulePage::on_forEverButton_clicked()
{
    if(warnMsgBox(tr("确定要覆盖配置文件？"), this)==QMessageBox::Cancel)
        {
        return;
    }
    saveDataFromUI();
    QFile file(RULE_CFG_FILE);
    QString fileStr = "";
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream inout(&file);
        fileStr = inout.readAll();
        QStringList fileConList = fileStr.split('\n');
        QStringList resList;
        //保留原有的规则，但是覆盖内核参数。
        bool backlog_limit=false, rate_limit=false,enable=false,bignore=false,fail_flag=false;
        for(int i=0; i<fileConList.count();i++)
        {
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-b")&&(!fileConList[i].contains("#")))
                {
                fileConList[i] = "-b "+kernAudParam.backlog_limit;
                backlog_limit=true;
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-r")&&(!fileConList[i].contains("#")))
                {
                 fileConList[i] = "-r "+kernAudParam.rate_limit;
                 rate_limit = true;
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-e")&&(!fileConList[i].contains("#")))
                {
                fileConList[i] = "-e "+kernAudParam.enable;
                enable=true;
            }
            if(fileConList[i].simplified().split(' ').count()==1 && fileConList[i].contains("-i")&&(!fileConList[i].contains("#")))
                {
                fileConList[i] = kernAudParam.bignore?"-i":"-c";
                bignore=true;
            }
            if(fileConList[i].simplified().split(' ').count()==1 && fileConList[i].contains("-c")&&(!fileConList[i].contains("#")))
                {
                fileConList[i] = kernAudParam.bignore?"-i":"-c";
                bignore=true;
            }
            if(fileConList[i].simplified().split(' ').count()==2 && fileConList[i].contains("-f")&&(!fileConList[i].contains("#")))
                {
                fileConList[i] = "-f "+kernAudParam.fail_flag;
                fail_flag=true;
            }
        }

        if(!backlog_limit)
            {
            fileConList.prepend("-b "+kernAudParam.backlog_limit);
        }
        if(!rate_limit)
            {
            fileConList.prepend("-r "+kernAudParam.rate_limit);
        }
        if(!enable)
            {
            fileConList.prepend("-e "+kernAudParam.enable);
        }
        if(!bignore)
            {
            fileConList.prepend(kernAudParam.bignore?"-i":"-c");
        }
        if(!fail_flag)
            {
            fileConList.prepend("-f "+kernAudParam.fail_flag);
        }

        fileStr = "";
        for(int i=0; i<fileConList.count(); i++)
            {
            fileStr += fileConList[i] + "\n";
        }
        file.close();
       // infoMsgBox(tr("保存到配置文件成功"));
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
