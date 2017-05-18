#include "sysadmform.h"
#include "ui_sysadmform.h"

SysAdmForm::SysAdmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysAdmForm)
{
    ui->setupUi(this);

    indexPage = new IndexWidget(SYSADMIN);
    helpPage = new HelpPage(SYSADMIN);
    userMgrPage = new SysUserMgrPage;
    svrMgrPage = new SvrMgrPage;
    sysLogPage = new SysLogPage;
    kernalPamPage = new KernalParamPage;
    ui->stackedWidget->addWidget(indexPage);
    ui->stackedWidget->addWidget(userMgrPage);
    ui->stackedWidget->addWidget(svrMgrPage);
    ui->stackedWidget->addWidget(sysLogPage);
    ui->stackedWidget->addWidget(kernalPamPage);
    ui->stackedWidget->addWidget(helpPage);
    ui->indexButton->setChecked(true);
}

SysAdmForm::~SysAdmForm()
{
    delete ui;
}

void SysAdmForm::on_svrMgrButton_toggled(bool checked)
{
    if(checked)
    ui->stackedWidget->setCurrentWidget(svrMgrPage);
}

void SysAdmForm::on_userMgrButton_toggled(bool checked)
{
    if(checked)
    ui->stackedWidget->setCurrentWidget(userMgrPage);
}

void SysAdmForm::on_indexButton_toggled(bool checked)
{
    if(checked)
    ui->stackedWidget->setCurrentWidget(indexPage);
}

void SysAdmForm::on_syslogButton_toggled(bool checked)
{
    if(checked)
    ui->stackedWidget->setCurrentWidget(sysLogPage);
}

void SysAdmForm::on_kernParamButton_toggled(bool checked)
{
    if(checked)
    ui->stackedWidget->setCurrentWidget(kernalPamPage);
}

void SysAdmForm::on_helpButton_toggled(bool checked)
{
    if(checked)
    ui->stackedWidget->setCurrentWidget(helpPage);
}
