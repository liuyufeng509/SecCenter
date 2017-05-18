#include "filerulesdialog.h"
#include "ui_filerulesdialog.h"
#include "audfunclass.h"
#include <QFileDialog>
FileRulesDialog::FileRulesDialog(FileAudRule &fileRule,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileRulesDialog),
    fileRule(fileRule)
{
    ui->setupUi(this);
    ui->stTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->etTimeEdit->setDateTime(QDateTime::currentDateTime());
}

bool FileRulesDialog::save_file_rules_from_ui()
{
    if(ui->filePathEdit->text().isEmpty())
    {
        errMsgBox(tr("文件名为空"));
        return false;
    }
    fileRule.file_name = ui->filePathEdit->text();
    fileRule.auth = (ui->rcheckBox->isChecked()? QString("r"):QString(""))+
            (ui->wcheckBox->isChecked()?"w":"")+
            (ui->xcheckBox->isChecked()?"x":"");
    fileRule.key_word = ui->kwlineEdit->text();
    fileRule.ts_time = ui->stTimeEdit->text();
    fileRule.te_time = ui->etTimeEdit->text();
    return true;
}

FileRulesDialog::~FileRulesDialog()
{
    delete ui;
}

void FileRulesDialog::on_okButton_clicked()
{
    if(!save_file_rules_from_ui())
        {
        return;
    }
//    cmd = "auditctl -w "+fileRule.file_name+
//            (fileRule.key_word.isEmpty()?"":" -k "+fileRule.key_word)+
//            (fileRule.auth.isEmpty()? "":" -p "+fileRule.auth)+
//            +" -ts "+fileRule.ts_time+" -te "+fileRule.te_time;
    accept();
}

void FileRulesDialog::on_cancelButton_clicked()
{
    reject();
}

void FileRulesDialog::on_browFileButton_clicked()
{
    fileRule.file_name = QFileDialog::getOpenFileName(this, tr("Open File"), tr("."));
    if(fileRule.file_name.isEmpty())
    {
        return;
    }
    ui->filePathEdit->setText(fileRule.file_name);
}

void FileRulesDialog::on_traceButton_clicked()
{
    if(ui->filePathEdit->text().isEmpty())
    {
        errMsgBox(tr("文件名为空"));
        return;
    }
    QString cmd = "autrace -r \""+ui->filePathEdit->text() +"\"";
    try
    {
        QString rs;
        AudFunClass::getInstance()->excuteAudCmd(cmd, tr("追踪文件"), rs);
        ui->trackRslineEdit->setText(rs);
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat());
        ui->trackRslineEdit->setText(tr("追踪文件失败"));
    }
}
