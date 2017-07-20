#include "audconfpage.h"
#include "ui_audconfpage.h"
#include<QDirModel>
#include"qreadconfig.h"

#define CONF_NAME "/etc/audit/auditd.conf"
#define SEV_NAME  "auditd"

AudConfPage::AudConfPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudConfPage)
{
    ui->setupUi(this);
    getAudConfigInfo();
    UpdateUI();
    QDirModel *model = new QDirModel(this);
    completer = new QCompleter(this);
    completer->setModel(model);
    ui->log_filelineEdit->setCompleter(completer);
    if(QGlobalClass::getInstance()->audCfgInfo.warn)
    {
        ui->groupBox_15->setHidden(false);
        ui->okButton->setGeometry(460,400, 85,31);
        ui->restartAduButton->setGeometry(10,400,85,31);
        ui->groupBox_6->setMinimumHeight(440);
    }else
    {
        ui->groupBox_15->setHidden(true);
        ui->okButton->setGeometry(460,290, 85,31);
        ui->restartAduButton->setGeometry(10,290,85,31);
        ui->groupBox_6->setMinimumHeight(332);
    }
    if(QGlobalClass::getInstance()->audCfgInfo.audctl)
        ui->restartAduButton->setHidden(false);
    else
        ui->restartAduButton->setHidden(true);
    QIntValidator * v = new QIntValidator (0, Max_INPUT, this);
     ui->max_log_filelineEdit->setValidator(v);
     ui->num_logslineEdit->setValidator(v);
     ui->admin_space_left_lineEdit->setValidator(v);
     ui->space_left_lineEdit->setValidator(v);
}
void AudConfPage::getAudConfigInfo()
{
    QString iniFilePath = CONF_NAME;
    QSettings settings(iniFilePath,QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    audCfgInfo.log_file = settings.value("log_file").toString();
    audCfgInfo.max_log_file = settings.value("max_log_file").toString();
    audCfgInfo.max_log_file_action = settings.value("max_log_file_action").toString();
    audCfgInfo.num_logs = settings.value("num_logs").toString();
    audCfgInfo.admin_space_left = settings.value("admin_space_left").toString();
    audCfgInfo.admin_space_left_action = settings.value("admin_space_left_action").toString();
    audCfgInfo.space_left = settings.value("space_left").toString();
    audCfgInfo.space_left_action = settings.value("space_left_action").toString();
    audCfgInfo.disk_full_action = settings.value("disk_full_action").toString();
    audCfgInfo.disk_error_action = settings.value("disk_error_action").toString();
    audCfgInfo.warning_mail = settings.value("warning_mail").toString();
    audCfgInfo.warning_tel = settings.value("warning_tel").toString();
    if(!QGlobalClass::getInstance()->audCfgInfo.warn)    //不配置报警信息时，如果是空，则需要设置一个默认值
    {
        if(audCfgInfo.warning_mail.isEmpty())
        {
            audCfgInfo.warning_mail="test@nfs-qd.com";
        }
        if(audCfgInfo.warning_tel.isEmpty())
        {
            audCfgInfo.warning_tel="18602345678";
        }
    }

}

#define SET_CURRENT_TEXT(ui, data)  \
for(int i=0; i<ui->count();i++)   \
{   \
    if(ui->itemText(i) == data)   \
    {   \
        qDebug()<<"currenttext:"<<ui->itemText(i)<<" data:"<<data<<" index:"<<i;  \
        ui->setCurrentIndex(i);   \
        break;  \
    }   \
}
void AudConfPage::UpdateUI()
{
    ui->log_filelineEdit->setText(audCfgInfo.log_file);
    ui->max_log_filelineEdit->setText(audCfgInfo.max_log_file);
    SET_CURRENT_TEXT(ui->max_log_file_action_comboBox, audCfgInfo.max_log_file_action);
    ui->num_logslineEdit->setText(audCfgInfo.num_logs);

    ui->admin_space_left_lineEdit->setText(audCfgInfo.admin_space_left);
    SET_CURRENT_TEXT(ui->admin_space_left_action_comboBox, audCfgInfo.admin_space_left_action);
    SET_CURRENT_TEXT(ui->space_left_action_comboBox, audCfgInfo.space_left_action);
    ui->space_left_lineEdit->setText(audCfgInfo.space_left);
    SET_CURRENT_TEXT(ui->disk_full_action_comboBox, audCfgInfo.disk_full_action);
    SET_CURRENT_TEXT(ui->disk_error_action_comboBox, audCfgInfo.disk_error_action);
    //if(QReadConfig::getInstance()->audCfgInfo.warn)
    {
        ui->mail_lineEdit->setText(audCfgInfo.warning_mail);
        ui->msg_lineEdit->setText(audCfgInfo.warning_tel);
    }
}

void AudConfPage::saveDataFromUI()
{
    audCfgInfo.log_file = ui->log_filelineEdit->text();
    audCfgInfo.max_log_file = ui->max_log_filelineEdit->text();
    audCfgInfo.max_log_file_action = ui->max_log_file_action_comboBox->currentText();
    audCfgInfo.num_logs = ui->num_logslineEdit->text();
    audCfgInfo.admin_space_left = ui->admin_space_left_lineEdit->text();
    audCfgInfo.admin_space_left_action = ui->admin_space_left_action_comboBox->currentText();
    audCfgInfo.space_left_action = ui->space_left_action_comboBox->currentText();
    audCfgInfo.space_left = ui->space_left_lineEdit->text();
    audCfgInfo.disk_full_action = ui->disk_full_action_comboBox->currentText();
    audCfgInfo.disk_error_action = ui->disk_error_action_comboBox->currentText();
    if(QGlobalClass::getInstance()->audCfgInfo.warn)
    {
        audCfgInfo.warning_mail = ui->mail_lineEdit->text();
        audCfgInfo.warning_tel = ui->msg_lineEdit->text();
    }
}

AudConfPage::~AudConfPage()
{
    delete ui;
}

void AudConfPage::on_okButton_clicked()
{
         saveDataFromUI();
//    if(audCfgInfo.admin_space_left_action==tr("EMAIL")||audCfgInfo.space_left_action==tr("EMAIL"))
//    {
//        if(audCfgInfo.warning_mail.isEmpty())
//        {
//            //QMessageBox::information(this, tr("提示"), tr("选择EMAIL时，收件人不能为空"));
//            errMsgBox(tr("选择EMAIL时，收件人不能为空"));
//            return;
//        }
//    }

    if(QGlobalClass::getInstance()->audCfgInfo.warn)
    {
        QStringList mails = audCfgInfo.warning_mail.split(',');
        QStringList tels = audCfgInfo.warning_tel.split(',');
        for(int i=0; i<mails.count(); i++)
        {
            if(!mails[i].contains("@"))
            {
                errMsgBox(tr("请填写正确的邮箱格式"), this);
                return;
            }
        }

        for(int i=0; i<tels.count(); i++)
        {
            if(tels[i].length()!=11||tels[i].toLongLong()==0)
            {
                errMsgBox(tr("请确认手机号是11位的数字"), this);
                return;
            }
        }
    }


    QFile file(CONF_NAME);
    if ( file.open( QFile::ReadWrite| QFile::Text) )
    {
            QTextStream stream( &file );
            QString line;
          while ( !stream.atEnd() )
          {
              line = stream.readLine(); // 不包括“\n”的一行文本
              QStringList lineargs = line.split("=");
              if(lineargs.length()!=2)
                continue;
              config_settings.insert(lineargs[0].trimmed(), lineargs[1].trimmed());
          }
           file.close();
          config_settings.insert("log_file", audCfgInfo.log_file);
          config_settings.insert("max_log_file", audCfgInfo.max_log_file);
          config_settings.insert("max_log_file_action", audCfgInfo.max_log_file_action);
          config_settings.insert("num_logs", audCfgInfo.num_logs);
          config_settings.insert("admin_space_left", audCfgInfo.admin_space_left);
          config_settings.insert("admin_space_left_action", audCfgInfo.admin_space_left_action);
          config_settings.insert("space_left", audCfgInfo.space_left);
          config_settings.insert("space_left_action", audCfgInfo.space_left_action);
          config_settings.insert("disk_full_action", audCfgInfo.disk_full_action);
          config_settings.insert("disk_error_action", audCfgInfo.disk_error_action);
          config_settings.insert("warning_mail", audCfgInfo.warning_mail);
          config_settings.insert("warning_tel", audCfgInfo.warning_tel);
    }
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream stream( &file );
        QMapIterator<QString, QString> i(config_settings);
          while (i.hasNext())
          {
              i.next();
              stream<<(i.key() + " = " + i.value()+"\n");
          }
        file.close();
    }
    infoMsgBox(tr("写入完成"), this);
}

void AudConfPage::on_restartAduButton_clicked()
{
    try
    {
        AudFunClass::getInstance()->startOrStopService(SEV_NAME, 2);
        infoMsgBox(tr("重启成功"), this);
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }
}
