#include "tabauditpage.h"
#include "ui_tabauditpage.h"
#include<QFileDialog>
#include "querydialog.h"
#include"reportdialog.h"
#include"ruledialog.h"
#include"configdialog.h"
#define SEV_NAME  "auditd"
#define CONF_NAME "/etc/audit/auditd.conf"
#define RULE_CONF_NAME  QString("/etc/audit/rules.d/audit.rules")

TabAuditPage::TabAuditPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabAuditPage),
    op_type(AUD_DISPLAY)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

    //open/close auditservice
    if(is_serv_running(tr(SEV_NAME))!=RUNNING)
    {
        ui->closeAduButton->setText(tr("开启审计服务"));
        ui->audStatusEdit->setText(tr("审计服务未运行"));
    }else
    {
        ui->closeAduButton->setText(tr("关闭审计服务"));
        ui->audStatusEdit->setText(tr("审计服务正在运行"));
    }

    //query fun
    set_query_fun_ui();
    set_signal_slot();

    //report fun

    //audit rules
    QRegExp regExp("^\\d{1,}$");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$
    ui->max_noaud_buf_lineEdit->setValidator(new QRegExpValidator(regExp,this));
    ui->rate_limit_lineEdit->setValidator(new QRegExpValidator(regExp,this));
    kernAudParam.bignore = false;
    get_kern_aud_param(kernAudParam);
    update_kern_aud_param_ui();

    //file rules
    ui->file_aud_st_TimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->file_aud_et_TimeEdit->setDateTime(QDateTime::currentDateTime());

    //syscall rules
    connect(ui->buttonGroup_6, SIGNAL(buttonClicked(int)), this, SLOT(on_list_group_radioButton_clicked()));

    //custom rules

    //audit config
    get_aud_config_info();
    update_aud_config_ui();

    //display

}

void TabAuditPage::read_and_display_file()
{
    QFile file(fileName);
    if ( file.open( QFile::ReadOnly | QFile::Text) ) {
            QTextStream stream( &file );
            ui->textBrowser->setText(stream.readAll());
            file.close();
            op_type = AUD_DISPLAY;
            ui->fresh_pushButton->setToolTip(tr("刷新类型：审计日志"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("日志文件打开失败"));
    }
}

void TabAuditPage::on_open_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),audCfgInfo.log_file);
    if(fileName.isEmpty())
    {
        return;
    }

    read_and_display_file();

}

void TabAuditPage::on_fresh_pushButton_clicked()
{
    ui->textBrowser->setText("");
    switch (op_type) {
    case QUERY_SET:
        on_aplButton_clicked();
        break;
    case AUD_REPORT:
        on_report_okButton_clicked();
        break;
//    case KER_AUD_PARM:
//        on_apl_kern_paramButton_clicked();

//        break;
    case AUD_DISPLAY:
        read_and_display_file();
        break;
    default:
        break;
    }
}


void TabAuditPage::get_aud_config_info()
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

void TabAuditPage::update_aud_config_ui()
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

void TabAuditPage::update_kern_aud_param_ui()
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

TabAuditPage::~TabAuditPage()
{
    delete ui;
}

void TabAuditPage::on_closeAduButton_clicked()
{
    if(ui->closeAduButton->text() == tr("开启审计服务"))
    {
        if(!start_service(SEV_NAME))
        {
            QMessageBox::information(this, tr("提示"), tr("启动失败"));
            return;
        }
        ui->closeAduButton->setText(tr("关闭审计服务"));
        ui->audStatusEdit->setText(tr("审计服务正在运行"));
    }else
    {
        if(!stop_service(SEV_NAME))
        {
            QMessageBox::information(this, tr("提示"), tr("停止失败"));
            return;
        }
        ui->closeAduButton->setText(tr("开启审计服务"));
        ui->audStatusEdit->setText(tr("审计服务未运行"));
    }
}

