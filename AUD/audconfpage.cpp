#include "audconfpage.h"
#include "ui_audconfpage.h"
#define CONF_NAME "/etc/audit/auditd.conf"
#include<QDirModel>

AudConfPage::AudConfPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudConfPage)
{
    ui->setupUi(this);
    QDirModel *model = new QDirModel(this);
    completer = new QCompleter(this);
    completer->setModel(model);
    ui->log_filelineEdit->setCompleter(completer);
}
void AudConfPage::getAudConfigInfo()
{
    QString iniFilePath = CONF_NAME;
    QSettings settings(iniFilePath,QSettings::IniFormat);

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
    ui->mail_lineEdit->setText(audCfgInfo.warning_mail);
    ui->msg_lineEdit->setText(audCfgInfo.warning_tel);
}

AudConfPage::~AudConfPage()
{
    delete ui;
}
