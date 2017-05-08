#include "sysusermgrpage.h"
#include "ui_sysusermgrpage.h"

SysUserMgrPage::SysUserMgrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysUserMgrPage)
{
    ui->setupUi(this);
}

SysUserMgrPage::~SysUserMgrPage()
{
    delete ui;
}
