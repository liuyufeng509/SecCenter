#include "audinfopage.h"
#include "ui_audinfopage.h"
#include<QFileDialog>
#include <QTextStream>
#include "reportdialog.h"
#define CONF_NAME "/etc/audit/auditd.conf"
AudInfoPage::AudInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudInfoPage)
{
    ui->setupUi(this);
}

AudInfoPage::~AudInfoPage()
{
    delete ui;
}

void AudInfoPage::on_openButton_clicked()
{
    QString iniFilePath = CONF_NAME;
    QSettings settings(iniFilePath,QSettings::IniFormat);
    QString log_file = settings.value("log_file").toString();
    QString filePathTmp = QFileDialog::getOpenFileName(this, tr("Open File"),log_file);
    if(filePathTmp.isEmpty())
    {
        return;
    }
    fileName = filePathTmp;
    read_and_display_file();
}

void AudInfoPage::read_and_display_file()
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
        errMsgBox(tr("日志文件打开失败"), this);
    }
}

void AudInfoPage::queryAction(QString cmd)
{
    QString res = "";
    try
    {
        AudFunClass::getInstance()->excuteAudCmd(cmd, tr("审计查询"), res);
        ui->textBrowser->setText(res);
        op_type = QUERY_SET;
        ui->fresh_pushButton->setToolTip(tr("刷新类型：审计查询"));
    }catch(Exception exp)
    {
        if(exp.getErroNo().toInt()==1)
            {
            ui->textBrowser->setText(res);
            op_type = QUERY_SET;
            ui->fresh_pushButton->setToolTip(tr("刷新类型：审计查询"));
        }else
            errMsgBox(exp.getErroWhat(), this);
    }
}

void AudInfoPage::reportAction(QString cmd)
{
     QString res = "";
    try
    {
        AudFunClass::getInstance()->excuteAudCmd(cmd, tr("审计报告生成"), res);
        ui->textBrowser->setText(res);
        op_type=AUD_REPORT;
        ui->fresh_pushButton->setToolTip(tr("刷新类型：审计报告"));
    }catch(Exception exp)
    {
        if(exp.getErroNo().toInt()==1)
            {
            ui->textBrowser->setText(res);
            op_type=AUD_REPORT;
            ui->fresh_pushButton->setToolTip(tr("刷新类型：审计报告"));
        }else
            errMsgBox(exp.getErroWhat(), this);
    }
}

void AudInfoPage::on_queryButton_clicked()
{
    QueryDialog qd;
    if(qd.exec()==QDialog::Accepted)
    {
        cmd  = qd.getCmdStence();
        queryAction(cmd);
    }/*else
        cmd = "";*/
}

void AudInfoPage::on_reportButton_clicked()
{
    ReportDialog rd;
    if(rd.exec()==QDialog::Accepted)
    {
        cmd  = rd.getCmdStence();
        reportAction(cmd);
    }/*else
        cmd = "";*/
}

void AudInfoPage::on_fresh_pushButton_clicked()
{
    ui->textBrowser->setText("");
    switch (op_type) {
    case QUERY_SET:
        //on_aplButton_clicked();
        if(cmd.isEmpty())
        {
            errMsgBox(tr("刷新失败，查询语句为空"), this);
            break;
        }
        queryAction(cmd);
        break;
    case AUD_REPORT:
        //on_report_okButton_clicked();
        if(cmd.isEmpty())
        {
            errMsgBox(tr("刷新失败，查询语句为空"), this);
            break;
        }
        reportAction(cmd);
        break;
    case AUD_DISPLAY:
        read_and_display_file();
        break;
    default:
        break;
    }
}

void AudInfoPage::on_pre_pushButton_clicked()
{
    if(ui->findlineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请输入查找关键字"));
        return;
    }
    if(!ui->textBrowser->find(ui->findlineEdit->text(),QTextDocument::FindBackward))
        infoMsgBox(tr("未查找到关键字"), this);
}

void AudInfoPage::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return )
    {
//        if(ui->listWidget->currentRow() == 0)           //审计查询
//            on_aplButton_clicked();
//        else if(ui->listWidget->currentRow() == 1)      //审计报告
//            on_report_okButton_clicked();
//        else if(ui->listWidget->currentRow() == 3)      //审计配置
//            on_apl_cfg_Button_clicked();
        on_next_pushButton_clicked();
    }

    QWidget::keyPressEvent(event);
}

void AudInfoPage::on_next_pushButton_clicked()
{
    if(ui->findlineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("请输入查找关键字"));
        return;
    }
    if(!ui->textBrowser->find(ui->findlineEdit->text()))
        infoMsgBox(tr("未查找到关键字"), this);
}
