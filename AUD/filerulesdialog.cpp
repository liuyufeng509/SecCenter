#include "filerulesdialog.h"
#include "ui_filerulesdialog.h"
#include "audfunclass.h"
#include <QFileDialog>
#include <QDirModel>
FileRulesDialog::FileRulesDialog(FileAudRule &fileRule,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileRulesDialog),
    fileRule(fileRule)
{
    ui->setupUi(this);
    ui->stTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->etTimeEdit->setDateTime(QDateTime::currentDateTime());

    QDirModel *model = new QDirModel(this);
    completer = new QCompleter(this);
    completer->setModel(model);
    ui->tracePathEdit->setCompleter(completer);
}

bool FileRulesDialog::save_file_rules_from_ui()
{
    if(ui->filePathEdit->text().isEmpty())
    {
        errMsgBox(tr("文件名为空"), this);
        return false;
    }
    if(ui->stTimeEdit->dateTime()>ui->etTimeEdit->dateTime())
        {
        errMsgBox(tr("起始时间不能超过结束时间"));
        return false;
    }
    fileRule.file_name = ui->filePathEdit->text();
    fileRule.auth = (ui->rcheckBox->isChecked()? QString("r"):QString(""))+
            (ui->wcheckBox->isChecked()?"w":"")+
            (ui->xcheckBox->isChecked()?"x":"")+
            (ui->acheckBox->isChecked()?"a":"");
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
    if(ui->tracePathEdit->text().isEmpty())
    {
        errMsgBox(tr("文件名为空"), this);
        return;
    }
    QString cmd = "autrace -r \""+ui->tracePathEdit->text() +"\"";
    try
    {
        QString rs;
        AudFunClass::getInstance()->excuteAudCmd(cmd, tr("追踪文件"), rs);
       // ui->tracePathEdit->setText(rs);
        QDialog *d = new QDialog(this);
        d->setWindowTitle(tr("文件追踪信息"));
        QTextBrowser *browser = new QTextBrowser;
        QVBoxLayout *vlayout = new QVBoxLayout(d);
        QPushButton *ok = new QPushButton(tr("确定"));
        ok->setMaximumWidth(70);
        vlayout->addWidget(browser);
        vlayout->addWidget(ok);
        browser->setText(rs);
        d->resize(500,300);
        connect(ok, SIGNAL(clicked()), d, SLOT(accept()));
        d->exec();
        delete d;
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
      //  ui->tracePathEdit->setText(tr("追踪文件失败"));
    }
}
