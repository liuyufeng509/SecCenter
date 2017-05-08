#include "usermgrpage.h"
#include "ui_usermgrpage.h"

UserMgrPage::UserMgrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMgrPage)
{
    ui->setupUi(this);
}

UserMgrPage::~UserMgrPage()
{
    delete ui;
}
