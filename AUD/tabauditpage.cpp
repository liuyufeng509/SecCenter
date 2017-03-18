#include "tabauditpage.h"
#include "ui_tabauditpage.h"
#include<QFileDialog>
#include "querydialog.h"
#include"reportdialog.h"
#include"ruledialog.h"
#include"configdialog.h"
TabAuditPage::TabAuditPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabAuditPage)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));

}

TabAuditPage::~TabAuditPage()
{
    delete ui;
}

void TabAuditPage::on_openButton_clicked()
{
    QFileDialog::getOpenFileName(this, tr("打开文件"), ".", NULL);
}

//void TabAuditPage::on_queryButton_clicked()
//{
//    QueryDialog d;
//    d.exec();
//}

//void TabAuditPage::on_reportButton_clicked()
//{
//    ReportDialog  d;
//    d.exec();
//}

//void TabAuditPage::on_roleButton_clicked()
//{
//    RuleDialog d;
//    d.exec();
//}

//void TabAuditPage::on_setCfgButton_clicked()
//{
//    ConfigDialog d;
//    d.exec();
//}
