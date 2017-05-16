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
        errMsgBox(tr("日志文件打开失败"));
    }
}

void AudInfoPage::queryAction(QString cmd)
{
    try
    {
        QString res = "";
        AudFunClass::getInstance()->excuteAudCmd(cmd, tr("审计查询"), res);
        ui->textBrowser->setText(res);
        op_type = QUERY_SET;
        ui->fresh_pushButton->setToolTip(tr("刷新类型：审计查询"));
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}

void AudInfoPage::reportAction(QString cmd)
{
    try
    {
        QString res = "";
        AudFunClass::getInstance()->excuteAudCmd(cmd, tr("审计报告生成"), res);
        ui->textBrowser->setText(res);
        op_type=AUD_REPORT;
        ui->fresh_pushButton->setToolTip(tr("刷新类型：审计报表"));
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}

void AudInfoPage::on_queryButton_clicked()
{
    QueryDialog qd;
    if(qd.exec()==QDialog::Accepted)
    {
        cmd  = qd.getCmdStence();
        queryAction(cmd);
    }else
        cmd = "";
}

void AudInfoPage::on_reportButton_clicked()
{
    ReportDialog rd;
    if(rd.exec()==QDialog::Accepted)
    {
        cmd  = rd.getCmdStence();
        reportAction(cmd);
    }else
        cmd = "";
}

void AudInfoPage::on_fresh_pushButton_clicked()
{
    ui->textBrowser->setText("");
    switch (op_type) {
    case QUERY_SET:
        //on_aplButton_clicked();
        if(cmd.isEmpty())
        {
            errMsgBox(tr("刷新失败，查询语句为空"));
            break;
        }
        queryAction(cmd);
        break;
    case AUD_REPORT:
        //on_report_okButton_clicked();
        if(cmd.isEmpty())
        {
            errMsgBox(tr("刷新失败，查询语句为空"));
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
