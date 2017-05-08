#include "svrmgrpage.h"
#include "ui_svrmgrpage.h"

SvrMgrPage::SvrMgrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SvrMgrPage)
{
    ui->setupUi(this);
}

SvrMgrPage::~SvrMgrPage()
{
    delete ui;
}
