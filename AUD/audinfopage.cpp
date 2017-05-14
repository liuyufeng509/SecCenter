#include "audinfopage.h"
#include "ui_audinfopage.h"
#include<QFileDialog>
#include "reportdialog.h"
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
    QString filePathTmp = QFileDialog::getOpenFileName(this, tr("Open File"),"");
    if(filePathTmp.isEmpty())
    {
        return;
    }
}

void AudInfoPage::on_queryButton_clicked()
{
    QueryDialog qd;
    qd.exec();
}

void AudInfoPage::on_reportButton_clicked()
{
    ReportDialog rd;
    rd.exec();
}