void TabAuditPage::set_signal_slot()
{
    connect(ui->evidCheckBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->gid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->agid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->egid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->kw_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->hn_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->mt_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->rs_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->ppid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->pid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->fn_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->word_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->term_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->ef_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->euid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
   // connect(ui->dac_set_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->uid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->luid_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
 //   connect(ui->dac_get_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
//    connect(ui->avc_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->syscall_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));

    connect(ui->st_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
    connect(ui->et_checkBox, SIGNAL(clicked(bool)), this, SLOT(set_query_fun_ui()));
}

void TabAuditPage::set_query_fun_ui()
{
    ui->evid_lineEdit->setEnabled(ui->evidCheckBox->isChecked());
    ui->gid_lineEdit->setEnabled(ui->gid_checkBox->isChecked());
    ui->agid_lineEdit->setEnabled(ui->agid_checkBox->isChecked());
    ui->egid_lineEdit->setEnabled(ui->egid_checkBox->isChecked());
    ui->kw_lineEdit->setEnabled(ui->kw_checkBox->isChecked());
    ui->hn_lineEdit->setEnabled(ui->hn_checkBox->isChecked());
    ui->mt_lineEdit->setEnabled(ui->mt_checkBox->isChecked());
    ui->rs_lineEdit->setEnabled(ui->rs_checkBox->isChecked());
    ui->ppid_lineEdit->setEnabled(ui->ppid_checkBox->isChecked());
    ui->pid_lineEdit->setEnabled(ui->pid_checkBox->isChecked());
    ui->fn_lineEdit->setEnabled(ui->fn_checkBox->isChecked());
    ui->word_lineEdit->setEnabled(ui->word_checkBox->isChecked());
    ui->auid_lineEdit->setEnabled(ui->auid_checkBox->isChecked());
    ui->term_lineEdit->setEnabled(ui->term_checkBox->isChecked());
    ui->ef_lineEdit->setEnabled(ui->ef_checkBox->isChecked());
    ui->euid_lineEdit->setEnabled(ui->euid_checkBox->isChecked());
 //   ui->dac_set_lineEdit->setEnabled(ui->dac_set_checkBox->isChecked());
    ui->uid_lineEdit->setEnabled(ui->uid_checkBox->isChecked());
    ui->luid_lineEdit->setEnabled(ui->luid_checkBox->isChecked());
//    ui->dac_get_lineEdit->setEnabled(ui->dac_get_checkBox->isChecked());
 //   ui->avc_lineEdit->setEnabled(ui->avc_checkBox->isChecked());
    ui->syscall_lineEdit->setEnabled(ui->syscall_checkBox->isChecked());
    ui->st_timeEdit->setEnabled(ui->st_checkBox->isChecked());
    ui->et_timeEdit->setEnabled(ui->et_checkBox->isChecked());
    ui->st_timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->et_timeEdit->setDateTime(QDateTime::currentDateTime());
}

#define EMPTY(param, param2) \
   {    \
    if(param->text().isEmpty() && param2->isChecked())    \
    {   \
        QMessageBox::information(this, tr("提示"), param->objectName()+tr("参数为空")); \
        return false;   \
    }   \
}

bool TabAuditPage::check_param()
{

    EMPTY(ui->evid_lineEdit, ui->evidCheckBox);
    EMPTY(ui->gid_lineEdit, ui->gid_checkBox);
    EMPTY(ui->agid_lineEdit, ui->agid_checkBox);
    EMPTY(ui->egid_lineEdit, ui->egid_checkBox);
    EMPTY(ui->kw_lineEdit, ui->kw_checkBox);
    EMPTY(ui->hn_lineEdit,ui->hn_checkBox);
    EMPTY(ui->mt_lineEdit,ui->mt_checkBox);
    EMPTY(ui->rs_lineEdit, ui->rs_checkBox);
    EMPTY(ui->ppid_lineEdit, ui->ppid_checkBox);
    EMPTY(ui->pid_lineEdit, ui->pid_checkBox);
    EMPTY(ui->fn_lineEdit, ui->fn_checkBox);
    EMPTY(ui->word_lineEdit, ui->word_checkBox);
    EMPTY(ui->auid_lineEdit, ui->auid_checkBox);
    EMPTY(ui->term_lineEdit, ui->term_checkBox);
    EMPTY(ui->ef_lineEdit, ui->ef_checkBox);
    EMPTY(ui->euid_lineEdit, ui->euid_checkBox);
  //  EMPTY(ui->dac_set_lineEdit, ui->dac_set_checkBox);
    EMPTY(ui->uid_lineEdit, ui->uid_checkBox);
    EMPTY(ui->luid_lineEdit, ui->luid_checkBox);
  //  EMPTY(ui->dac_get_lineEdit, ui->dac_get_checkBox);
   // EMPTY(ui->avc_lineEdit, ui->avc_checkBox);
    EMPTY(ui->syscall_lineEdit, ui->syscall_checkBox);
    EMPTY(ui->st_timeEdit, ui->st_checkBox);
    EMPTY(ui->et_timeEdit, ui->et_checkBox);
    return true;
}

void TabAuditPage::on_query_produceButton_clicked()
{

    if(!check_param())
    {
        //cmd = "";
        return;
    }

    cmd = "ausearch -i "+
            (ui->evidCheckBox->isChecked()? "--event "+ui->evid_lineEdit->text():"")+" "+
            (ui->gid_checkBox->isChecked()? "-gi "+ui->gid_lineEdit->text():"") + " "+
            (ui->fn_checkBox->isChecked()? "-f "+ui->fn_lineEdit->text():"") + " "+
            (ui->agid_checkBox->isChecked()? "-ga "+ui->agid_lineEdit->text():"")+" "+
            (ui->egid_checkBox->isChecked()? "-ge "+ui->egid_lineEdit->text():"")+" "+
            (ui->kw_checkBox->isChecked()? "-k "+ui->kw_lineEdit->text():"") +" "+
            (ui->hn_checkBox->isChecked()?"-hn "+ui->hn_lineEdit->text():"")+" "+
            (ui->ppid_checkBox->isChecked()? "-pp "+ui->ppid_lineEdit->text():"")+" "+
            (ui->mt_checkBox->isChecked()? "-m " +ui->mt_lineEdit->text():"")+ " "+
            (ui->rs_checkBox->isChecked()? "-sv "+ui->rs_lineEdit->text():"") + " "+
            (ui->pid_checkBox->isChecked()? "-p " +ui->pid_lineEdit->text():"")+" "+
            (ui->st_checkBox->isChecked()? "-ts "+ui->st_timeEdit->text():"")+ " "+
            (ui->et_checkBox->isChecked()? "-te "+ui->et_timeEdit->text():"")+" "+
            (ui->auid_checkBox->isChecked()? "-ua "+ui->auid_lineEdit->text():"") + " "+
            (ui->term_checkBox->isChecked()? "-tm "+ui->term_lineEdit->text():"")+ " "+
            (ui->word_checkBox->isChecked()? "-w "+ui->word_lineEdit->text():"")+" "+
            (ui->euid_checkBox->isChecked()? "-ue "+ui->euid_lineEdit->text():"")+" "+
            //(ui->dac_set_checkBox->isChecked()? "-")
            //(ui->dac_get_checkBox->isChecked()? "-")
            (ui->luid_checkBox->isChecked()? "-ul "+ui->luid_lineEdit->text():"")+" "+
            (ui->ef_checkBox->isChecked()? "-x "+ui->ef_lineEdit->text():"")+" "+
            (ui->uid_checkBox->isChecked()? "-ui "+ui->uid_lineEdit->text():"")+" "+
            (ui->syscall_checkBox->isChecked()? "-sc "+ui->syscall_lineEdit->text():"")+ " "+
           // (ui->avc_checkBox->isChecked()? "-")
            "";
    ui->query_sent_lineEdit->setText(cmd);
}

void TabAuditPage::on_aplButton_clicked()
{
    //ui->listWidget->setCurrentItem(5);      //turn to display page
    cmd = ui->query_sent_lineEdit->text();
    if(excute_aud_cmd(cmd, res))
    {
        ui->listWidget->setCurrentRow(5);
        ui->textBrowser->setText(res);
        op_type = QUERY_SET;
        ui->fresh_pushButton->setToolTip(tr("刷新类型：审计查询"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("执行查询语句失败，请检查查询语句"));
    }
    //emit emitQuerySentence()
}

void TabAuditPage::on_sv_to_fileButton_clicked()
{
   on_query_produceButton_clicked();
   cmd = ui->query_sent_lineEdit->text();
   QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                ".");
   QFile file(fileName);//创建一个文件对象，存储用户选择的文件
   if (!file.open(QIODevice::ReadWrite))
   {
        return;
   }

   QTextStream out(&file);
   out<<cmd<<endl;
   out.flush();
   file.close();
}

void TabAuditPage::on_res_checkBox_clicked()
{
    ui->sucButton->setEnabled(ui->res_checkBox->isChecked());
    ui->failButton->setEnabled(ui->res_checkBox->isChecked());
}
void TabAuditPage::on_tm_sec_checkBox_clicked()
{
    ui->st_TimeEdit->setEnabled(ui->tm_sec_checkBox->isChecked());
    ui->et_TimeEdit->setEnabled(ui->tm_sec_checkBox->isChecked());
    ui->st_TimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->et_TimeEdit->setDateTime(QDateTime::currentDateTime());
}

void TabAuditPage::on_report_okButton_clicked()
{
    cmd = QString("aureport ") + (ui->res_checkBox->isChecked()? (ui->sucButton->isChecked()?"--success ":" --failed "):"")+
            (ui->tm_sec_checkBox->isChecked()? ("-ts "+ui->st_TimeEdit->text() +" -te "+ui->et_TimeEdit->text()+ " "):"")+
            (ui->rep_gener_checkBox->isChecked()?"--summary ":"")+
            (ui->avc_radioButton->isChecked()? "-a ":"")+
            (ui->file_radioButton->isChecked()?"-f ":"")+
            (ui->logt_radioButton->isChecked()?"-t ":"")+
            (ui->config_radioButton->isChecked()? "-c ":"")+
            (ui->acc_change_radioButton->isChecked()?"-m ":"")+
            (ui->pid_radioButton->isChecked()? "-p ":"")+
            (ui->sys_call_radioButton->isChecked()?"-s ":"")+
            (ui->term_radioButton->isChecked()?"-tm ":"")+
            (ui->aud_evt_radioButton->isChecked()?"-e ":"")+
            (ui->exe_file_radioButton->isChecked()?"-x ":"")+
            (ui->user_radioButton->isChecked()?"-u ":"")+
            (ui->kw_radioButton->isChecked()?"-k ":"");

    if(excute_aud_cmd(cmd, res))
    {
        ui->listWidget->setCurrentRow(5);
        ui->textBrowser->setText(res);
        op_type=AUD_REPORT;
        ui->fresh_pushButton->setToolTip(tr("刷新类型：审计报表"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("生成报表失败"));
    }
}

void TabAuditPage::save_kern_aud_param_from_ui()
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

void TabAuditPage::on_apl_kern_paramButton_clicked()
{

    save_kern_aud_param_from_ui();
   cmd = "auditctl -r "+kernAudParam.rate_limit+ " -e "+ kernAudParam.enable+
            " -b "+kernAudParam.backlog_limit+ " -f "+kernAudParam.fail_flag+(kernAudParam.bignore?" -i":" -c");
    QString rs;
    if(!excute_aud_cmd(cmd,rs ))
    {
        QMessageBox::information(this, tr("提示"), tr("内核审计参数设置失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("内核审计参数设置成功"));
        op_type=KER_AUD_PARM;
    }
}

void TabAuditPage::save_aud_config_from_ui()
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
    audCfgInfo.warning_mail = ui->mail_lineEdit->text();
    audCfgInfo.warning_tel = ui->msg_lineEdit->text();
}

void TabAuditPage::on_ker_aud_param_saveButton_clicked()
{
    save_kern_aud_param_from_ui();
    QString str = "/etc/audit/rules.d/audit.rules";
    QString cmdstr = "sed -i \'/-b/\'d "+str+" && sed -i \'/-r/\'d "+str+
            " && sed -i \'/-e/\'d "+str+" && sed -i \'/-f/\'d "+str+" && sed -i \'/-c/\'d "+str+" && sed -i \'/-i/\'d "+str+"; echo $?";
   // qDebug()<<cmdstr;
    QString rs;
    if(!excute_aud_cmd(cmdstr,rs))
    {
        QMessageBox::information(this, tr("提示"), tr("删除原配置项失败"));
        return ;
    }

    cmdstr = "echo -e \"-r "+kernAudParam.rate_limit+ "\n-e "+ kernAudParam.enable+
                  "\n-b "+kernAudParam.backlog_limit+ "\n-f "+kernAudParam.fail_flag+(kernAudParam.bignore?"\n-i":"\n-c")+"\" >>/etc/audit/audit.rules";
 //  qDebug()<<cmdstr;
    if(!excute_aud_cmd(cmdstr, rs))
    {
        QMessageBox::information(this, tr("提示"), tr("保存到配置文件失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("保存到配置文件成功"));
    }
}

void TabAuditPage::on_apl_cfg_Button_clicked()
{
    save_aud_config_from_ui();
    if(audCfgInfo.admin_space_left_action==tr("EMAIL")||audCfgInfo.space_left_action==tr("EMAIL"))
    {
        if(audCfgInfo.warning_mail.isEmpty())
        {
            QMessageBox::information(this, tr("提示"), tr("选择EMAIL时，收件人不能为空"));
            return;
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
//          settings.insert("warning_mail", audCfgInfo.warning_mail);
//          settings.insert("warning_tel", audCfgInfo.warning_tel);
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
    QMessageBox::information(this, tr("提示"), tr("写入完成"));
}


void TabAuditPage::on_pre_pushButton_clicked()
{
    if(ui->findlineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请输入查找关键字"));
        return;
    }
    ui->textBrowser->find(ui->findlineEdit->text(),QTextDocument::FindBackward);
}

void TabAuditPage::on_next_pushButton_clicked()
{
    if(ui->findlineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请输入查找关键字"));
        return;
    }
    ui->textBrowser->find(ui->findlineEdit->text());
}

void TabAuditPage::save_file_rules_from_ui()
{
    if(ui->file_lineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("文件名为空"));
    }
    fileRule.file_name = ui->file_lineEdit->text();
    fileRule.auth = (ui->r_checkBox->isChecked()? QString("r"):QString(""))+
            (ui->w_checkBox->isChecked()?"w":"")+
            (ui->x_checkBox->isChecked()?"x":"");
    fileRule.key_word = ui->kw_file_aud_lineEdit->text();
    fileRule.ts_time = ui->file_aud_st_TimeEdit->text();
    fileRule.te_time = ui->file_aud_et_TimeEdit->text();
}

void TabAuditPage::on_file_rule_aply_pushButton_clicked()
{

    save_file_rules_from_ui();
    if(!set_file_rule(fileRule))
    {
        QMessageBox::information(this, tr("提示"), tr("文件审计规则设置失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("文件审计规则设置成功"));
    }

}

void TabAuditPage::on_brow_pushButton_clicked()
{
    fileRule.file_name = QFileDialog::getOpenFileName(this, tr("Open File"), tr("."));
    if(fileRule.file_name.isEmpty())
    {
        return;
    }
    ui->file_lineEdit->setText(fileRule.file_name);
}

void TabAuditPage::on_file_aud_param_saveButton_clicked()
{
    save_file_rules_from_ui();
    QString cmd = "echo -w "+fileRule.file_name+
            (fileRule.key_word.isEmpty()?"":" -k "+fileRule.key_word)+
            (fileRule.auth.isEmpty()? "":" -p "+fileRule.auth)+
            //+" -ts "+fileRule.ts_time+" -te "+fileRule.te_time;
            ">>"+RULE_CONF_NAME+";echo $?";

    QString res = GetCmdRes(cmd).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        QMessageBox::information(this, tr("提示"), tr("添加到配置文件失败"));
        qDebug()<<"fail command:"<<cmd;
    }else
        QMessageBox::information(this, tr("提示"), tr("添加到配置文件成功"));
}


void TabAuditPage::on_trace_Button_clicked()
{
    if(ui->file_lineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("文件名为空"));
    }
    QString cmd = "autrace -r "+ui->file_lineEdit->text() +";echo $?";
    QString res = GetCmdRes(cmd).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        ui->track_lineEdit->setText(tr("追踪文件失败"));
        qDebug()<<"trace file command:"<<cmd;
    }else
    {
       // ui->track_lineEdit->setText(res.right(res.length()-res.indexOf("with \'")));
        list.removeLast();
        ui->track_lineEdit->setText(list.last());
    }

}

void TabAuditPage::on_sys_call_rule_apl_pushButton_clicked()
{
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


    QString cmdstr = "auditctl -a "+liststr +" -S "+ui->buttonGroup_8->checkedButton()->text()+
            " -ts "+ui->sys_call_st_TimeEdit->text()+" -te "+ui->sys_call_et_TimeEdit->text() + ";echo $?";
    QString res= GetCmdRes(cmdstr).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        qDebug()<<"system call rules failed, command:"<<cmdstr;
        QMessageBox::information(this, tr("提示"), tr("设置失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("设置成功"));
    }

}

void TabAuditPage::on_sys_call_save_pushButton_clicked()
{
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


    QString cmdstr = "echo  \"-a "+liststr +" -S "+ui->buttonGroup_8->checkedButton()->text()+
            " -ts "+ui->sys_call_st_TimeEdit->text()+" -te "+ui->sys_call_et_TimeEdit->text() + "\">>"+RULE_CONF_NAME+";echo $?";
    QString res= GetCmdRes(cmdstr).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        qDebug()<<"system call rules failed, command:"<<cmdstr;
        QMessageBox::information(this, tr("提示"), tr("保存失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("保存成功"));
    }

}

void TabAuditPage::on_list_group_radioButton_clicked()
{
    ui->msgtype_lineEdit->setEnabled(ui->exclude_radioButton->isChecked());
}

void TabAuditPage::on_clean_all_rules_Button_clicked()
{
    QString cmdstr = "auditctl -D; echo $?";
    QString res = GetCmdRes(cmdstr).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        qDebug()<<"clean all rules failed, command:"<<cmdstr;
        QMessageBox::information(this, tr("提示"), tr("清理失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("清理成功"));
    }
}

void TabAuditPage::on_clean_all_rules_infile_Button_clicked()
{
    QString cmdstr = "echo >"+RULE_CONF_NAME+"; echo $?";
    QString res = GetCmdRes(cmdstr).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        qDebug()<<"clean all file rules failed, command:"<<cmdstr;
        QMessageBox::information(this, tr("提示"), tr("清理失败"));
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("清理成功"));
    }
}

void TabAuditPage::on_display_cur_rules_Button_clicked()
{
    QString cmdstr = "auditctl -l; echo $?";
    QString res = GetCmdRes(cmdstr).trimmed();
    QStringList list = res.split('\n');
    if(list.last().toInt()!=0)
    {
        qDebug()<<"display all rules failed, command:"<<cmdstr;
        QMessageBox::information(this, tr("提示"), tr("获取当前规则失败"));
        ui->custom_rules_textBrowser->setText("");
    }else
    {
        QMessageBox::information(this, tr("提示"), tr("获取当前规则成功"));
        ui->custom_rules_textBrowser->setText(res.left(res.length()-1));
    }
}
