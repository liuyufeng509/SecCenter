#include "svrctrlpage.h"
#include "ui_svrctrlpage.h"

SvrCtrlPage::SvrCtrlPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SvrCtrlPage)
{
    ui->setupUi(this);
}

SvrCtrlPage::~SvrCtrlPage()
{
    delete ui;
}

void SvrCtrlPage::on_openAllButton_clicked()
{

}

void SvrCtrlPage::on_open_closeSecPolButton_clicked()
{
    warnMsgBox(tr("关闭安全策略会使系统安全性降低，确定要进行该项操作？"));
}
