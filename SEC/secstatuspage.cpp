#include "secstatuspage.h"
#include "ui_secstatuspage.h"

SecStatusPage::SecStatusPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecStatusPage)
{
    ui->setupUi(this);
}

SecStatusPage::~SecStatusPage()
{
    delete ui;
}
