#include "querydialog.h"
#include "ui_querydialog.h"

QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
    setAuSearchUI();
    connCheckBoxAndLineEdit();
    ui->st_timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->et_timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->st_checkBox->setHidden(true);
    ui->et_checkBox->setHidden(true);
    ui->st_timeEdit->setHidden(true);
    ui->et_timeEdit->setHidden(true);
}

void QueryDialog::setAuSearchUI()
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
    ui->uid_lineEdit->setEnabled(ui->uid_checkBox->isChecked());
    ui->luid_lineEdit->setEnabled(ui->luid_checkBox->isChecked());
    ui->syscall_lineEdit->setEnabled(ui->syscall_checkBox->isChecked());
    ui->st_timeEdit->setEnabled(ui->st_checkBox->isChecked());
    ui->et_timeEdit->setEnabled(ui->et_checkBox->isChecked());
    ui->st_timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->et_timeEdit->setDateTime(QDateTime::currentDateTime());
}

void QueryDialog::connCheckBoxAndLineEdit()
{
    connect(ui->evidCheckBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->gid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->agid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->egid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->kw_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->hn_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->mt_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->rs_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->ppid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->pid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->fn_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->word_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->term_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->ef_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->euid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->uid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->luid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->syscall_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->auid_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->st_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
    connect(ui->et_checkBox, SIGNAL(clicked(bool)), this, SLOT(setAuSearchUI()));
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

#define EMPTY(param, param2) \
   {    \
    if(param->text().isEmpty() && param2->isChecked())    \
    {   \
        errMsgBox(param->objectName()+tr("参数为空")); \
        return false;   \
    }   \
}

bool QueryDialog::check_param()
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

void QueryDialog::on_query_produceButton_clicked()
{
    if(!check_param())
    {
        //cmd = "";
        return;
    }

    querySt = "ausearch -i"+
            (ui->evidCheckBox->isChecked()? " --event "+ui->evid_lineEdit->text():"")+
            (ui->gid_checkBox->isChecked()? " -gi "+ui->gid_lineEdit->text():"") +
            (ui->fn_checkBox->isChecked()? " -f "+ui->fn_lineEdit->text():"") +
            (ui->agid_checkBox->isChecked()? " -ga "+ui->agid_lineEdit->text():"")+
            (ui->egid_checkBox->isChecked()? " -ge "+ui->egid_lineEdit->text():"")+
            (ui->kw_checkBox->isChecked()? " -k "+ui->kw_lineEdit->text():"") +
            (ui->hn_checkBox->isChecked()?" -hn "+ui->hn_lineEdit->text():"")+
            (ui->ppid_checkBox->isChecked()? " -pp "+ui->ppid_lineEdit->text():"")+
            (ui->mt_checkBox->isChecked()? " -m " +ui->mt_lineEdit->text():"")+
            (ui->rs_checkBox->isChecked()? " -sv "+ui->rs_lineEdit->text():"") +
            (ui->pid_checkBox->isChecked()? " -p " +ui->pid_lineEdit->text():"")+
            (ui->st_checkBox->isChecked()? " -ts "+ui->st_timeEdit->text():"")+
            (ui->et_checkBox->isChecked()? " -te "+ui->et_timeEdit->text():"")+
            (ui->auid_checkBox->isChecked()? " -ua "+ui->auid_lineEdit->text():"") +
            (ui->term_checkBox->isChecked()? " -tm "+ui->term_lineEdit->text():"")+
            (ui->word_checkBox->isChecked()? " -w "+ui->word_lineEdit->text():"")+
            (ui->euid_checkBox->isChecked()? " -ue "+ui->euid_lineEdit->text():"")+
            (ui->luid_checkBox->isChecked()? " -ul "+ui->luid_lineEdit->text():"")+
            (ui->ef_checkBox->isChecked()? " -x "+ui->ef_lineEdit->text():"")+
            (ui->uid_checkBox->isChecked()? " -ui "+ui->uid_lineEdit->text():"")+
            (ui->syscall_checkBox->isChecked()? " -sc "+ui->syscall_lineEdit->text():"")+ "";
    ui->query_sent_lineEdit->setText(querySt);
}

void QueryDialog::on_okButton_clicked()
{
    querySt = ui->query_sent_lineEdit->text();
    if(querySt.isEmpty())
    {
        errMsgBox(tr("查询语句不能为空"), this);
        return;
    }
    if(ui->st_checkBox->isChecked() && ui->et_checkBox->isChecked())
    {
        if(ui->st_timeEdit->dateTime()>ui->et_timeEdit->dateTime())
        {
            errMsgBox(tr("起始时间不能超过结束时间"),this);
            return;
        }
    }
    accept();
}

void QueryDialog::on_cancelButton_clicked()
{
    reject();
}
