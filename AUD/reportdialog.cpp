#include "reportdialog.h"
#include "ui_reportdialog.h"

ReportDialog::ReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportDialog)
{
    ui->setupUi(this);
}

ReportDialog::~ReportDialog()
{
    delete ui;
}

void ReportDialog::on_okButton_clicked()
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
    accept();
}

void ReportDialog::on_res_checkBox_clicked()
{
    ui->sucButton->setEnabled(ui->res_checkBox->isChecked());
    ui->failButton->setEnabled(ui->res_checkBox->isChecked());
}

void ReportDialog::on_tm_sec_checkBox_clicked()
{
    ui->st_TimeEdit->setEnabled(ui->tm_sec_checkBox->isChecked());
    ui->et_TimeEdit->setEnabled(ui->tm_sec_checkBox->isChecked());
    ui->st_TimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->et_TimeEdit->setDateTime(QDateTime::currentDateTime());
}

void ReportDialog::on_cancelButton_clicked()
{
    reject();
}
